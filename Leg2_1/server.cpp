#include "tool.h"

#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <fcntl.h>

int main(){
    int server = socket(AF_INET, SOCK_STREAM, 0);
    err(server == -1, "socket创建错误");

    sockaddr_in server_addr;
    memset(&server_addr, '\0', sizeof server_addr);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port        = htons(10086);

    err(
        bind(server, (sockaddr*)&server_addr, sizeof server_addr) == -1,
        "绑定服务端地址错误"
    );

    err(
        listen(server, 0) == -1,
        "服务端监听错误"
    );

    // 服务端设置为非阻塞
    fcntl(server, F_SETFL, O_NONBLOCK);

    while(true){
        // 接受客户端连接
        sockaddr_in client_addr;
        memset(&client_addr, '\0', sizeof client_addr);
        socklen_t   client_addr_len = sizeof client_addr;
        int client = accept(server, (sockaddr*)&server_addr, &client_addr_len);
        if( war(client==-1, "接受客户端连接错误") ){
            sleep(1);   // 休眠 1 秒
            continue;
        }
        else
            std::cout << "\n新的连接：" << client << " IP地址：" << inet_ntoa(client_addr.sin_addr) << std::endl;
        
        // 客户端设置为非阻塞
        fcntl(client, F_SETFL, O_NONBLOCK);
        char buf[1024];
        while(true){
            // 接收数据
            memset(buf, '\0', sizeof buf);        
            int len = recv(client, buf, sizeof buf, 0);
            if( war(len <= 0, "接收数据错误") ){
                sleep(1);   // 休眠一秒
                continue;
            } else 
                std::cout << "\n接收数据成功，长度：" << len << "；内容：" << buf << std::endl;

            // 发送数据
            memset(buf, '\0', sizeof buf);
            std::cout << "输入要发送的数据：";
            scanf("%s", buf);
            if(strcmp(buf, "exit") == 0){
                close(client);
                break;
            }
            if( war(send(client, buf, strlen(buf), 0) <= 0, "发送数据错误") )
                break;
            else
                std::cout << "发送数据成功！\n" << std::endl;
        }
    }
    close(server);
}
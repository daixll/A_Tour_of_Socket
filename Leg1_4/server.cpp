#include "tool.h"

#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <fstream>
#include <string>

int main(int argc, char* args[]){
    int server = socket(AF_INET, SOCK_STREAM, 0);
    err(server == -1, "socket创建错误");

    sockaddr_in server_addr;
    memset(&server_addr, '\0', sizeof server_addr);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port        = htons(std::stoi(args[1]));

    err(
        bind(server, (sockaddr*)&server_addr, sizeof server_addr) == -1,
        "绑定服务端地址错误"
    );

    err(
        listen(server, 5) == -1,
        "服务端监听错误"
    );

    while(true){
        // 接受客户端连接
        sockaddr_in client_addr;
        memset(&client_addr, '\0', sizeof client_addr);
        socklen_t   client_addr_len = sizeof client_addr;
        int client = accept(server, (sockaddr*)&client_addr, &client_addr_len);
        if( war(client==-1, "接受客户端连接错误") )
            continue;
        else
            std::cout << "\n新的连接：" << client << " IP地址：" << inet_ntoa(client_addr.sin_addr) << std::endl;

        char buf[10240];
        while(true){ 
            // 接收数据
            memset(buf, '\0', sizeof buf);
            std::cout << "等待接受数据" << client << std::endl;      
            int len = recv(client, buf, sizeof buf, 0);
            if( war(len <= 0, "接收数据错误") ){
                close(client);
                break;
            } else 
                std::cout << "\n接收数据成功，长度：" << len << "；内容：\n" << buf << std::endl;

            // 发送数据
            std::string content = "HTTP/1.1 200 OK\r\n";// 1. 组装响应行
            content += "Connection: keep-alive\r\n";    // 2. 组装响应头
            content += "\r\n";                          // 3. 组装空行
            std::ifstream file("index.html");           // 4. 组装响应体
            std::string   line;                         //    读取文件
            while( std::getline(file, line) )
                content += line;
            // 发送
            if( war(send(client, content.c_str(), content.size(), 0) <= 0, "发送数据错误") )
                break;
            else
                std::cout << "发送数据成功！\n" << std::endl;

            close(client);
            break;
        }
    }

    close(server);
    return 0;
}
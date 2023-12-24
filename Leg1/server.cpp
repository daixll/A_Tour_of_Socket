#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>

void err(bool flg, std::string msg){
    if(!flg) return ;   // 如果没有错误，直接返回
    std::cerr << msg << "\n错误代码：" << errno << std::endl;
    exit(1);
}

bool war(bool flg, std::string msg){
    if(!flg) return 0;  // 如果没有警告，返回 0
    std::cerr << msg << "\n警告代码：" << errno << std::endl;
    return 1;
}

int main(){
    int server = socket(AF_INET, SOCK_STREAM, 0);
    err(server == -1, "socket创建错误");

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port        = htons(8080);
    
    int reuse = 1;  // 端口复用
    setsockopt(reuse, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse, sizeof reuse);

    err(
        bind(server, (sockaddr*)&server_addr, sizeof server_addr) == -1,
        "绑定服务端地址错误"
    );

    err(
        listen(server, 5) == -1,
        "服务端监听错误"
    );

    while(1){
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof client_addr;
        int client = accept(server, (sockaddr*)&server_addr, &client_addr_len);
        if( war( client==-1, "接受客户端连接错误") ) continue;
        std::cout << "\n新的连接：" << client << " IP地址：" << inet_ntoa(client_addr.sin_addr) << std::endl;

        while(1){
            char buf[1024];
            // 接收数据
            memset(buf, '\0', sizeof buf); 
            int len = recv(client, buf, sizeof buf, 0);
            if(len <= 0){
                std::cout<<client<<" "<<"客户端断开连接\n";
                close(client);
                break;
            }
            std::cout << "\n接收数据成功，长度：" << len << "；内容：" << buf <<"\n";
            // 输入数据
            memset(buf, '\0', sizeof buf);
            std::cout<<"输入要发送的数据："; std::cin>>buf;
            send(client, buf, strlen(buf), 0);
        }
    }

    close(server);
    return 0;
}
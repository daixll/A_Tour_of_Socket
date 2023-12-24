#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

int main(){
    sockaddr_in server_addr;
    server_addr.sin_family       = AF_INET;                 // IPv4
    server_addr.sin_addr.s_addr  = inet_addr("127.0.0.1");  // IP 
    server_addr.sin_port         = htons(8081);             // 端口

    int client = socket(AF_INET, SOCK_DGRAM, 0);            // 1. 创建套接字
    while(1){
        char buf[1024]; std::cin>>buf;                      // 2. 发送数据
        sendto(client, buf, strlen(buf), 0, (sockaddr*)&server_addr, sizeof server_addr);
        memset(buf, 0, sizeof buf);                         // 3. 接收数据
        std::cout<<"完成输入"<<std::endl;
        recvfrom(client, buf, sizeof buf, 0, NULL, NULL);
        std::cout<<buf<<std::endl;
        std::cout<<"完成接收"<<std::endl;
    }
    return 0;
}
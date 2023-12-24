#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

int main(){
    sockaddr_in server_addr, client_addr; memset(&client_addr, 0, sizeof client_addr);
    server_addr.sin_family       = AF_INET;                  // IPv4
    server_addr.sin_addr.s_addr  = inet_addr("127.0.0.1");   // IP
    server_addr.sin_port         = htons(8081);              // 端口
    
    int server = socket(AF_INET, SOCK_DGRAM, 0);                // 1. 创建套接字
    bind(server, (sockaddr*)&server_addr, sizeof server_addr);  // 2. 绑定套接字
    
    socklen_t client_addr_len = sizeof client_addr;
    while(1){
        char buf[1024]; memset(buf, 0, sizeof buf);             // 3. 接收数据
        recvfrom(server, buf, sizeof buf, 0, (sockaddr*)&client_addr, &client_addr_len);
        std::cout<<buf<<std::endl;                              // 4. 发送数据
        sendto(server, "我已收到", strlen("我已收到"), 0, (sockaddr*)&client_addr, sizeof client_addr);    
    }
    return 0;
}
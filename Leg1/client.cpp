#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
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
    int client = socket(AF_INET, SOCK_STREAM, 0);
    err(client == -1, "socket创建错误");
    
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port        = htons(8081);
    err(
        connect(client, (sockaddr*)&server_addr, sizeof server_addr) == -1,
        "连接服务端错误"
    );

    char buf[1024];
    while(1){
        // 输入数据
        memset(buf, '\0', sizeof buf);
        std::cout<<"输入要发送的数据：";
        std::cin>>buf;
        if(strcmp(buf, "quit")==0) break;
        send(client, buf, strlen(buf), 0);
        // 接收数据
        memset(buf, '\0', sizeof buf);
        int len = recv(client, buf, sizeof buf, 0);
        std::cout<<"\n接收数据成功，长度:"<<len<<"；内容："<<buf<<"\n";
    }
    close(client);
    return 0;
}

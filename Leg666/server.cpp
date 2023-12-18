#include "socket.hpp"

void func(int fd){
    char buf[1024];
    memset(buf, 0, sizeof buf);
    int len = recv(fd, buf, sizeof buf, 0);
    std::cout<<"我已收到！";
    char msg[] = "我已收到！";
    send(fd, msg, strlen(msg), 0);
}

int main(){
    jiao::TcpServer* a = new jiao::TcpServer("127.0.0.1", 8081);
    a->deal_func(func); // 设置处理函数
    a->run(10);         // 服务器启用 10 个线程
    delete a;
    return 0;
}
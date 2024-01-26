#include "../Headers/Sock.h"

using namespace jiao;

Sock::Sock(){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    err(fd == -1,
        "套接字创建失败");
}

Sock::Sock(const std::string& ip, const int& port){
    if(ip == "" || port == 0){
        fd = -1;
        addr = new SockAddr("", 0);
        return;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    err(fd == -1,
        "套接字创建失败");
    
    addr = new SockAddr(ip, port);
    err(bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1,
        "套接字绑定失败");
}

Sock::~Sock(){
    delete addr;
    close(fd);
}

void Sock::Listen(const int& num){
    err(listen(fd, num) == -1,
        "监听失败");
}

Sock Sock::Ac(){
    Sock new_sock("", 0);   // 用于接受连接的套接字

    std::cout << "等待连接..." << std::endl;
    new_sock.fd = accept(fd, (sockaddr*)&new_sock.addr->addr, &new_sock.addr->addr_len);
    err(new_sock.fd == -1,
        "接受连接失败");

    return new_sock;
}

void Sock::Conn(const std::string& ip, const int& port){
    SockAddr Server(ip, port);
    err(connect(fd, (sockaddr*)&Server.addr, Server.addr_len) == -1,
        "连接失败");
}

void Sock::Send(const std::string& msg){
    err(send(fd, msg.c_str(), msg.size(), 0) == -1,
        "发送失败");
}

std::string Sock::Recv(){
    if(!buf_init){
        buf = new char[BUFSIZE];
        buf_init = true;
    }
    memset(buf, '\0', BUFSIZE);
    int len = recv(fd, buf, BUFSIZE, 0);
    err(len == -1,
        "接收失败");
    return buf;
}
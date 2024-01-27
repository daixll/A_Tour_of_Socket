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

Sock* Sock::Ac(){
    Sock* new_sock = new Sock("", 0);
    new_sock->fd = accept(fd, (sockaddr*)&new_sock->addr->addr, &new_sock->addr->addr_len);

    if(non_block)           // 非阻塞情况下
        err(new_sock->fd == -1 && errno != EAGAIN && errno != EWOULDBLOCK,
            "非阻塞: 接受连接失败");
    else                    // 阻塞情况下
        err(new_sock->fd == -1,
            "接受连接失败");

    return new_sock;
}

bool Sock::Conn(const std::string& ip, const int& port){
    SockAddr* Server = new SockAddr(ip, port);
    if(war(connect(fd, (sockaddr*)&Server->addr, Server->addr_len) == -1,
        "连接失败"))
        return false;
    return true;
}

bool Sock::Send(const std::string& msg){
    int len = send(fd, msg.c_str(), msg.size(), 0);
    
    if(war(len == 0,
        "对方已关闭连接"))
        return false;
    
    if(non_block)   // 非阻塞情况下，再试一次的情况
        if(len == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
            return true;

    war(len == -1,
        "发送失败"); 
    
    return true;
}

std::string Sock::Recv(){
    if(!buf_init){
        buf = new char[BUFSIZE];
        buf_init = true;
    }   // 初始化缓冲区
    memset(buf, '\0', BUFSIZE);

    int len = recv(fd, buf, BUFSIZE, 0);
    
    if(war(len == 0,
        "对方已关闭连接"))
        return "kill";
    
    if(non_block)   // 非阻塞情况下，再试一次的情况
        if(len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
            return "";
        
    war(len == -1,
        "理论阻塞: 接收失败");

    return buf;
}
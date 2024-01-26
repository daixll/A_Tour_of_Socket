#pragma once

#include "Tool.h"
#include "SockAddr.h"
#include "unistd.h"

#include <fcntl.h>

#define BUFSIZE 1024

namespace jiao{

class Sock
{

public:
    Sock();                                         // 仅创建套接字
    Sock(const std::string& ip, const int& port);   // 创建套接字并绑定
    ~Sock();
    
    void Listen(const int& num=0);                      // 监听
    Sock Ac();                                          // 接受一个连接
    void Conn(const std::string& ip, const int& port);  // 连接

    void Send(const std::string& msg);                  // 发送
    std::string Recv();                                 // 接收

    void setNonBlock() {                                // 设置非阻塞
        fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL));
    }

    SockAddr*   addr;           // 套接字地址
    int         fd;             // 套接字
private:
    char*       buf;            // 缓冲区
    bool        buf_init=false; // 缓冲区是否初始化
};

}   // namespace jiao
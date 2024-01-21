#pragma once
#include "tool.h"

#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>

namespace jiao{
class cc{

public:
    cc(const std::string& ip = "0.0.0.0", const int& port = 10086);
    ~cc();

    void start();   // 开始运行
private:
    int conn(const std::string& ip, const int& port);   // 主动连接
    void sendMsg(const int& target, std::string& msg);  // 主动发送
    std::string recvMsg(const int& target);             // 主动接收
    int acc();                                          // 接受一个连接

    int server;                                         // 接受连接
    sockaddr_in server_addr;                            // 服务器地址
    std::set<int> cs;                                   // 连接的客户端 or 服务器
    char* buf;                                          // 缓冲区

};
}   // namespace jiao
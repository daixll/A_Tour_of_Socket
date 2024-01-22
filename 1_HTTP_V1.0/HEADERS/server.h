#pragma once
#include "../HEADERS/tool.h"
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>

namespace jiao{

class SockAddr
{
public:
    SockAddr(const std::string& ip="0.0.0.0", const int& port=10086);
    ~SockAddr();

    sockaddr_in* get_sock_addr() const { return sock_addr; }
    socklen_t*   get_sock_addr_len() const { return sock_addr_len; }

private:
    sockaddr_in* sock_addr;
    socklen_t*   sock_addr_len;
};

class Accepter
{
public:
    Accepter(const int& sock);
    ~Accepter();

    int ac(const int sock);

private:
    sockaddr_in* client_addr;
    socklen_t*   client_addr_len;
};


class HTTPServer
{

public:
    HTTPServer(const std::string& ip="0.0.0.0", const int& port=10086);
    ~HTTPServer();
    void run(std::function<std::string(std::string)> deal);

private:
    char buf[1024];
    std::string recvMsg(const int& client);
    bool sendMsg(const int& client, std::string& msg);
    int sock;
};

}
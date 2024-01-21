#include "HEADERS/server.h"

using namespace jiao;

SockAddr::SockAddr(const std::string& ip, const int& port){
    sock_addr     = new sockaddr_in;
    sock_addr_len = new socklen_t;

    memset(sock_addr, '\0', sizeof *sock_addr);
    sock_addr->sin_family      = AF_INET;
    sock_addr->sin_addr.s_addr = inet_addr(ip.c_str());
    sock_addr->sin_port        = htons(port);
    *sock_addr_len = sizeof *sock_addr;
}


SockAddr::~SockAddr(){
    delete sock_addr;
    delete sock_addr_len;
}

Accepter::Accepter(const int& sock){
    SockAddr* client_addr = new SockAddr();
}

Accepter::~Accepter(){
    delete client_addr;
}

int Accepter::ac(int sock){
    memset(client_addr->get_sock_addr(), '\0', *client_addr->get_sock_addr_len());
    return accept(sock, (sockaddr*)client_addr->get_sock_addr(), client_addr->get_sock_addr_len());
}

HTTPServer::HTTPServer(const std::string& ip="0.0.0.0", const int& port=10086){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    err(sock == -1, "socket创建错误");

    SockAddr* server_addr = new SockAddr(ip, port);
    err(
        bind(sock, (sockaddr*)server_addr->get_sock_addr(), *server_addr->get_sock_addr_len()) == -1,
        "绑定服务端地址错误"
    );
}

HTTPServer::~HTTPServer(){}

void HTTPServer::run(std::function<std::string(std::string)> deal){
    err(
        listen(sock, 0) == -1,
        "服务端监听错误"
    );

    // 创建一个接收器
    Accepter* ac = new Accepter(sock);
    while(true){
        int client = ac->ac(sock);

        if( war(client==-1, "接受客户端连接错误") ) continue;
        else std::cout << "\n新的连接：" << client << std::endl;

        while(true){
            // 接收数据
            std::string content = recvMsg(client);
            if(content == "") {
                close(client);
                break;
            }

            // 处理数据
            content = deal(content);

            // 发送数据
            content = sendMsg(client, content);
            if(content == "") {
                close(client);
                break;
            }

        }
    }
}

std::string HTTPServer::recvMsg(const int& client){
    // 循环接收，直到接收完
    std::string content = "";
    
}

std::string HTTPServer::sendMsg(const int& client, const std::string& msg){
    
}
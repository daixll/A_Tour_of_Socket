#include "../HEADERS/Conn.h"

using namespace jiao;

Conn::Conn(const std::string& ip, const int& port){
    addr = new SockAddr(ip, port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(war(sock == -1, "创建套接字错误")) return;

    fcntl(sock, F_SETFL, ~O_NONBLOCK); // 设置为阻塞

    int ret = connect(sock, (sockaddr*)addr->get_addr(), *addr->get_addr_len());
    if(war(ret == -1, "连接套接字错误 " + ip + ":" + std::to_string(port))) return;

}

Conn::~Conn(){
    delete addr;
    close(sock);
}

int Conn::getSock() const{
    return sock;
}
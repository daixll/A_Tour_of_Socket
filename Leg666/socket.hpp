#include "tool.hpp"
#include "socket_addr.hpp"
#include <sys/socket.h>
#include <unistd.h>

namespace jiao{

class Client{   // 客户端
public:
    Client();   
    ~Client();

private:

};


class Server{   // 服务端
public:
    Server(const char* ip="", int port=8080, bool is_v4=true):
        _addr(ip, port, is_v4){
        
        // 创建 socket
        _server = socket(
            is_v4 ? AF_INET : AF_INET6,
            SOCK_STREAM,
            0
        );
        // 端口复用
        int reuse = 1;
        setsockopt(reuse, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse, sizeof reuse);
        // 绑定地址
        err(
            bind(_server, _addr.get_addr(), _addr.get_addr_len()) == -1,
            "绑定服务端地址错误"
        );
        
    };  
    ~Server(){
        close(_server);
    }

    void run(const int backlog=10){
        _backlog = backlog;
        err(
            listen(_server, _backlog) == -1,
            "服务端运行错误"
        );

        // 等待连接
        

    };      // 服务器运行

private:
    int              _server;   // 服务器 socket
    jiao::SocketAddr _addr;     // 服务器地址
    int              _backlog;  // 最大连接数
    
};

}   // namespace jiao
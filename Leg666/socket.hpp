#include "tool.hpp"
#include "socket_addr.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>
#include <thread>

namespace jiao{

class TcpClient{   // 客户端
public:
    TcpClient();   
    ~TcpClient();

private:

};


class TcpServer{   // 服务端
public:
    TcpServer(const char* ip="127.0.0.1", int port=8080, bool is_v4=true):
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

        //struct sockaddr_in t_addr;
        //memset(&t_addr, 0, sizeof t_addr);
        //t_addr.sin_family      = AF_INET;
        //t_addr.sin_addr.s_addr = inet_addr(ip);
        //t_addr.sin_port        = htons(port);

        // 绑定地址
        err(
            //bind(_server, (struct sockaddr*)_addr.get_addr(), *_addr.get_addr_len()) == -1,
            bind(_server, _addr.get_addr(), sizeof *_addr.get_addr()) == -1,
            //bind(_server, (struct sockaddr*)&t_addr, sizeof *_addr.get_addr()) == -1,
            "绑定服务端地址错误"
        );

        // 创建 epoll
        _epoll = epoll_create(1);

        // 设置非阻塞模式
        this->set_nonblock(_server);

        // 添加事件 服务器 fd 上有可读事件发生时（有客户端连接）
        epoll_event ev;
        memset(&ev, 0, sizeof ev);
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = _server;
        err(
            epoll_ctl(_epoll, EPOLL_CTL_ADD, _server, &ev) == -1,
            "添加事件错误"
        );
        
    };  
    ~TcpServer(){
        close(_server);
    }

    void deal_func(void (*func)(int)){
        _func = func;
    };  // 处理函数

    void run(const int t){
        std::vector<epoll_event> events(100000);

        err(
            listen(_server, 100000) == -1,
            "服务端运行错误"
        );

        while(true){
            int n = epoll_wait(_epoll, events.data(), 100000, -1);
            err(
                n == -1,
                "epoll_wait 错误"
            );
            for(auto& i: events){
                if(i.data.fd == _server){
                    accept();
                }else if(i.events & EPOLLIN){
                    std::thread t(_func, (int)i.data.fd);
                    t.detach(); // 分离线程
                }
            }
        }

    };      // 服务器运行

private:
    int              _server;       // 服务器 socket
    jiao::SocketAddr _addr;         // 服务器地址
    int              _backlog;      // 最大连接数
    void             (*_func)(int); // 处理函数

    int              _epoll;        // epoll
    int              _max_events;   // 最大事件数

    void set_nonblock(int fd){
        int flags = fcntl(fd, F_GETFL, 0);
        err(
            fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1,
            "设置非阻塞模式错误"
        );
    };  // 设置非阻塞模式

    void accept(){
        jiao::SocketAddr client_addr;
        int client = ::accept(_server, client_addr.get_addr(), client_addr.get_addr_len());
        err(
            client == -1,
            "接收客户端连接错误"
        );

        this->set_nonblock(client);
        // 在非阻塞模式下，如果没有客户端连接，accept会返回-1，此时errno为EAGAIN或EWOULDBLOCK
        // 在阻塞模式下，如果没有客户端连接，accept会阻塞，直到有客户端连接

        epoll_event ev;
        memset(&ev, 0, sizeof ev);
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = client;

        std::cout << "客户端连接成功！"<< std::endl;
        std::cout << "客户端套接字: " << client << std::endl;
    };  // 接收客户端连接
};

}   // namespace jiao
#include "Headers/Sock.h"
#include "Headers/Epoll.h"

int main(int argc, char const *argv[]){
    jiao::Sock* server = new jiao::Sock("0.0.0.0", std::atoi(argv[1]));
    server -> Listen();
    server -> setNonBlock();
    
    jiao::Epoll* epoll = new jiao::Epoll();
    epoll -> Add(server);

    while(true)
    for(auto ev : epoll->Wait())
        if(ev.data.fd == server->fd){           // 有新连接
            jiao::Sock* client = server -> Ac();// 接受一个连接
            client -> setNonBlock();            // 设置非阻塞
            epoll -> Add(client, EPOLLOUT | EPOLLIN | EPOLLET);
        } 
        else if(ev.events & EPOLLIN){           // 可读事件
            jiao::Sock* client = new jiao::Sock();
            client -> fd = ev.data.fd;
            
            if(std::string msg = client -> Recv(); msg == "kill")
                epoll -> Del(client);
            else
                std::cout << msg << std::endl;
        }
        else if(ev.events & EPOLLOUT){
            std::cout << "可写 out 事件" << std::endl;
        }

    return 0;
}
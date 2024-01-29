#include "Headers/Sock.h"
#include "Headers/Epoll.h"

int main(int argc, char const *argv[]){
    jiao::Sock server("0.0.0.0", std::atoi(argv[1]));
    server.Listen();
    server.setNonBlock();
    jiao::Epoll epoll;
    epoll.Add(server.fd);
    epoll.Mod(server.fd, EPOLLIN | EPOLLET);

    while(true)
    for(auto ev : epoll.Wait()){
        std::cout << "有事件发生! " << ev.data.fd << ": ";

        if(ev.data.fd == server.fd){            // 有新连接
            std::cout << "新连接事件" << std::endl;
            jiao::Sock* client = server.Ac();   // 接受一个连接
            client->setNonBlock();              // 设置非阻塞
            epoll.Add(client->fd);              // 添加到 epoll 中
            epoll.Mod(client->fd, EPOLLOUT | EPOLLIN | EPOLLET); // 修改事件
        } 
        else if(ev.events & EPOLLIN){           // 可读事件
            std::cout << "可读 in 事件" << std::endl;
            jiao::Sock* cc = new jiao::Sock();
            cc -> fd = ev.data.fd;
            std::string msg = cc -> Recv();
            std::cout << msg << std::endl;
        }
        else if(ev.events & EPOLLOUT){
            std::cout << "可写 out 事件" << std::endl;
        }
    }

    return 0;
}
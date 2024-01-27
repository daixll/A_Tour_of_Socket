#include "Headers/Sock.h"
#include "Headers/Epoll.h"

int main(int argc, char const *argv[]){
    jiao::Sock server("0.0.0.0", std::atoi(argv[1]));
    server.Listen();
    server.setNonBlock();
    jiao::Epoll epoll;
    epoll.Add(server.fd);

    while(true)
    for(auto& ev : epoll.Wait()){
        if(ev.data.fd == server.fd){            // 有新连接
            jiao::Sock* client = server.Ac();   // 接受一个连接
            client->setNonBlock();              // 设置非阻塞
            epoll.Add(client->fd);              // 添加到 epoll 中
        } 
        else if(ev.events & EPOLLIN){           // 读事件
            jiao::Sock client;
            client.fd = ev.data.fd;

            std::string msg = client.Recv();
            std::cout << msg << std::endl;
        }
    }

    return 0;
}
#include "../Headers/Epoll.h"

using namespace jiao;

Epoll::Epoll(){
    fd = epoll_create1(0);
    err(fd == -1,
        "epoll 创建失败");
}

Epoll::~Epoll(){
    close(fd);
}

void Epoll::Add(const int& fd){
    epoll_event ev;
    ev.data.fd = fd;
    ev.events  = EPOLLIN | EPOLLET;  // 读事件 | 边缘触发

    err(epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &ev) == -1,
        "epoll 添加事件失败");
}

std::vector<epoll_event> Epoll::Wait(){
    events.resize(MAXEVENTS);
    int n = epoll_wait(fd, events.data(), MAXEVENTS, -1);
    err(n == -1,
        "epoll 等待事件失败");
    events.resize(n);
    return events;
}
#include "../Headers/Epoll.h"

using namespace jiao;

Epoll::Epoll(){
    epoll_fd = epoll_create1(0);
    err(epoll_fd == -1,
        "epoll 创建失败");
}

Epoll::~Epoll(){
    close(epoll_fd);
}

void Epoll::Add(Sock* sock, const int &new_event){
    epoll_event ev;
    ev.data.fd = sock->fd;
    ev.events  = new_event;

    err(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, sock->fd, &ev) == -1,
        "epoll 添加事件失败");
}

void Epoll::Mod(Sock* sock, const int& new_event){
    epoll_event ev;
    ev.data.fd = sock->fd;
    ev.events  = new_event;

    err(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sock->fd, &ev) == -1,
        "epoll 修改事件失败");
}

void Epoll::Del(Sock* sock){
    err(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock->fd, NULL) == -1,
        "epoll 删除事件失败");
}

std::vector<epoll_event> Epoll::Wait(){
    events.resize(MAXEVENTS);
    int n = epoll_wait(epoll_fd, events.data(), MAXEVENTS, -1);
    err(n == -1,
        "epoll 等待事件失败");
    events.resize(n);
    return events;
}
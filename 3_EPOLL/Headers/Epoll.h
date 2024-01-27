#pragma once

#include <unistd.h>
#include <sys/epoll.h>
#include <vector>
#include "Tool.h"

#define MAXEVENTS 1024

namespace jiao{

class Epoll
{
public:
    Epoll();
    ~Epoll();

    // 添加事件
    void Add(const int& fd);

    // 获取事件
    std::vector<epoll_event> Wait();
private:
    int fd;                         // epoll 也是有一个文件描述符的
    std::vector<epoll_event> events;// 事件数组
};

}   // namespace jiao
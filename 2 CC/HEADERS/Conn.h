#pragma once 

#include "Tool.h"
#include "SockAddr.h"
#include <fcntl.h>
#include <unistd.h>

namespace jiao{

class Conn
{
public:
    Conn(const std::string& ip, const int& port);
    ~Conn();
    int getSock() const;
private:
    int sock;
    SockAddr* addr;
};

}   // namespace jiao
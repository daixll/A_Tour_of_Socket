#pragma once
#include "tool.hpp"
#include <arpa/inet.h>
#include <cstring>

namespace jiao{

class SocketAddr{   // socket 地址
public:
    SocketAddr(const char* ip="127.0.0.1", int port=8080, bool is_v4=true){
        if(is_v4){
            _is_v4 = true;
            memset(&_addr_v4, 0, sizeof _addr_v4);
            _addr_v4.sin_family      = AF_INET;
            _addr_v4.sin_addr.s_addr = inet_addr(ip);
            _addr_v4.sin_port        = htons(port);
        }else{
            _is_v4 = false;
            memset(&_addr_v6, 0, sizeof _addr_v6);
            _addr_v6.sin6_family      = AF_INET6;
            if(strlen(ip) == 0){    // 绑定本地ip
                _addr_v6.sin6_addr    = in6addr_any;
            }else{                  // 绑定指定ip
                inet_pton(AF_INET6, ip, &_addr_v6.sin6_addr);
            }
            _addr_v6.sin6_port        = htons(port);
        }  
    }
    ~SocketAddr(){}

    struct sockaddr* get_addr(){
        return _is_v4 ? (struct sockaddr*)&_addr_v4 : (struct sockaddr*)&_addr_v6;
    }

    socklen_t* get_addr_len(){
        return &_addr_len;
    }
    

private:
    bool         _is_v4;    // 是否是 IPv4 地址
    struct sockaddr_in  _addr_v4;  // IPv4 地址
    struct sockaddr_in6 _addr_v6;  // IPv6 地址
    socklen_t    _addr_len; // 地址长度
};
}   // namespace jiao
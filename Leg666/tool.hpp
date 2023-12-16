#pragma once
#include <iostream>

namespace jiao{

bool war(bool flg, std::string msg){
    if(!flg) return 0;  // 如果没有警告，返回 0
    std::cerr << msg << "\n警告代码：" << errno << std::endl;
    return 1;
}

void err(bool flg, std::string msg){
    if(!flg) return ;   // 如果没有错误，直接返回
    std::cerr << msg << "\n错误代码：" << errno << std::endl;
    exit(1);
}

} // namespace jiao
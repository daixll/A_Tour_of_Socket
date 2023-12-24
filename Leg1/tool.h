#pragma once
#include <iostream>
#include <string>
#include <ctime>

void err(bool flg, std::string msg){
    if(!flg) return ;   // 如果没有错误，直接返回
    std::cerr << "\n" << msg << "\n错误代码：" << errno << std::endl;
    exit(1);
}

bool war(bool flg, std::string msg){
    if(!flg) return 0;  // 如果没有警告，返回 0
    std::cerr << "\n" << msg << "\n警告代码：" << errno << std::endl;
    return 1;
}

std::string getTime() {
    // 获取当前时间的秒数
    std::time_t currentTime;
    std::time(&currentTime);

    // 将秒数转换为本地时间结构
    std::tm* localTime = std::localtime(&currentTime);

    // 使用std::strftime将时间格式化为字符串
    char timeString[50];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    // 返回格式化后的时间字符串
    return timeString;
}
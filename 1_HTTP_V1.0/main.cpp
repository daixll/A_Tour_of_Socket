#include "HEADERS/server.h"
#include <functional>
#include <fstream>
#include <sstream>

std::string deal(std::string s){
    // GET / HTTP/1.1 123 123
    // ...
    // 只取 / 的部分
    int a = s.find(' ');
    int b = s.find(' ', a+1);
    std::string path = s.substr(a+2, b-a-2);
    if(path == "") path = "index.html";
    
    std::string res = "HTTP/1.1 200 OK\r\n\r\n";
    std::ifstream ifs("src/" + path);
    if(!ifs.is_open()){
        res = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
        return res;
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::cout << path << std::endl;
    return res + ss.str();
}

int main(int argc, char const *argv[]){
    jiao::HTTPServer* web = new jiao::HTTPServer("0.0.0.0", atoi(argv[1]));
    web->run(deal);
    delete web;
    return 0;
}
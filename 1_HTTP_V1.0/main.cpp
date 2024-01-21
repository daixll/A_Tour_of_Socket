#include "HEADERS/server.h"
#include <functional>

std::string deal(std::string s){
    return "";
}

int main(int argc, char const *argv[]){
    jiao::HTTPServer* web = new jiao::HTTPServer("0.0.0.0", atoi(argv[1]));
    web->run(deal);
    delete web;
    return 0;
}
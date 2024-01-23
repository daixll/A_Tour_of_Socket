#include "../HEADERS/Event.h"
#include "../HEADERS/Conn.h"

using namespace jiao;

Event::Event(const std::string& ip, const int& port){
    addr = new SockAddr(ip, port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    err(sock == -1, "创建套接字错误");

    int ret = bind(sock, (sockaddr*)addr->get_addr(), *addr->get_addr_len());
    err(ret == -1, "绑定套接字错误");

    ret = listen(sock, 5);
    err(ret == -1, "监听套接字错误");

    fcntl(sock, F_SETFL, O_NONBLOCK);
}

Event::~Event(){
    delete addr;
    close(sock);
}

std::string Event::recvMsg(const int& client){
    if(cs.count(client) == 0) return "";

    memset(buf, '\0', sizeof buf);
    int len = recv(client, buf, sizeof buf, 0);
    
    if(len == -1) {
        //war(true, "接收数据错误");
        return "";
    }
    else if(len == 0) {
        if (war(errno != 11, "接收时 客户端断开连接: " + std::to_string(client))){
            close(client);
            return "kill";
        }
    }
    
    std::string tmp(buf);
    if(tmp.size()) std::cout << "接收到" << client << "的消息: " << tmp << std::endl;
    return buf;
}

int Event::sendMsg(const int& client, const std::string& msg){
    if(cs.count(client) == 0) return 0;
    if(msg == "") return -1;

    int len = send(client, msg.c_str(), msg.size(), 0);
    
    if(len == -1) {
        //war(true, "发送数据错误");
        return -1;
    }
    else if(len == 0) {
        war(true, "发送时 客户端断开连接: " + std::to_string(client));
        close(client);
        return 0;
    }

    return 1;
}

void Event::loop(std::function<std::string(std::string)> deal){
    acer = new Accepter(sock);

    std::ios::sync_with_stdio(false);
    std::string s;
    while(true){
        // 接收输入的数据，主动连接或者发送数据
        if(std::cin.rdbuf() -> in_avail() > 0){ // 如果输入缓冲区有数据
            std::getline(std::cin, s);          // 读取数据

            if(std::string flg = s.substr(0, 4); flg == "quit"){
                std::cout << "下号 ..." << std::endl;
                break;
            } else if (flg == "kill"){
                std::string target = s.substr(5);
                std::cout << "删除 " << target << " ..." << std::endl;
                close(std::stoi(target));
                cs.erase(std::stoi(target));
            } else if (flg == "find"){
                std::cout << "当前已经建立连接 " << std::endl;
                for(const auto& c : cs)
                    std::cout << c << std::endl;
            } else if (flg == "conn"){
                std::string ip = s.substr(5, s.find(" ", 5) - 5);
                int port = std::stoi(s.substr(s.find(" ", 5) + 1));
                
                jiao::Conn conn(ip, port);
                int cc = conn.getSock();
                if(cc == -1){
                    std::cout << "连接 " << ip << ":" << port << " 失败！" << std::endl;
                    continue;
                }
                std::cout << "连接 " << cc << " 成功！" << std::endl;
                cs.insert(cc);
            } else if (flg == "send"){
                std::string target = s.substr(5, s.find(" ", 5) - 5);
                std::string msg = s.substr(s.find(" ", 5) + 1);

                if(sendMsg(std::stoi(target), msg))
                    std::cout << "发送给 " << target << " 成功！" << std::endl;
            } else {
                std::cout << "指令错误！" << std::endl;

            }
        }

        // 持续接收客户端连接
        int client = acer -> ac();
        if(client != -1) {
            fcntl(client, F_SETFL, O_NONBLOCK);
            cs.insert(client);
            log("客户端建立连接: " + std::to_string(client));
        }

        // 持续接收客户端消息
        std::set<int> down;
        for(const auto c: cs){
            std::string msg = recvMsg(c);
            if(msg == "") continue;

            if(msg == "kill")
                down.insert(c);
            else
                std::cout << c << ": " + msg << std::endl;
        }

        for(const auto c: down) cs.erase(c);
    }
}
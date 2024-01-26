#include "Headers/Sock.h"

int main(int argc, char const *argv[]){
    jiao::Sock Server("0.0.0.0", std::atoi(argv[1]));
    Server.Listen();                        // 监听

    while(true){
        jiao::Sock Client = Server.Ac();    // 接受连接
        
        std::cout << "连接成功! Socket: " << Client.fd << std::endl;
        while(true){
            std::string msg = Client.Recv();// 接收
            std::cout << msg << std::endl;
            Client.Send(msg);               // 发送
        }
    }

    return 0;
}
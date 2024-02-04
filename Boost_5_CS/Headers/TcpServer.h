#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <cstring>

using namespace boost::asio;

class Connection{   // 连接
public:
    Connection(io_context& io): socket(io){};
    ~Connection(){
        std::cout << "连接释放!" << std::endl;
    };

    ip::tcp::socket socket;

    void start_read(){
        char data[1024];
        // 异步读取数据
        socket.async_read_some(buffer(data, sizeof data),
            [&, this](const boost::system::error_code& ec, std::size_t len){
                if(!ec){
                    data[len] = '\0';
                    std::cout << "收到消息: " << data << std::endl;
                    start_read();
                }else{
                    std::cout << "读取错误! " << ec.message() << " " << error::operation_aborted << std::endl;
                    delete this;
                }
            });
    }

    void start_write(){
        std::string s;
        // 异步发送数据
        socket.async_write_some(buffer(s),
            [this](const boost::system::error_code& ec, std::size_t len){
                if(!ec){
                    start_write();
                }
                else{
                    std::cout << "发送错误! " << ec.message() << " " << error::operation_aborted << std::endl;
                    delete this;
                }
            });
    }

    char data[1024];
};

class TcpServer{    // 服务器
public:
    TcpServer(io_context io, ip::tcp::endpoint ep):
        _io(io), _ac(io, ep){
        start();
        _io.run();
    };
    ~TcpServer(){};
private:
    void start(){
        // 创建一个新的连接
        Connection* new_conn = new Connection(_io);

        // 异步接收连接
        _ac.async_accept(new_conn->socket,
            [this, new_conn](const boost::system::error_code& ec){
                if(!ec){
                    std::cout << "新连接 " << new_conn->socket.remote_endpoint().address() << ":" << new_conn->socket.remote_endpoint().port() << std::endl;
                    new_conn->start_read(), new_conn->start_write();  
                }
            // 重新开始接收连接
            start();
        });
    }

    io_context&         _io;
    ip::tcp::acceptor   _ac;
};
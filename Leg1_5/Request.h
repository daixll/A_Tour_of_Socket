#include <string>
#include <map>

class Request{
public:
    Request(const char* buf){
        int idx=0;
        while(buf[idx]!=' ' && buf[idx]!='\0') _method += buf[idx++]; // 1. 获取请求方法
        ++idx;
        while(buf[idx]!=' ' && buf[idx]!='\0') _path   += buf[idx++]; // 2. 获取请求路径
        ++idx;
        while(buf[idx]!='\n' && buf[idx]!='\0') _vhttp  += buf[idx++];// 3. 获取HTTP版本
        ++idx;
        if(_path=="/") _path = "/index.html"; // 默认请求路径为 index.html

        // 获取请求头
        while(buf[idx]!='\n' && buf[idx+1]!='\n'){
            std::string key, value;
            while(buf[idx]!=':' && buf[idx]!='\0') key += buf[idx++];   // 1. 获取请求头的键
            idx += 2;
            while(buf[idx]!='\n' && buf[idx]!='\0') value += buf[idx++];// 2. 获取请求头的值
            ++idx;
            _headers[key] = value;
        }

        // 获取请求体
        // todo
    }

    std::string line_method() const{    // 请求方法
        return _method;
    };    
    std::string line_path() const{      // 请求路径
        return _path;
    }
    std::string line_vhttp() const{     // HTTP版本
        return _vhttp;
    }
    std::string header(const std::string& key) const{ // 请求头
        return _headers.at(key);
    }

private:
    std::string _method, _path, _vhttp; // 请求行的三个参数
    std::map<std::string, std::string> _headers; // 请求头

};
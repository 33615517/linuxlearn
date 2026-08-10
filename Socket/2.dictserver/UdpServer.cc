#include<iostream>
#include<memory>
#include"UdpServer.hpp"
#include"Dict.hpp"
#include"UdpServer.hpp"
std::string defaulthandler(const std::string& msg)
{
    std::string result = "hello,";
    result += msg;
    return result;
}

//需求
//1.翻译系统，字符串当成英文单词，翻译为汉语
//2、基于文件来做

// ./udpserver  port
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " ip port" << std::endl;
        return 1;
    }
    //std::string ip = argv[1];
    uint16_t port = static_cast<uint16_t>(std::stoi(argv[1]));
    Enable_Console_log_strategy();

    //1.字典对象提供翻译功能
    Dict dict;
    dict.LoadDict();


    //2.网络服务器对象，提供通信功能



    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port,[&dict](const std::string& msg, InetAddr& client)->std::string{
        return dict.Translate(msg, client);
    });
    usvr->Init();
    usvr->Start();
    usvr->Stop();
    return 0;
}
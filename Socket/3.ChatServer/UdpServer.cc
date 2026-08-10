#include<iostream>
#include<memory>
#include"UdpServer.hpp"
#include"Route.hpp"
#include"UdpServer.hpp"
#include"Thread.hpp"
#include"ThreadPool.hpp"


using namespace ThreadPoolModule;



std::string defaulthandler(const std::string& msg)
{
    std::string result = "hello,";
    result += msg;
    return result;
}

//需求
//1.翻译系统，字符串当成英文单词，翻译为汉语
//2、基于文件来做

using task_t = std::function<void()>;



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
 
    //1.路由服务
    Route r;

    //2.线程池
    auto tp = ThreadPool<task_t>::GetInstance();

    //3.网络服务器对象，提供通信功能
    
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port,[&r,&tp](int sockfd, const std::string& message, InetAddr& peer){
        task_t t = std::bind(&Route::MessageRoute,&r,sockfd,message,peer);
        tp->Equeue(t);
    });



    // std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port,[&r](int sockfd, const std::string& message, InetAddr& peer){
    //     r.MessageRoute(sockfd, message, peer);
    // });

    usvr->Init();
    usvr->Start();
    usvr->Stop();
    return 0;
}
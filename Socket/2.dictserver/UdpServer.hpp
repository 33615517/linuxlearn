#pragma once
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include"Log.hpp"
#include<unistd.h>
#include<string>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<functional>
#include <strings.h>
#include"InetAddr.hpp"
using namespace LogModule;

using func_t = std::function<const std::string(const std::string&,InetAddr&)>; //回调函数类型，用户自定义的处理逻辑
const int defultfd = -1;


//为了网络通信
class UdpServer
{
public:
    UdpServer( uint16_t port,func_t func):_sockfd(defultfd), _port(port), _isrunning(false), _func(func)
    {

    }
    void Init()
    {
        //1.创建套接字
        _sockfd = socket(AF_INET,SOCK_DGRAM,0);
        if(_sockfd == defultfd)
        {
            LOG(Loglevel::FATAL) << "创建套接字失败";
            exit(1);
        }
        LOG(Loglevel::INFO) << "创建套接字成功,sockfd:" << _sockfd;

        //2.绑定socket套接字信息，ip地址，端口号
        //2.1 填充sockaddr_in结构体
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        //local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_addr.s_addr = INADDR_ANY; //监听本机所有网卡
        //为什么服务器端要显示bind？因为服务器端要明确告诉操作系统，自己要监听的IP地址和端口号，客户端不需要显示bind，客户端的IP地址和端口号是随机的，操作系统会自动分配
        //2.2 绑定套接字
        int n = bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n == defultfd)
        {
            LOG(Loglevel::FATAL) << "绑定套接字失败";
            exit(2);    
        }
        LOG(Loglevel::INFO) << "绑定套接字成功" << ",port:" << _port;



    }
    void Start()
    {
        _isrunning = true;
        while(_isrunning)
        {
            //1.接收消息
            char _buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t s = recvfrom(_sockfd, _buffer, sizeof(_buffer)-1, 0, (struct sockaddr*)&peer, &len);
            if(s > 0)
            {
                InetAddr client(peer);
                //int peer_port = ntohs(peer.sin_port);
                //std::string peer_ip = inet_ntoa(peer.sin_addr);
                _buffer[s] = 0;
                std::string result = _func(_buffer,client);
               // LOG(Loglevel::DEBUG) << "[" << peer_ip << ":" << peer_port << "] " << _buffer << " -> " << result;
                //2.发送消息
                // std::string sendmsg = "server echo: ";
                // sendmsg += _buffer;
                sendto(_sockfd, result.c_str(), result.size(), 0, (struct sockaddr*)&peer, len);
            }
            

        }

    }
    void Stop()
    {
        _isrunning = false;

    }
    ~UdpServer()
    {
        
    }

private:
    int _sockfd;
    uint16_t _port;
   // std::string _ip; //?????
    bool _isrunning;
    func_t _func; //回调函数，用户自定义的处理逻辑

};
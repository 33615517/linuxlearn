#pragma once
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<functional>

//网络地址和主机地址之间进行转换的类

class InetAddr
{
public:
    InetAddr(struct sockaddr_in &addr):_addr(addr)
    {
        _port = ntohs(_addr.sin_port);
        _ip = inet_ntoa(_addr.sin_addr);
    }
    InetAddr(const std::string &ip,uint16_t port):_ip(ip),_port(port)
    {
        memset(&_addr,0,sizeof(_addr));
        inet_pton(AF_INET,ip.c_str(),&_addr.sin_addr);
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(port);
    }
    struct sockaddr_in& NetAddr()
    {
        return _addr;
    }
    bool operator==(const InetAddr& other) const
    {
        return (_port == other._port) && (_ip == other._ip);
    }
    std::string StringAddr()
    {
        return _ip + ":" + std::to_string(_port);
    }
    ~InetAddr()
    {

    }
    uint16_t Port()
    {
        return _port;
    }
    std::string Ip()
    {
        return _ip;
    }

private:
    struct sockaddr_in _addr;
    uint16_t _port;
    std::string _ip;
};
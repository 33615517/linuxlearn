#pragma once
#include "Common.hpp"
// 网络地址和主机地址之间进行转换的类

class InetAddr
{
public:
    InetAddr() {}
    InetAddr(struct sockaddr_in &addr)
    {
        SetAddr(addr);
    }
    InetAddr(const std::string &ip, uint16_t port) : _ip(ip), _port(port)
    {
        // 主机转网络
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        inet_pton(AF_INET, _ip.c_str(), &_addr.sin_addr);
        _addr.sin_port = htons(_port);
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // TODO
    }
    InetAddr(uint16_t port) : _port(port), _ip()
    {
        // 主机转网络
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = INADDR_ANY;
        _addr.sin_port = htons(_port);
    }
    void SetAddr(struct sockaddr_in &addr)
    {
        _addr = addr;
        // 网络转主机
        _port = ntohs(_addr.sin_port); // 从网络中拿到的！网络序列
        // _ip = inet_ntoa(_addr.sin_addr); // 4字节网络风格的IP -> 点分十进制的字符串风格的IP
        char ipbuffer[64];
        inet_ntop(AF_INET, &_addr.sin_addr, ipbuffer, sizeof(_addr));
        _ip = ipbuffer;
    }
    uint16_t Port() { return _port; }
    std::string Ip() { return _ip; }
    const struct sockaddr_in &NetAddr() { return _addr; }
    const struct sockaddr *NetAddrPtr()
    {
        return CONV(_addr);
    }
    socklen_t NetAddrLen()
    {
        return sizeof(_addr);
    }
    bool operator==(const InetAddr &addr)
    {
        return addr._ip == _ip && addr._port == _port;
    }
    std::string StringAddr()
    {
        return _ip + ":" + std::to_string(_port);
    }
    ~InetAddr()
    {
    }

private:
    struct sockaddr_in _addr;
    std::string _ip;
    uint16_t _port;
};
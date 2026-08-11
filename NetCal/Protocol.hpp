#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Socket.hpp"

// 实现一个自定义的网络版本的计算器

using namespace SocketModule;

// 约定好各个字段的含义，本质就是约定好协议！
// client -> server
// 如何要做序列化和反序列化：
// 1. 我们自己写(怎么做) ---> 往往不具备很好的扩展性
// 2. 使用现成的方案(这个是我们要写的) ---> json -> jsoncpp
class Request
{
public:
    Request()
    {
    }
    Request(int x, int y, char oper) : _x(x), _y(y), _oper(oper)
    {
    }
    std::string Serialize()
    {
        // _x = 10 _y = 20, _oper = '+'
        // "10" "20" '+' : 用空格作为分隔符
    }
    bool Deserialize(std::string &in)
    {
        // "10" "20" '+' -> 以空格作为分隔符 -> 10 20 '+'
    }
    ~Request() {}

private:
    int _x;
    int _y;
    char _oper; // + - * / % -> _x _oper _y -> 10 + 20
};

// server -> client
class Response
{
public:
    Response() {}
    Response(int result, int code) : _result(result), _code(code)
    {
    }
    std::string Serialize()
    {
    }
    bool Deserialize(std::string &in)
    {
    }
    ~Response() {}

private:
    int _result; // 运算结果，无法区分清楚应答是计算结果，还是异常值
    int _code;   // 0:sucess, 1,2,3,4->不同的运算异常的情况
};

// 协议(基于TCP的)需要解决两个问题：
// 1. request和response必须得有序列化和反序列化功能
// 2. 你必须保证，读取的时候，读到完整的请求(TCP, UDP不用考虑)
class Protocol
{
public:
    Protocol()
    {}
    void GetRequest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {

    }
    ~Protocol()
    {}
private:
    // 因为我们用的是多进程
    // Request _req;
    // Response _resp;
};
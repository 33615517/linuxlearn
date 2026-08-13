#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Socket.hpp"
#include <jsoncpp/json/json.h>
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
        Json::Value root;
        root["x"] = _x;
        root["y"] = _y;
        root["oper"] = _oper;

        Json::FastWriter writer;
        std::string s = writer.write(root);

        return s;
    }
    bool Deserialize(std::string &in)
    {
        // "10" "20" '+' -> 以空格作为分隔符 -> 10 20 '+'

        Json::Value root;
        Json::Reader reader;
        bool ok = reader.parse(in, root);
        if (!ok)
        {
            return false;
        }
        _x = root["x"].asInt();
        _y = root["y"].asInt();
        _oper = root["oper"].asInt();

        return true;
    }
    ~Request() {}
    int X() { return _x; }
    int Y() { return _y; }
    char Oper() { return _oper; }
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
        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;

        Json::FastWriter writer;
        std::string s = writer.write(root);
        return s;
    }
    bool Deserialize(std::string &in)
    {
        Json::Value root;
        Json::Reader reader;
        bool ok = reader.parse(in, root);
        if (!ok)
        {
            return false;
        }
        _result = root["result"].asInt();
        _code = root["code"].asInt();

        return true;
    }
    ~Response() {}
    void SetResult(int result) { _result = result; }
    void SetCode(int code) { _code = code; }
private:
    int _result; // 运算结果，无法区分清楚应答是计算结果，还是异常值
    int _code;   // 0:sucess, 1,2,3,4->不同的运算异常的情况
};

using func_t = std::function<Response( Request &)>;
// 协议(基于TCP的)需要解决两个问题：
// 1. request和response必须得有序列化和反序列化功能
// 2. 你必须保证，读取的时候，读到完整的请求(TCP, UDP不用考虑)
class Protocol
{
public:
    Protocol(func_t func) : _func(func)
    {
    }
    std::string Encode(const std::string jsonstr)
    {
        std::string len = std::to_string(jsonstr.size());
        std::string package = len + sep + jsonstr + sep;
        return package;
    }
    // 1.判断报文完整性
    // 2.如果至少包含一个完整请求，提取他，并从缓冲区移除，方便处理下一个
    bool Decode(std::string &buffer, std::string *package)
    {
        ssize_t pos = buffer.find(sep);
        if (pos == std::string::npos)
        {
            return false; // 让调用方继续读取数据
        }
        std::string package_len_str = buffer.substr(0, pos);
        int package_len_int = std::stoi(package_len_str);
        // buffer一定有长度，但是一定要有一定的报文吗？
        int target_len = package_len_str.size() + package_len_int + sep.size() * 2;
        if (buffer.size() < target_len)
        {
            return false;
        }
        // buffer一定至少有一个完整的报文
        *package = buffer.substr(pos + sep.size(), package_len_int);
        buffer.erase(0, target_len);
        return true;
    }
    void GetRequest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {
        // 读取了数据，放到缓冲区中
        std::string inbuffer;
        while (true)
        {
            int n = sock->Recv(&inbuffer);
            if (n > 0)
            {
                // 1.判断报文完整性
                std::string package;
                bool ret = Decode(inbuffer, &package);
                if(ret)
                {
                    //一定拿到了完整的报文
                    // 2.反序列化
                    Request req;
                    bool ok = req.Deserialize(package);
                    if (!ok)
                    {
                        //我一定拿到了完整的报文，但是反序列化失败了，说明客户端发来的数据有问题
                        LOG(Loglevel::WARNING) << "client:" << client.StringAddr() << " request error ...";
                        continue;
                    }
                    else
                    {
                       
                        Response resp = _func(req);
                        //序列化
                        std::string json_str = resp.Serialize();
                        //添加自定义长度
                        std::string send_str = Encode(json_str);
                        //发送
                        sock->Send(send_str);
                    }
                }
            }
            else if (n == 0)
            {
                LOG(Loglevel::INFO) << "client:" << client.StringAddr() << " closed ...";
                break;
            }
            else
            {
                LOG(Loglevel::WARNING) << "client:" << client.StringAddr() << " recv error ...";
                break;
            }
        }
    }
    ~Protocol()
    {
    }

private:
    // 因为我们用的是多进程
    // Request _req;
    // Response _resp;
    func_t _func; // 计算函数，用户自定义的计算函数
};
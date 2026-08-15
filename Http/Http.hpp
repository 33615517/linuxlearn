#pragma once
#include "Socket.hpp"
#include "TcpServer.hpp"
#include <memory>
#include <iostream>
#include <unordered_map>
#include "Util.hpp"
#include "Log.hpp"
#include <sstream>
using namespace SocketModule;
using namespace LogModule;
const std::string gspace = " ";
const std::string gglinespace = "\r\n";
const std::string glinesep = ": ";
const std::string wberoot = "./wwwroot";
const std::string homepage = "/index.html";
class HttpRequest
{
public:
    HttpRequest()
    {
    }
    std::string Serialize()
    {
        return "";
    }
    void ParseReqLine(std::string &reqline)
    {
        std::istringstream ss(reqline);
        ss >> _method >> _uri >> _version;
    }
    bool Deserialize(std::string &request)
    {
        // 1.提取请求行
        std::string reqline;
        bool res = Util::ReadOneLine(request, &reqline, gglinespace);
        LOG(Loglevel::DEBUG) << "request line:" << reqline;
        //std::cout << "\n";
        //std::cout << request;
        // 2.对请求行进行反序列化
        ParseReqLine(reqline);
        LOG(Loglevel::DEBUG) << "method:" << _method;
        LOG(Loglevel::DEBUG) << " uri:" << _uri;
        LOG(Loglevel::DEBUG) << " version:" << _version;
        if(_uri == "/")
        {
            _uri = wberoot + _uri + homepage;
        }
        else
        {
            _uri = wberoot + _uri;
        }
        return true;
    }
    void ParseRequest()
    {
    }
    std::string Uri()
    {
        return _uri;
    }

private:
    std::string _method;
    std::string _uri;
    std::string _version;

    std::unordered_map<std::string, std::string> _header;
    std::string _blankline;
    std::string _text;
};
class HttpResponse
{
public:
    HttpResponse() : _blankline(gglinespace)
    {
    }
    ~HttpResponse()
    {
    }
    std::string Serialize()
    {
        std::string status_line = _version + gspace + std::to_string(_code) + gspace + _desc + gglinespace;
        std::string headers;
        for (const auto &header : _headers)
        {
            headers += header.first + glinesep + header.second + gglinespace;
        }
        return status_line + headers + _blankline + _text;
    }
    void SetTargetFile(const std::string &filename)
    {
        _targetfile = filename;
    }
    bool MakeResponse()
    {
        if (_targetfile.empty())
        {
            _code = 404;
            _desc = "Not Found";
            _text = "<html><body><h1>404 Not Found</h1></body></html>";
            return false;
        }
        else
        {
            bool res = Util::ReadFileContent(_targetfile, &_text);
            if (!res)
            {
                _code = 404;
                _desc = "Not Found";
                _text = "<html><body><h1>404 Not Found</h1></body></html>";
                return false;
            }
            else
            {
                _code = 200;
                _desc = "OK";
                return true;
            }
        }
    }
    bool Deserialize()
    {
        return true;
    }

    // private:
public:
    std::string _version;
    int _code;         // 404
    std::string _desc; // Not Found
    std::unordered_map<std::string, std::string> _headers;
    std::string _blankline;
    std::string _text;

    //其他属性
    std::string _targetfile;
};
class Http
{
public:
    Http(uint16_t port) : _tsvrp(std::make_unique<TcpServer>(port))
    {
    }
    void HandlerHttpRquest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {
        // 收到请求
        std::string httprequest;
        // 假设：概率大读到完整的请求
        int n = sock->Recv(&httprequest);
        if (n > 0)
        {
            HttpRequest req;
            req.Deserialize(httprequest);
            HttpResponse resp;
            resp.SetTargetFile(req.Uri());
            resp.MakeResponse();
            //std::string filename = req.Uri();

            HttpResponse resp;
            resp._version = "HTTP/1.1";
            resp._code = 200;
            resp._desc = "OK";
            LOG(Loglevel::DEBUG) << "用户请求：" << filename;
            bool res = Util::ReadFileContent(filename, &resp._text);
            std::string response_str = resp.Serialize();
            sock->Send(response_str);
        }

// 处理HTTP请求
// #ifndef DEBUG
// #define DEBUG
#ifdef DEBUG
        // 收到请求
        std::string httprequest;
        // 假设：概率大读到完整的请求
        sock->Recv(&httprequest);

        std::cout << httprequest << std::endl;

        HttpResponse resp;

        resp._version = "HTTP/1.1";
        resp._code = 200;
        resp._desc = "OK";
        std::string filename = wberoot + homepage;
        bool res = Util::ReadFileContent(filename, &resp._text);
        std::string response_str = resp.Serialize();
        sock->Send(response_str);
#endif
    }
    void Start()
    {
        _tsvrp->Start([this](std::shared_ptr<Socket> &sock, InetAddr &client)
                      { this->HandlerHttpRquest(sock, client); });
    }
    ~Http()
    {
    }

private:
    std::unique_ptr<TcpServer> _tsvrp;
};

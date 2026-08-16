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

// HTTP协议中常用的一些分隔符
const std::string gspace = " ";         // 空格，用于分隔状态行中的字段
const std::string gglinespace = "\r\n"; // HTTP协议规定的换行符
const std::string glinesep = ": ";      // HTTP请求头/响应头中 key 和 value 的分隔符

// Web服务器的资源根目录
const std::string wberoot = "./wwwroot";

// 默认首页
const std::string homepage = "/index.html";
// 404页面
const std::string page_404 = "/404.html";

// HttpRequest：描述并解析一个HTTP请求
// 主要负责：
// 1. 保存请求方法、URI、HTTP版本
// 2. 对浏览器发送过来的HTTP请求字符串进行反序列化
// 3. 将URI转换成本地静态资源路径
class HttpRequest
{
public:
    HttpRequest()
    {
    }

    // 将HttpRequest对象重新序列化成HTTP请求字符串
    // 当前暂时没有实现
    std::string Serialize()
    {
        return "";
    }

    // 解析HTTP请求行
    // 例如：
    // GET /index.html HTTP/1.1
    //
    // 最终解析为：
    // _method  = "GET"
    // _uri     = "/index.html"
    // _version = "HTTP/1.1"
    void ParseReqLine(std::string &reqline)
    {
        std::istringstream ss(reqline);
        ss >> _method >> _uri >> _version;
    }

    // 对收到的完整HTTP请求进行反序列化
    // 即：把HTTP请求字符串解析成HttpRequest对象中的各个成员
    bool Deserialize(std::string &request)
    {
        // 1.提取请求行
        // 从HTTP请求中读取第一行，例如：
        // GET /index.html HTTP/1.1
        std::string reqline;
        bool res = Util::ReadOneLine(request, &reqline, gglinespace);

        LOG(Loglevel::DEBUG) << "request line:" << reqline;

        // std::cout << "\n";
        // std::cout << request;

        // 2.对请求行进行反序列化
        // 将请求方法、URI、HTTP版本分别保存起来
        ParseReqLine(reqline);

        LOG(Loglevel::DEBUG) << "method:" << _method;
        LOG(Loglevel::DEBUG) << " uri:" << _uri;
        LOG(Loglevel::DEBUG) << " version:" << _version;

        // 如果用户访问的是根目录 /
        // 则自动访问默认首页 ./wwwroot/index.html
        if (_uri == "/")
        {
            _uri = wberoot + homepage;
        }
        else
        {
            // 如果访问的是其他资源
            // 例如 /login.html
            // 则转换成服务器本地文件路径：
            // ./wwwroot/login.html
            _uri = wberoot + _uri;
        }

        return true;
    }

    // 用来进一步解析HTTP请求
    // 例如请求头、正文等
    // 当前暂时没有实现
    void ParseRequest()
    {
    }

    // 获取客户端请求资源对应的本地文件路径
    std::string Uri()
    {
        return _uri;
    }

private:
    // HTTP请求行中的三个核心字段

    std::string _method;  // 请求方法，例如 GET、POST
    std::string _uri;     // 请求资源路径，例如 /index.html
    std::string _version; // HTTP版本，例如 HTTP/1.1

    // HTTP请求头
    // 例如：
    // Host: 127.0.0.1:8080
    // User-Agent: Chrome
    std::unordered_map<std::string, std::string> _header;

    // HTTP请求中的空行
    // 用于分隔请求头和请求正文
    std::string _blankline;

    // HTTP请求正文
    // POST等请求可能携带数据
    std::string _text;
};

// HttpResponse：描述一个HTTP响应
// 主要负责：
// 1. 根据客户端请求找到目标文件
// 2. 读取文件内容
// 3. 设置HTTP状态码
// 4. 将响应对象序列化成HTTP响应字符串
class HttpResponse
{
public:
    // HTTP头和正文之间需要有一个空行
    // 因此初始化为 "\r\n"
    HttpResponse() : _blankline(gglinespace),
                     _version("HTTP/1.1")
    {
    }

    ~HttpResponse()
    {
    }

    // 将HttpResponse对象序列化成完整HTTP响应字符串
    //
    // 最终格式类似：
    //
    // HTTP/1.1 200 OK\r\n
    // Content-Type: text/html\r\n
    // Content-Length: xxx\r\n
    // \r\n
    // <html>...</html>
    std::string Serialize()
    {
        // 构建HTTP状态行
        // 例如：
        // HTTP/1.1 200 OK\r\n
        std::string status_line = _version + gspace + std::to_string(_code) + gspace + _desc + gglinespace;

        // 构建响应头
        std::string headers;

        for (const auto &header : _headers)
        {
            // 每一个响应头格式：
            // key: value\r\n
            headers += header.first + glinesep + header.second + gglinespace;
        }

        // 状态行 + 响应头 + 空行 + 响应正文
        return status_line + headers + _blankline + _text;
    }

    // 设置客户端请求对应的服务器本地文件
    // 例如：
    // ./wwwroot/index.html
    void SetTargetFile(const std::string &filename)
    {
        _targetfile = filename;
    }

    void SetCode(int code)
    {
        _code = code;
        switch (code)
        {
        case 200:
            _desc = "OK";
            break;
        case 404:
            _desc = "Not Found";
            break;
        case 302:
            _desc = "Found";
            break;
        case 301:
            _desc = "Moved Permanently";
            break;
        default:
            _desc = "Unknown";
            break;
        }
    }
    // 根据客户端请求的目标文件构建HTTP响应
    //
    // 文件存在：
    // 200 OK
    //
    // 文件不存在：
    // 404 Not Found
    void SetHeader(const std::string &key, const std::string &value)
    {
        auto it = _headers.find(key);
        if (it != _headers.end())
        {
            return;
        }
        else
        {
            _headers.insert({key, value});
        }
    }
    std::string Uri2Suffix(const std::string &uri)
    {
        auto pos = uri.rfind(".");
        if (pos == std::string::npos)
        {
            return "text/html";
        }
        std::string suffix = uri.substr(pos);
        if (suffix == ".css")
        {
            return "text/css";
        }
        else if (suffix == ".js")
        {
            return "application/javascript";
        }
        else if (suffix == ".jpg" || suffix == ".jpeg")
        {
            return "image/jpeg";
        }
        else if (suffix == ".png")
        {
            return "image/png";
        }
        else if (suffix == ".gif")
        {
            return "image/gif";
        }
        else if (suffix == ".ico")
        {
            return "image/x-icon";
        }
        else if (suffix == ".html" || suffix == ".htm")
        {
            return "text/html";
        }
        else
        {
            return "text/html";
        }
        return "text/html";
    }
    bool MakeResponse()
    {
        if (_targetfile == "./wwwroot/favicon.ico")
        {
            LOG(Loglevel::DEBUG) << "用户请求 ：" << _targetfile << "忽略他";
            return false;
        }
        if (_targetfile == "./wwwroot/redir_test")
        {
            SetCode(301);
            SetHeader("Location", "https://blog.csdn.net/2502_93089837?type=blog");
            return true;
        }
        // 尝试读取客户端请求的文件内容
        bool res = Util::ReadFileContent(_targetfile, &_text);

        // 文件读取失败，说明资源不存在
        if (!res)
        {
            // SetCode(404);
            // _targetfile = wberoot + page_404; // 设置404页面
            // int filesize = Util::FileSize(_targetfile);
            // SetHeader("Content-Length", std::to_string(filesize));
            // std::string suffix = Uri2Suffix(_targetfile);
            // SetHeader("Content-Type", suffix);
            // Util::ReadFileContent(_targetfile, &_text); // 读取404页面内容
            // return false;
            SetCode(302);
            SetHeader("Location", "http://122.51.11.221:8080/404.html");
            return true;
        }
        else
        {
            // 文件读取成功
            SetCode(200);
            int filesize = Util::FileSize(_targetfile);
            SetHeader("Content-Length", std::to_string(filesize));
            std::string suffix = Uri2Suffix(_targetfile);
            SetHeader("Content-Type", suffix);
            return true;
        }
    }

    // 对HTTP响应进行反序列化
    // 一般服务端主要负责生成响应，所以这里暂时没有具体实现
    bool Deserialize()
    {
        return true;
    }

    // private:
public:
    // HTTP响应状态行相关字段
    std::string _version; // HTTP版本，例如 HTTP/1.1
    int _code;            // HTTP状态码，例如 200、404
    std::string _desc;    // 状态码描述，例如 OK、Not Found

    // HTTP响应头
    std::unordered_map<std::string, std::string> _headers;

    // HTTP响应头和正文之间的空行
    std::string _blankline;

    // HTTP响应正文
    // 对于静态Web服务器，一般就是HTML/CSS/JS文件内容
    std::string _text;

    // 其他属性

    // 当前HTTP请求对应的服务器本地文件路径
    std::string _targetfile;
};

// Http：HTTP服务器的上层封装
//
// 底层使用 TcpServer 完成TCP通信
// 上层负责：
// 1. 接收HTTP请求
// 2. 解析HttpRequest
// 3. 找到客户端请求的文件
// 4. 构建HttpResponse
// 5. 将HTTP响应发送给客户端
class Http
{
public:
    // 创建Http服务器时，同时创建底层TcpServer
    Http(uint16_t port) : _tsvrp(std::make_unique<TcpServer>(port))
    {
    }

    // 处理一次客户端HTTP请求
    //
    // sock   ：当前客户端对应的通信Socket
    // client ：当前客户端的IP、端口等信息
    void HandlerHttpRquest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {
        // 收到请求

        // 保存浏览器发送过来的完整HTTP请求
        std::string httprequest;

        // 假设：概率大读到完整的请求
        // 从TCP连接中读取客户端发送过来的HTTP请求
        int n = sock->Recv(&httprequest);

        // 成功读取到客户端数据
        if (n > 0)
        {
            // 创建HTTP请求对象
            HttpRequest req;

            // 对客户端发送过来的HTTP请求进行反序列化
            req.Deserialize(httprequest);

            // 创建HTTP响应对象
            HttpResponse resp;

            // 将客户端请求URI对应的本地文件设置为响应目标文件
            resp.SetTargetFile(req.Uri());

            // 根据目标文件是否存在，构建200或者404响应
            resp.MakeResponse();
            std::string response_str = resp.Serialize();
            sock->Send(response_str);
            // std::string filename = req.Uri();

            // // 创建HTTP响应对象
            // HttpResponse resp;

            // // 设置HTTP响应状态行
            // resp._version = "HTTP/1.1";
            // resp._code = 200;
            // resp._desc = "OK";

            // // 打印客户端请求的资源
            // // LOG(Loglevel::DEBUG) << "用户请求：" << filename;

            // // 将客户端请求的文件读取到响应正文 _text 中
            // bool res = Util::ReadFileContent(filename, &resp._text);

            // // 将HttpResponse对象序列化成HTTP协议格式的字符串
            // std::string response_str = resp.Serialize();

            // // 通过TCP连接将HTTP响应发送给客户端
            // sock->Send(response_str);
        }

        // 处理HTTP请求

        // #ifndef DEBUG
        // #define DEBUG

#ifdef DEBUG

        // DEBUG模式下的HTTP请求处理逻辑
        // 主要用于测试服务器能否正常接收HTTP请求并返回首页

        // 收到请求
        std::string httprequest;

        // 假设：概率大读到完整的请求
        // 从客户端读取HTTP请求
        sock->Recv(&httprequest);

        // 直接打印浏览器发送来的原始HTTP请求
        std::cout << httprequest << std::endl;

        // 构建HTTP响应
        HttpResponse resp;

        // 设置响应状态
        resp._version = "HTTP/1.1";
        resp._code = 200;
        resp._desc = "OK";

        // DEBUG模式下固定返回服务器首页
        std::string filename = wberoot + homepage;

        // 读取首页内容到HTTP响应正文
        bool res = Util::ReadFileContent(filename, &resp._text);

        // 将响应对象转换成HTTP响应字符串
        std::string response_str = resp.Serialize();

        // 将HTTP响应发送给浏览器
        sock->Send(response_str);

#endif
    }

    // 启动HTTP服务器
    void Start()
    {
        // 启动底层TcpServer
        // TcpServer收到新客户端请求后，
        // 回调当前Http对象的HandlerHttpRquest函数处理HTTP业务
        _tsvrp->Start([this](std::shared_ptr<Socket> &sock, InetAddr &client)
                      { this->HandlerHttpRquest(sock, client); });
    }

    ~Http()
    {
    }

private:
    // Http服务器底层的TCP服务器
    // unique_ptr表示Http对象独占这个TcpServer对象
    std::unique_ptr<TcpServer> _tsvrp;
};
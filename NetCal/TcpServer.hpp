#include "Socket.hpp"
#include <iostream>
#include <memory>
#include <sys/wait.h>
#include <functional>

using namespace SocketModule;
using namespace LogModule;

using ioservice_t = std::function<void(std::shared_ptr<Socket> &sock, InetAddr &client)>;

// 主要解决：连接的问题，IO通信的问题
// 细节: TcpServer,需不需要关心自己未来传递的信息是什么？不需要关心！！
// 网络版本的计算器，长服务
class TcpServer
{
public:
    TcpServer(uint16_t port, ioservice_t service) : _port(port),
                                                    _listensockptr(std::make_unique<TcpSocket>()),
                                                    _isrunning(false),
                                                    _service(service)
    {
        _listensockptr->BuildTcpSocketMethod(_port);
    }
    void Start()
    {
        _isrunning = true;
        while (_isrunning)
        {
            InetAddr client;
            auto sock = _listensockptr->Accept(&client); // 1. 和client通信sockfd 2. client 网络地址
            if (sock == nullptr)
            {
                continue;
            }
            LOG(Loglevel::DEBUG) << "accept success ...";

            // sock && client
            pid_t id = fork();
            if (id < 0)
            {
                LOG(Loglevel::FATAL) << "fork error ...";
                exit(FORK_ERR);
            }
            else if (id == 0)
            {
                // 子进程 -> listensock
                _listensockptr->Close();
                if (fork() > 0)
                    exit(OK);
                // 孙子进程在执行任务，已经是孤儿了
                _service(sock, client);
                exit(OK);
            }
            else
            {
                // 父进程 -> sock
                sock->Close();
                pid_t rid = ::waitpid(id, nullptr, 0);
                (void)rid;
            }
        }
        _isrunning = false;
    }
    ~TcpServer() {}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockptr;
    bool _isrunning;
    ioservice_t _service;
};
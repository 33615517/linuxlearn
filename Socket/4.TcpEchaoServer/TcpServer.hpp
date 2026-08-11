#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ThreadPool.hpp"
#include <memory>
#include <functional>
#include <unistd.h>
#include <memory>
#include <functional>
#include "Thread.hpp"
#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"
#include"Dict.hpp"
#include <sys/wait.h>
#include <thread>
// 服务器往往是禁止拷贝的
using namespace LogModule;
using namespace ThreadModlue;
using namespace MutexModule;
using namespace ThreadPoolModule;

//using task_t = std::function<void()>;
using func_t = std::function<std::string(const std::string&, InetAddr&)>; // 业务逻辑函数类型
const static int defaultsockfd = -1;
class TcpServer : public NoCopy
{
public:
    TcpServer(uint16_t port,func_t func) : _port(port), _listensockfd(-1), _isrunning(false), _func(func)
    {
    }
    ~TcpServer()
    {
    }
    class ThreadDate
    {
    public:
        ThreadDate(int fd, InetAddr &addr, TcpServer *tsvr) : sockfd(fd), addr(addr), tsvr(tsvr)
        {
        }
        int sockfd;
        InetAddr addr;
        TcpServer *tsvr;

    private:
    };
    void Service(int sockfd, InetAddr &peeraddr)
    {
        char buffer[1024];
        while (true)
        {
            memset(buffer, 0, sizeof(buffer));
            ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
            if (n < 0)
            {
                LOG(Loglevel::ERROR) << "client " << peeraddr.StringAddr() << " read error";
                close(sockfd);
                break;
            }
            else if (n == 0)
            {
                LOG(Loglevel::DEBUG) << "client " << peeraddr.StringAddr() << " 退出了……";
                close(sockfd);
                break;
            }
            else
            {
                buffer[n] = '\0';
                LOG(Loglevel::DEBUG) << peeraddr.StringAddr() << " # " << buffer;
               std::string result = _func(buffer,peeraddr); // 调用业务逻辑函数
                // LOG(Loglevel::DEBUG) << "recv data:" << peeraddr.StringAddr() << "say# " << buffer;
                write(sockfd, result.c_str(), result.size());
            }
        }
        close(sockfd);
    }
    static void *Routine(void *arg)
    {
        pthread_detach(pthread_self());
        ThreadDate *td = static_cast<ThreadDate *>(arg);
        td->tsvr->Service(td->sockfd, td->addr);
        delete td;
        return nullptr;
    }
    void Run()
    {
        _isrunning = true;
        while (_isrunning)
        {
            // 4.阻塞等待客户端连接accept
            struct sockaddr_in peeraddr;
            socklen_t len = sizeof(peeraddr);
            // 如果没有客户端连接，accept会阻塞等待
            int sockfd = accept(_listensockfd, CONV(peeraddr), &len);
            if (sockfd < 0)
            {
                LOG(Loglevel::ERROR) << "accept error";
                continue;
            }
            InetAddr addr(peeraddr);
            LOG(Loglevel::INFO) << "accept success:" << sockfd << " from " << addr.StringAddr();
            // version -- 多线程版本
            ThreadDate *td = new ThreadDate(sockfd, addr, this);
            pthread_t tid;
            pthread_create(&tid, nullptr, Routine, td);
            // version -- test version
            // Service(sockfd, addr);

            // version -- 多进程版本
            // pid_t id = fork();
            // if (id < 0)
            // {
            //     LOG(Loglevel::FATAL) << "fork error";
            //     exit(ExitCode::SOCKET_ERROR);
            //     continue;
            // }
            // else if (id == 0)
            // {
            //     // child
            //     close(_listensockfd); // 关闭监听套接字
            //     if(fork() > 0)
            //     {
            //         exit(OK); // 子进程退出
            //     }

            //     Service(sockfd, addr);
            //     exit(OK); // 子进程退出
            // }
            // else
            // {
            //     // parent
            //     close(sockfd); // 关闭通信套接字
            //     pid_t rid = waitpid(id, nullptr, 0); // 回收子进程
            //     (void)rid;
            // }

            // // version -- 多线程版本
            // ThreadDate *td = new ThreadDate(sockfd, addr, this);
            // pthread_t tid;
            // pthread_create(&tid,nullptr,Routine,td);

            // version3 -- 线程池版本，线程池适合短服务
            // 将新链接和客户端地址封装成一个任务，交给线程池处理
            //  ThreadPool<task_t>::GetInstance()->Equeue([this, &sockfd, &addr]()
            //  {
            //      this->Service(sockfd, addr);
            //  });
        }
        _isrunning = false;
    }
    void Init()
    {

        // 1.创建套接字
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensockfd < 0)
        {
            LOG(Loglevel::FATAL) << "socket error";
            exit(ExitCode::SOCKET_ERROR);
        }
        LOG(Loglevel::INFO) << "socket success:" << _listensockfd;

        // 2.绑定bind众所周知的端口号
        InetAddr local(_port);
        int n = bind(_listensockfd, local.NetAddrPtr(), local.NetAddrLen());
        if (n < 0)
        {
            LOG(Loglevel::FATAL) << "bind error";
            exit(ExitCode::BIND_ERROR);
        }
        LOG(Loglevel::INFO) << "bind success:" << _listensockfd;

        // 3.设置socket状态为监听listen
        n = listen(_listensockfd, 5);
        if (n < 0)
        {
            LOG(Loglevel::FATAL) << "listen error";
            exit(ExitCode::LISTEN_ERROR);
        }
        LOG(Loglevel::INFO) << "listen success:" << _listensockfd;
    }

private:
    uint16_t _port;
    int _listensockfd; // 监听套接字
    bool _isrunning;
    func_t _func; // 业务逻辑函数
};
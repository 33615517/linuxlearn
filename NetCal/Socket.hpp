#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"

namespace SocketModule
{
    using namespace LogModule;
    const static int gbacklog = 16;
    // 模版方法模式
    // 基类socket, 大部分方法，都是纯虚方法
    class Socket
    {
    public:
        virtual ~Socket() {}
        virtual void SocketOrDie() = 0;
        virtual void BindOrDie(uint16_t port) = 0;
        virtual void ListenOrDie(int backlog) = 0;
        virtual std::shared_ptr<Socket> Accept(InetAddr *client) = 0;
        virtual void Close() = 0;

    public:
        void BuildTcpSocketMethod(uint16_t port, int backlog = gbacklog)
        {
            SocketOrDie();
            BindOrDie(port);
            ListenOrDie(backlog);
        }
        // void BuildUdpSocketMethod()
        // {
        //     SocketOrDie();
        //     BindOrDie();
        // }
    };

    const static int defaultfd = -1;

    class TcpSocket : public Socket
    {
    public:
        TcpSocket():_sockfd(defaultfd)
        {}
        TcpSocket(int fd):_sockfd(fd)
        {}
        ~TcpSocket() {}
        void SocketOrDie() override
        {
            _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                LOG(Loglevel::FATAL) << "socket error";
                exit(SOCKET_ERR);
            }
            LOG(Loglevel::INFO) << "socket success";
        }
        void BindOrDie(uint16_t port) override
        {
            InetAddr localaddr(port);
            int n = ::bind(_sockfd, localaddr.NetAddrPtr(), localaddr.NetAddrLen());
            if (n < 0)
            {
                LOG(Loglevel::FATAL) << "bind error";
                exit(BIND_ERR);
            }
            LOG(Loglevel::INFO) << "bind success";
        }
        void ListenOrDie(int backlog) override
        {
            int n = ::listen(_sockfd, backlog);
            if (n < 0)
            {
                LOG(Loglevel::FATAL) << "listen error";
                exit(LISTEN_ERR);
            }
            LOG(Loglevel::INFO) << "listen success";
        }
        std::shared_ptr<Socket> Accept(InetAddr *client) override
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int fd = ::accept(_sockfd, CONV(peer), &len);
            if(fd < 0)
            {
                LOG(Loglevel::WARNING) << "accept warning ...";
                return nullptr; // TODO
            }
            client->SetAddr(peer);
            return std::make_shared<TcpSocket>(fd);
        }
        void Close() //??
        {
            if(_sockfd >= 0)
                ::close(_sockfd);
        }

    private:
        int _sockfd; // _sockfd , listensockfd, sockfd;
    };

    // class UdpSocket : public Socket
    // {
    // };
}
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "InetAddr.hpp"
#include "Log.hpp"
#include"Mutex.hpp"
using namespace LogModule;
using namespace MutexModule;
class Route
{
private:
    bool IsExist(InetAddr &user)
    {
        for (auto &u : _online_users)
        {
            if (u == user)
            {
                return true;
            }
        }
        return false;
    }
    void AddUser(InetAddr &user)
    {
        LOG(LogModule::Loglevel::INFO) << "New user online: " << user.StringAddr();
        _online_users.push_back(user);
    }
    void DeleteUser(InetAddr &user)
    {
        LOG(LogModule::Loglevel::INFO) << "User offline: " << user.StringAddr();
        for (auto it = _online_users.begin(); it != _online_users.end(); ++it)
        {
            if (*it == user)
            {
                _online_users.erase(it);
                break;
            }
        }
    }

public:
    Route()
    {
    }
    void MessageRoute(int sockfd, const std::string &message, InetAddr &peer)
    {
        LockGuard lock(_mutex); // 保护在线用户列表的互斥锁
        
        if (!IsExist(peer))
        {
            AddUser(peer);
        }

        std::string send_message = peer.StringAddr() + " # " + message;
        for (auto &user : _online_users)
        {
            sendto(sockfd, send_message.c_str(), send_message.size(), 0, (struct sockaddr *)&user.NetAddr(), sizeof(user.NetAddr()));
        }

        if (message == "quit")
        {
            LOG(LogModule::Loglevel::INFO) << "删除一个在线用户: " << peer.StringAddr();
            DeleteUser(peer);
        }
    }
    ~Route()
    {
    }

private:
    // 首次给我发消息，等同于登录
    std::vector<InetAddr> _online_users; // 在线用户列表
    Mutex _mutex; // 保护在线用户列表的互斥锁
};
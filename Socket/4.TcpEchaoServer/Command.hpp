#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include<cstdio>
#include <cstdlib>
#include "InetAddr.hpp"
#include "Log.hpp"
#include"Log.hpp"
#include <set>
#include "Common.hpp"
using namespace LogModule;
class Command
{
public:
    Command()
    {
        // 严格匹配
        _whiteListcommands.insert("ls");
        _whiteListcommands.insert("pwd");
        _whiteListcommands.insert("ls -l");
        _whiteListcommands.insert("ll");
        _whiteListcommands.insert("touch");
        _whiteListcommands.insert("who");
        _whiteListcommands.insert("whoami");
    }
    bool InSafeCommand(const std::string &cmd)
    {
        return _whiteListcommands.find(cmd) != _whiteListcommands.end();
    }
    std::string Execute(const std::string &cmd, InetAddr &addr)
    {
        //1.属于白名单命令
        if(!InSafeCommand(cmd))
        {
            return std::string("你要执行的命令不存在或者不在白名单中")+cmd;
        }
        std::string who = addr.StringAddr();
        //2.执行命令
        FILE *fd = popen(cmd.c_str(), "r");
        if (fd == nullptr)
        {
            return std::string("你要执行的命令不存在或者不在白名单中")+cmd;
        }
        std::string result;
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fd) != nullptr)
        {
            result += buffer;
        }
        pclose(fd);
        std::string res = who+"executed done,result is:\n"+result;
        LOG(Loglevel::DEBUG) << res;
        return res;

    }
    ~Command()
    {
    }

private:
    // 受限制的远程执行
    std::set<std::string> _whiteListcommands;
};
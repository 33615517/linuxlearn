#pragma once
#include <fcntl.h>

#include <string>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include "Log.hpp"
#include "Common.hpp"
using namespace LogModule;
const std::string dev = "/dev/null";
void Daemon(int nochdir , int noclose)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    if (fork() > 0)
    {
        exit(0);
    }

    setsid();
    if(nochdir == 0)
    {
        chdir("/");
    }
    if (noclose == 0)
    {
        // 如果noclose为0，则重定向标准输入、输出和错误到/dev/null
        int fd = ::open(dev.c_str(), O_RDWR);
        if (fd < 0)
        {
            LOG(Loglevel::ERROR) << "open " << dev << " error";
            exit(OPEN_ERR);
        }
        else
        {
            dup2(fd, 0);
            dup2(fd, 1);
            dup2(fd, 2);

            close(fd);
        }
    }
}
#pragma once


#include<string>
#include<cstdio>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>
void Daemon()
{
    signal(SIGCHLD, SIG_IGN);
    if(fork()>0)
    {
        exit(0);
    }
    setsid();

}
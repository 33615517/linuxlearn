#pragma once

#include <iostream>
#include <functional>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

enum ExitCode
{
    OK = 0,
    USAGE_ERR,
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR,
    CONNECT_ERR,
    FORK_ERR,
    OPEN_ERR
};

class NoCopy
{
public:
    NoCopy(){}
    ~NoCopy(){}
    NoCopy(const NoCopy &) = delete;
    const NoCopy &operator = (const NoCopy&) = delete;
};

#define CONV(addr) ((struct sockaddr*)&addr)
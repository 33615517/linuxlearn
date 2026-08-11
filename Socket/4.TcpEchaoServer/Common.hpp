#pragma once
#include <iostream>
#include<unistd.h>
enum ExitCode
{
    OK = 0,
    USAGE_ERROR ,
    SOCKET_ERROR,
    BIND_ERROR,
    LISTEN_ERROR,
    CONNECT_ERROR,
};

class NoCopy
{
public:
    NoCopy()
    {

    }
    ~NoCopy() 
    {
        
    }
    NoCopy(const NoCopy &) = delete;
    const NoCopy &operator=(const NoCopy &) = delete;
private:


};



#define CONV(addr) ((struct sockaddr *)&addr)
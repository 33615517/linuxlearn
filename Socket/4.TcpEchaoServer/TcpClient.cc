#include<iostream>
#include "Common.hpp"
#include <sys/socket.h>
#include <cstdint>
#include"InetAddr.hpp"
void Usage(std::string proc)
{
    std::cout << "Usage:" << proc << " server_ip server_port" << std::endl;
}

// ./Tcpclient server_ip server_port
int main(int argc , char *argv[])
{
    //1.创建套接字socket
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(SOCKET_ERROR);
    }
    //2.连接服务器connect
    InetAddr serveraddr(server_ip, server_port);
    int n = connect(sockfd, serveraddr.NetAddrPtr(), sizeof(serveraddr));
    if (n < 0)
    {
        std::cerr << "connect error" << std::endl;
        exit(CONNECT_ERROR);
    }

    //3.通信 echo client
    while (true)
    {
        std::string line;
        std::cout << "Please Enter# ";
        std::getline(std::cin, line);
        if (line.empty())
        {
            continue;
        }
        write(sockfd, line.c_str(), line.size());

        char buffer[1024] = {0};

        ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
        if(n > 0)
        {
            buffer[n] = '\0';
            std::cout << "recv echo# " << buffer << std::endl;
        }
       
    }
    return 0;
}
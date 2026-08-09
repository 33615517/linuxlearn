#include <iostream>
#include <cstdint>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string>
#include <sys/socket.h>
#include <cstring>
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " ip port" << std::endl;
        return 1;
    }
    std::string sever_ip = argv[1];
    uint16_t server_port = static_cast<uint16_t>(std::stoi(argv[2]));
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 2;
    }
    // 2. 本地的ip和端口是什么？要不要和上面的“文件”关联呢？
    // 问题：client要不要bind？需要bind
    //      client要不要显式的bind？不要显式bind，首次发送消息，OS会自动给client进行bind，OS知道IP，端口号采用随机端口号的方式
    //      为什么？一个端口号，只能被一个进程bind，为了避免client端口冲突
    //client端口号是随机的，不重要，只要是唯一的就行
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(sever_ip.c_str());   

    while(true)
    {
        std::string message;
        std::cout << "Please enter a message:";
        std::getline(std::cin, message);
        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));
        char buffer[1024];
        struct sockaddr_in peer;
        socklen_t peer_len = sizeof(peer);
        int m = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &peer_len);
        if (m > 0)
        {
            buffer[m] = 0;
            std::cout << "Received from server: " << buffer << std::endl;
        }
    }

    return 0;
}
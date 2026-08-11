#include "Protocol.hpp"
#include "TcpServer.hpp"
#include <memory>

void Usage(std::string proc)
{
    std::cerr << "Usage: " << proc << " port" << std::endl;
}

// ./tcpserver 8080
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>();

    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(std::stoi(argv[1]),
        [&protocol](std::shared_ptr<Socket> &sock, InetAddr &client){
            protocol->GetRequest(sock, client);
    });
    tsvr->Start();

    return 0;
}
#include"Socket.hpp"
#include"Common.hpp"
#include<memory>
#include<string>
using namespace SocketModule;
void Usage(std::string proc)
{
    std::cerr << "Usage: " << proc << " server_ip server_port" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);
    std::unique_ptr<TcpSocket> client = std::make_unique<TcpSocket>();
    client->BuildTcpClinentSocketMethod();
    if(client->Connect(server_ip, server_port) == 0)
    {
        //成功
    }
    return 0;
}
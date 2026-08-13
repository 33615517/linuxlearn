#include "Socket.hpp"
#include "Common.hpp"
#include <memory>
#include "Protocol.hpp"
#include <string>
using namespace SocketModule;
void Usage(std::string proc)
{
    std::cerr << "Usage: " << proc << " server_ip server_port" << std::endl;
}
void GetDateFromStdin(int *x, int *y, char *oper)
{
    std::cout << "请输入计算表达式(如: 10 + 20): ";
    std::cin >> *x >> *oper >> *y;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);
    std::shared_ptr<Socket> client = std::make_shared<TcpSocket>();
    client->BuildTcpClinentSocketMethod();
    if (client->Connect(server_ip, server_port) != 0)
    {
        // 失败
        std::cerr << "connect error" << std::endl;
        exit(CONNECT_ERR);
    }
    // 连接服务器成功
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>();
    std::string resp_buff;
    while (true)
    {
        // 1.从标准输入获取计算表达式
        std::string request_str;
        int cnt = 3;
        while (cnt--)
        {
            int x, y;
            char oper;
            GetDateFromStdin(&x, &y, &oper);
            // 2.构建一个请求->可以直接发送的字符串
            request_str += protocol->BuildRequestString(x, y, oper);
            std::cout << "-----------request_str------------" << std::endl;
            std::cout << "request_str: " << request_str << std::endl;
            std::cout << "-----------request_str------------" << std::endl;
        }
        // 3.发送请求
        client->Send(request_str);
       sleep(2);
            // 4.接收响应
            Response resp;
            bool ret = protocol->GetResponse(client, resp_buff, &resp);
            if (!ret)
            {
                std::cerr << "get response error ..." << std::endl;
                break;
            }
            // 5.显示结果
            resp.ShowResult();
        
    }
    client->Close();
    return 0;
}
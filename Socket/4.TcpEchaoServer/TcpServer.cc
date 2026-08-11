#include"TcpServer.hpp"
#include"Dict.hpp"
#include"Command.hpp"
#include<iostream>
std::string defaultthandler(const std::string& buffer, InetAddr& peeraddr)
{
    LOG(Loglevel::DEBUG) << "业务逻辑函数被调用了，处理数据:" << buffer;
    std::string s="haha, ";
    s+= buffer;
    return s;
}
void Usage(const std::string& name)
{
    std::cerr << "Usage: " << name << " <port>" << std::endl;
}

//远程命令执行的功能
// ./TcpClient port

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(ExitCode::USAGE_ERROR);
    }
    uint16_t port = std::stoi(argv[1]);

    Enable_Console_log_strategy();
    // //1.翻译模块
    // Dict d;
    // d.LoadDict();
    //1.命令的执行模块
    Command cmd;
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port,std::bind(&Command::Execute,&cmd,std::placeholders::_1,std::placeholders::_2));



    // LOG(Loglevel::INFO) << "TcpServer is starting...";
    // std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port,[&d](const std::string& buffer, InetAddr& peeraddr){
    //     return d.Translate(buffer, peeraddr);
    // });

   


    tsvr->Init();
    tsvr->Run();
    return 0;
}
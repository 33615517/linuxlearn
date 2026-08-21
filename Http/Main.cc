#include"Http.hpp"

void Login(HttpRequest &req, HttpResponse &resp)
{
    LOG(Loglevel::DEBUG) << req.Args() << ",我们成功进入到了处理数据的逻辑";
    std::string text = "Hello, " + req.Args() ;

    //登陆认证





    resp.SetCode(200);
    resp.SetHeader("Content-Length", std::to_string(text.size()));
    resp.SetHeader("Content-Type", "text/plain");
    resp.SetText(text);
}

 
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<Http> httpserver = std::make_unique<Http>(port);

    httpserver->RegisterHandler("/login", Login);

    httpserver->Start();
    return 0;
}
#include"ServerMain.hpp"
#include"IOServer.hpp"
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int16_t port = atoi(argv[1]);
    IOServer iosrv;
    server_io_t io_func=std::bind(&IOServer::IOExcute, &iosrv, std::placeholders::_1, std::placeholders::_2);
    std::unique_ptr<TcpServer> tsvr=std::make_unique<TcpServer>(port,io_func);
    tsvr->loop();
    return 0;
}
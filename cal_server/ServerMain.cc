#include"ServeMain.hpp"
#include"IOServer.hpp"
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int16_t port = atoi(argv[1]);

    IOServer io_server;
    std::unique_ptr<TcpServer> tsvr= std::make_unique<TcpServer>(port, std::bind(&IOServer::IOExcute, &io_server, std::placeholders::_1, std::placeholders::_2));
    tsvr->loop();
    return 0;
}
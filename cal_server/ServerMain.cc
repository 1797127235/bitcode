#include "ServerMain.hpp"
#include "IOServer.hpp"
#include "NetCal.hpp"

// ./tcpserver 8888
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " local-port" << std::endl;
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);
    
    
    NetCal cal;
    IOServer service(std::bind(&NetCal::Calculator, &cal, std::placeholders::_1));

    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(
        std::bind(&IOServer::IOExcute, &service, std::placeholders::_1, std::placeholders::_2), 
        port
    );
    tsvr->loop();

    return 0;
}
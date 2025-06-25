#include "ServerMain.hpp"
#include "NetCal.hpp"
#include "IOServer.hpp"
//using process_t = std::function<std::shared_ptr<Response>(std::shared_ptr<Request>)>;
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int16_t port = atoi(argv[1]);
    NetCal cal;
    process_t process_func = std::bind(&NetCal::Calculator, &cal, std::placeholders::_1); 
    IOServer iosrv(process_func);
    server_io_t io_func=std::bind(&IOServer::IOExcute, &iosrv, std::placeholders::_1, std::placeholders::_2);
    std::unique_ptr<TcpServer> tsvr=std::make_unique<TcpServer>(port,io_func);
    tsvr->loop();
    return 0;
}
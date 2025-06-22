#include"Socket.hpp"
#include<thread>
using namespace socket_ns;

using server_io_t = std::function<void(SockPtr, const InetAddr &)>;
class TcpServer
{
public:
    TcpServer(int16_t port, server_io_t io_func):
        _listen_sock(std::make_shared<TcpSocket>()),
        _is_running(false),
        _io_func(io_func)
    {
        _listen_sock->BuildingListenSocket(port);
    }
    
    class ThreadData
    {
    public:
        SockPtr _sockfd;
        TcpServer *_self;
        InetAddr _addr;
    public:
        ThreadData(SockPtr sockfd, TcpServer *self, const InetAddr &addr):
            _sockfd(sockfd),
            _self(self),
             _addr(addr)
        {}
    };

    void loop()
    {
        _is_running = true;
        while(_is_running)
        {
            InetAddr peer_addr;
            SockPtr sockfd = _listen_sock->Accepter(&peer_addr);
            if(sockfd == nullptr)
            {
                continue;
            }

            std::thread t([this, sockfd, peer_addr]()
            {
                try {
                    _io_func(sockfd, peer_addr);
                } catch (const std::exception& e) {
                    LOG(ERROR_, "Exception in client handler: %s", e.what());
                } catch (...) {
                    LOG(ERROR_, "Unknown exception in client handler");
                }
                sockfd->Close();
            });
            t.detach();
        }
    }
private:
    SockPtr _listen_sock;
    bool _is_running;
    server_io_t _io_func;
};
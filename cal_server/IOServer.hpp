#pragma once
#include<iostream>
#include"Socket.hpp"
#include"Log.hpp"
#include<string>
using namespace socket_ns;
using namespace log_ns;
class IOServer
{
public:
    IOServer()=default;

    void IOExcute(SockPtr sockfd, const InetAddr &peer_addr)
    {
        while(true)
        {
            std::string request;
            int n=sockfd->Recv(&request);
            LOG(INFO, "Request: %s", request.c_str());
        }
    }
};
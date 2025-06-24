#pragma once
#include<iostream>
#include"Socket.hpp"
#include"Log.hpp"
#include"Protocol.hpp"
#include<string>
using namespace socket_ns;
using namespace log_ns;
class IOServer
{
public:
    IOServer()=default;
    ~IOServer()=default;
    void IOExcute(SockPtr sockfd, const InetAddr &peer_addr)
    {
        std::string packagestreamqueue;
        while(true)
        {
           try{
                int n = sockfd->Recv(&packagestreamqueue);
                if(n==0)
                {
                    LOG(INFO, "Connection closed by peer");
                    break;
                }
                std::string request=Decode(packagestreamqueue);
                //LOG(INFO, "Request: %s", request.c_str());
           }
           catch(const std::exception& e){
                LOG(ERROR_, "Exception in client handler: %s", e.what());
                break;
           }
        }
    }
};
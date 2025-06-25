#pragma once
#include<iostream>
#include<functional>
#include"Socket.hpp"
#include"Log.hpp"
#include"Protocol.hpp"
#include<string>
using namespace socket_ns;
using namespace log_ns;
using process_t = std::function<std::shared_ptr<Response>(std::shared_ptr<Request>)>;
class IOServer
{
public:
    IOServer(process_t process_func):
        _process_func(process_func)
    {}

    ~IOServer()=default;
    
    void IOExcute(SockPtr sockfd, const InetAddr &peer_addr)
    {
        std::string packagestreamqueue;
        //接收请求向上交付然后返回响应
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
                
                Request req;
                req.Deserialize(request);
                std::shared_ptr<Response> resp = _process_func(std::make_shared<Request>(req));
                std::string response;
                //序列化响应
                resp->Serialize(&response);

                Encode(response);
                std::string send_str=Encode(response);
                sockfd->Send(send_str);
           }
           catch(const std::exception& e){
                LOG(ERROR_, "Exception in client handler: %s", e.what());
                break;
           }
        }
    }
private:
    process_t _process_func;
};
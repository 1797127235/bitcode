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
                LOG(INFO, "Received request: %s", request.c_str());
                
                if(request.empty()) {
                    continue; // 数据不完整，继续接收
                }
                
                auto req = Factory::CreateRequest();
                if(!req->Deserialize(request)) {
                    LOG(ERROR_, "Failed to deserialize request: %s", request.c_str());
                    // 发送错误响应
                    auto error_resp = std::make_shared<Response>(0, 4, "deserialization error");
                    std::string error_response;
                    error_resp->Serialize(&error_response);
                    std::string error_send_str = Encode(error_response);
                    sockfd->Send(error_send_str);
                    continue;
                }
                
                LOG(INFO, "Deserialized: x=%d, op='%c', y=%d", req->getx(), req->getop(), req->gety());
                
                std::shared_ptr<Response> resp = _process_func(req);
                std::string response;
                //序列化响应
                resp->Serialize(&response);
                LOG(INFO, "Response: %s", response.c_str());

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
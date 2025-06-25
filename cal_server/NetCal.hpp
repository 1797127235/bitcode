#pragma once

#include "Protocol.hpp"
#include <memory>

class NetCal
{
public:
    NetCal()
    {
    }
    ~NetCal()
    {
    }
    std::shared_ptr<Response> Calculator(std::shared_ptr<Request> req)
    {
        char op=req->getop();
        std::cerr<<"[NetCal DEBUG] op is "<<(int)op<<" (char: '"<<op<<"')"<<std::endl;
        std::cerr<<"[NetCal DEBUG] x="<<req->getx()<<", y="<<req->gety()<<std::endl;
        auto resp=Factory::CreateResponse();
        switch (op)
        {
        case 43:  // '+' 的ASCII码
            resp->_result = req->getx() + req->gety();
            resp->_code = 0;
            resp->_desc = "success";
            break;
        case 45:  // '-' 的ASCII码
            resp->_result = req->getx() - req->gety();
            resp->_code = 0;
            resp->_desc = "success";
            break;
        case 42:  // '*' 的ASCII码
            resp->_result = req->getx() * req->gety();
            resp->_code = 0;
            resp->_desc = "success";
            break;
        case 47:  // '/' 的ASCII码
        {
            if (req->gety() == 0)
            {
                resp->_code = 1;
                resp->_desc = "div zero";
            }
            else
            {
                resp->_result = req->getx() / req->gety();
                resp->_code = 0;
                resp->_desc = "success";
            }
        }
        break;
        case '%':
        {
            if (req->gety() == 0)
            {
                resp->_code = 2;
                resp->_desc = "mod zero";
            }
            else
            {
                resp->_result = req->getx() % req->gety();
                resp->_code = 0;
                resp->_desc = "success";
            }
        }
        break;
        default:
        {
            resp->_code = 3;
            resp->_desc = "illegal operation";
        }
        break;
        }
        return resp;
    }
};
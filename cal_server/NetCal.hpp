#include"Protocol.hpp"


class NetCal{
public:
    NetCal()=default;
    ~NetCal()=default;

    std::shared_ptr<Response> Calculator(std::shared_ptr<Request> request)
    {
        char op=request->getop();
        switch(op)
        {
            case '+':
                return std::make_shared<Response>(request->getx()+request->gety(),0,"success");
                break;
            case '-':
                return std::make_shared<Response>(request->getx()-request->gety(),0,"success");
                break;
            case '*':
                return std::make_shared<Response>(request->getx()*request->gety(),0,"success");
                break;
            case '/':
                if(request->gety()==0)
                {
                    return std::make_shared<Response>(0,1,"error");
                }
                else
                {
                    return std::make_shared<Response>(request->getx()/request->gety(),0,"success");
                }
                break;
            default:
                return std::make_shared<Response>(0,1,"error");
                break;
        }
    }

};
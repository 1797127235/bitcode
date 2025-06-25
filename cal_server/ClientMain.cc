#include"Protocol.hpp"
#include"Socket.hpp"
#include"Log.hpp"
using namespace socket_ns;
using namespace log_ns;
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        std::cerr<<"Usage: "<<argv[0]<<" <server_ip> <server_port>"<<std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    int server_port = atoi(argv[2]);
    try
    {
        TcpSocket sock;
        sock.BuildClientSocket(server_ip,server_port);
        while(true)
        {
            LOG(INFO,"Please enter expression (e.g., 1+2): ");
            std::string input;
            std::getline(std::cin, input);
            
            // 解析输入，支持 "1+2" 格式
            int x, y;
            char op;
            size_t op_pos = std::string::npos;
            
            // 查找操作符
            for (size_t i = 1; i < input.length(); i++) {
                if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
                    op_pos = i;
                    op = input[i];
                    break;
                }
            }
            
            if (op_pos == std::string::npos) {
                LOG(ERROR, "Invalid input format. Please use format like: 1+2");
                continue;
            }
            
            try {
                x = std::stoi(input.substr(0, op_pos));
                y = std::stoi(input.substr(op_pos + 1));
            } catch (const std::exception& e) {
                LOG(ERROR, "Invalid numbers in expression");
                continue;
            }
            auto req = Factory::CreateRequest();
            req->SetRequest(x,op,y);
            //进行序列化

            std::string request;
            req->Serialize(&request);
            std::string send_str= Encode(request);
            sock.Send(send_str);

            //接收响应
            while(true)
            {
                std::string recv_str;
                sock.Recv(&recv_str);
                if(recv_str.empty())
                {
                    continue;
                }

                std::string recv_str_decode=Decode(recv_str);
                auto resp = Factory::CreateResponse();
                resp->Deserialize(recv_str_decode);
                if(resp->getcode()==0)
                {
                    LOG(INFO,"Result: %d\n",resp->getresult());
                    break;
                }
                else
                {
                    LOG(ERROR,"Calculation failed: %s\n",resp->getdesc().c_str());
                    break;
                }  
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
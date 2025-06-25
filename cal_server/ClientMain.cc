#include <iostream>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#define sleep(x) Sleep((x) * 1000)
#define getpid() _getpid()
#else
#include <unistd.h>
#endif
#include "Socket.hpp"
#include "Protocol.hpp"

using namespace socket_ns;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server-ip server-port" << std::endl;
        exit(0);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    SockPtr sock = std::make_shared<TcpSocket>();
    if (!sock->BuildClientSocket(serverip, serverport))
    {
        std::cerr << "connect error" << std::endl;
        exit(1);
    }

    srand(time(nullptr) ^ getpid());
    const std::string opers = "+-*/";

    int cnt = 3;
    std::string packagestreamqueue;
    while (true)
    {
        // 构建数据
        int x = rand() % 10;
        int y = rand() % 10;
        char oper = opers[y % opers.size()];

        // 构建请求
        auto req = Factory::CreateRequest();
        req->SetRequest(x, oper, y);

        // 1. 序列化
        std::string reqstr;
        req->Serialize(&reqstr);

        // 2. 添加长度报头字段
        reqstr = Encode(reqstr);

        std::cout << "####################################" << std::endl;

        std::cout << "request string: \n" <<  reqstr << std::endl;

        // 3. 发送数据
        sock->Send(reqstr);

        while (true)
        {
            // 4. 读取应答，response
            int n = sock->Recv(&packagestreamqueue);
            if (n <= 0)
            {
                std::cerr << "Connection error or closed by server" << std::endl;
                break;
            }
            // 我们能保证我们读到的是一个完整的报文吗？不能！
            // 5. 报文解析，提取报头和有效载荷
            std::string package = Decode(packagestreamqueue);
            if (package.empty())
                continue;

            std::cout << "package: \n" << package << std::endl;
            
            // 6. 反序列化
            auto resp = Factory::CreateResponse();
            if (!resp->Deserialize(package)) {
                std::cerr << "Failed to deserialize response" << std::endl;
                continue;
            }

            // 7. 打印结果
            std::cout << "Response: " << resp->getresult() << std::endl;

            break;
        }

        sleep(1);
        // break;
    }

    sock->Close();
    return 0;
}
#include "Protocol.hpp"
#include "Socket.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if(argc!=3)
    {
        std::cout<<"Usage: "<<argv[0]<<" serverip serverport\n";
        return 0;
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    Socket *s = new TcpSocket();
    if(!s->BuildConnectSocketMethod(serverip,serverport))
    {
        std::cerr<<"connect "<<serverip<<":"<<serverport<<" failed\n";
    }
    std::cout<<"connect "<<serverip<<":"<<serverport<<" success\n";

    std::unique_ptr<Factory> factory = std::make_unique<Factory>();

    srand(time(nullptr)^getpid());
    const std::string opers = "+-*/%^=&";
    while(true)
    {
        int x = rand()%100;
        usleep(rand()%7777);
        int y = rand()%100;
        char oper = opers[rand()%opers.size()];
        std::shared_ptr<Request> req = factory->BuildRequest(x,y,oper);

        std::string requests;
        req->Serialize(&requests);
        std::string testreq=requests;
        requests = Encode(requests);
        s->Send(requests);

        while(true)
        {
            std::string responsestr;
            s->Recv(&responsestr,1024);

            std::string response;
            if(!Decode(responsestr,&response)) continue;
            auto resp = factory->BuildResponse();
            resp->Deserialize(response);

            std::cout<<testreq<<" = "<<resp->GetResult()<<" ["<<resp->GetCode()<<"]\n";
            break;
        }
        sleep(1);
    }

    s->CloseSocket();
    return 0;
}

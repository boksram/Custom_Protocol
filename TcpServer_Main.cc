#include <iostream>
#include <unistd.h>
#include <memory>
#include "Calculate.hpp"
#include "TcpServer.hpp"
#include "Protocol.hpp"


std::string HandlerRequest(std::string &inbufferstream,bool *error_code)
{
    *error_code = true;
    Calculate calculate;
    std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    auto req = factory->BuildRequest();
    std::string total_string;
    std::string message;
    if(!Decode(inbufferstream,&message))
    {
        *error_code = true;
        return " ";
    }
    if(!req->Deserialize(message))
    {
        *error_code = false;
        return " ";
    }
    auto resp = calculate.Cal(req);
    std::string send_string;
    resp->Serialize(&send_string);
    total_string=Encode(send_string);
    return total_string;
}

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        std::cout<<"Usage: "<<argv[0]<<" port\n";
        return 0;
    }

    uint16_t localport = std::stoi(argv[1]);
    std::unique_ptr<TcpServer> svr(new TcpServer(localport,HandlerRequest));
    svr->Loop();
    return 0;
}

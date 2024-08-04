#pragma once
#include <iostream>
#include <pthread.h>
#include <functional>
#include "Socket.hpp"

using func_t = std::function<std::string (std::string &,bool *error_code)>;

class TcpServer;

class ThreadData
{
public:
    ThreadData(TcpServer *tcp_this, Socket *psock)
    :_this(tcp_this),_psock(psock)
    {}
public:
    TcpServer *_this;
    Socket *_psock;
};

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t handler_request)
    :_port(port),_listensocket(new TcpSocket()),_handler_request(handler_request)
    {
        _listensocket->BuildListenSocketMethod(_port,backlog);
    }

    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData*>(args);
        std::string inbufferstream;
        while(true)
        {
            bool ok = true;
            if(!td->_psock->Recv(&inbufferstream,1024)) break;
            std::string send_string = td->_this->_handler_request(inbufferstream,&ok);
            if(ok)
            {
                if(!send_string.empty())
                {
                    td->_psock->Send(send_string);
                }
            }
            else
            {
                break;
            }
        }
        td->_psock->CloseSocket();
        delete td->_psock;
        delete td;
        return nullptr;
    }

    void Loop()
    {
        while(true)
        {
            std::string peerip;
            uint16_t peerport;
            Socket *newsock = _listensocket->AcceptConnection(&peerip,&peerport);
            if(!newsock) continue;
            std::cout<<"Get a new connection, sockfd: "<<newsock->GetSockfd()<<" client info: "<<peerip<<":"<<peerport<<std::endl;
            pthread_t tid;
            ThreadData *td = new ThreadData(this,newsock);
            pthread_create(&tid,nullptr,ThreadRun,td);
        }
    }
    ~TcpServer()
    {
        delete _listensocket;
    }
private:
    int _port;
    Socket *_listensocket;
public:
    func_t _handler_request;
};

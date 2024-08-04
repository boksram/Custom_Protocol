#pragma once
#include <iostream>
#include <memory>
#include "Protocol.hpp"

enum ErrCode
{
    Success=0,
    Divzero,
    Modzero,
    Unknow
};

class Calculate
{
    public:
        Calculate(){}
        std::shared_ptr<Response> Cal(std::shared_ptr<Request> req)
        {
            std::shared_ptr<Response> resp = factory.BuildResponse();
            resp->SetCode(Success);
            switch(req->GetOper())
            {
                case '+':
                    resp->SetResult(req->GetX()+req->GetY());
                    break;
                case '-':
                    resp->SetResult(req->GetX()-req->GetY());
                    break;
                case '*':
                    resp->SetResult(req->GetX()*req->GetY());
                    break;
                case '/':
                    {
                        if(req->GetY()==0)
                        {
                            resp->SetCode(Divzero);
                        }
                        else
                        {
                            resp->SetResult(req->GetX()/req->GetY());
                        }
                    }
                    break;
                case '%':
                    {
                        if(req->GetY()==0)
                        {
                            resp->SetCode(Modzero);
                        }
                        else
                        {
                            resp->SetResult(req->GetX()%req->GetY());
                        }
                    }
                    break;
                default:
                    resp->SetCode(Unknow);
                    break;
            }
            return resp;
        }
        ~Calculate(){}
    private:
        Factory factory;
};

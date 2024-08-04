#pragma once
#include <iostream>
#include <memory>
#include <json/json.h>

//#define SelfDefine 1

const std::string ProtSep = " ";
const std::string BlankSep = "\n";

std::string Encode(std::string &message)
{
    std::string len = std::to_string(message.size());
    std::string package = len+BlankSep+message+BlankSep;
    return package;
}

bool Decode(std::string &package,std::string *message)
{
    auto pos = package.find(BlankSep);
    if(pos==std::string::npos) return false;
    std::string lens = package.substr(0,pos);
    int message_len = std::stoi(lens);
    int total = lens.size()+message_len+2*BlankSep.size();
    if(package.size()<total) return false;
    *message = package.substr(pos+BlankSep.size(),message_len);
    package.erase(0,total);
    return true;
}

class Request
{
    public:
        Request()
        :_data_x(0),_data_y(0),_oper(0)
        {}
        Request(int x,int y,char op)
            :_data_x(x),_data_y(y),_oper(op)
        {}
        void Debug()
        {
            std::cout<<"_data_x: "<<_data_x<<std::endl;
            std::cout<<"_data_y: "<<_data_y<<std::endl;
            std::cout<<"_oper: "<<_oper<<std::endl;
        }
        void Inc()
        {
            _data_x++;
            _data_y++;
        }
        bool Serialize(std::string *out)
        {
        #ifdef SelfDefine
            *out = std::to_string(_data_x)+ ProtSep +_oper+ProtSep + std::to_string(_data_y);
            return true;
        #else
            Json::Value root;
            root["data_x"]=_data_x;
            root["data_y"]=_data_y;
            root["oper"]=_oper;
            Json::FastWriter writer;
            *out=writer.write(root);
            return true;
        #endif
        }
        bool Deserialize(std::string &in)
        {
        #ifdef SelfDefine
            auto left = in.find(ProtSep);
            if(left==std::string::npos) return false;
            auto right = in.rfind(ProtSep);
            if(right==std::string::npos) return false;
            _data_x=std::stoi(in.substr(0,left));
            _data_y=std::stoi(in.substr(right+ProtSep.size()));
            std::string oper=in.substr(left+ProtSep.size(),right-(left+ProtSep.size()));
            if(oper.size()!=1) return false;
            _oper=oper[0];
            return true;
        #else
            Json::Value root;
            Json::Reader reader;
            if(!reader.parse(in,root)) return false;
            _data_x=root["data_x"].asInt();
            _data_y=root["data_y"].asInt();
            _oper=root["oper"].asInt();
            return true;
        #endif
        }
        int GetX(){return _data_x;}
        int GetY(){return _data_y;}
        char GetOper(){return _oper;}
    private:
        int _data_x;
        int _data_y;
        char _oper;
};

class Response
{
    public:
        Response()
        :_result(0),_code(0)
        {}
        Response(int result, int code)
            :_result(result),_code(code)
        {}
        bool Serialize(std::string *out)
        {
        #ifdef SelfDefine
            *out = std::to_string(_result)+ProtSep+std::to_string(_code);
            return true;
        #else
            Json::Value root;
            root["result"]=_result;
            root["code"]=_code;
            Json::FastWriter writer;
            *out=writer.write(root);
            return true;
        #endif
        }
        bool Deserialize(std::string &in)
        {
        #ifdef SelfDefine
            auto pos = in.find(ProtSep);
            if(pos==std::string::npos) return false;
            _result=std::stoi(in.substr(0,pos));
            _code=std::stoi(in.substr(pos+ProtSep.size()));
            return true;
        #else
            Json::Value root;
            Json::Reader reader;
            if(!reader.parse(in,root)) return false;
            _result = root["result"].asInt();
            _code = root["code"].asInt();
            return true;
        #endif
        }
        void SetResult(int res){_result=res;}
        void SetCode(int code){_code=code;}
        int GetResult(){return _result;}
        int GetCode(){return _code;}
    private:
        int _result;
        int _code;
};

class Factory
{
    public:
        std::shared_ptr<Request> BuildRequest()
        {
            return std::make_shared<Request>();
        }

        std::shared_ptr<Request> BuildRequest(int x,int y,char op)
        {
            return std::make_shared<Request>(x,y,op);
        }

        std::shared_ptr<Response> BuildResponse()
        {
            return std::make_shared<Response>();
        }

        std::shared_ptr<Response> BuildResponse(int result,int code)
        {
            return std::make_shared<Response>(result,code);
        }
};

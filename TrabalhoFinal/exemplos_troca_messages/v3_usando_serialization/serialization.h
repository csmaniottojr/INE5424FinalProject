#ifndef serialization_h__
#define serialization_h__

#include "messages.h"

OStream cout;

class Serialization {
public:
    static const unsigned char BASE_SIZE = RegisterMessage::BASE_SIZE;
    static const unsigned char ID_SIZE = RegisterMessage::ID_SIZE;

    typedef RegisterMessage::Size Size;
    typedef RegisterMessage::ID ID;
    typedef RegisterMessage::Type Type;
public:
    static const char * serialize(RegisterMessage *message){
        int length = message->size();
        char * result = new char[length+1];
        memset(result, '\0', length+1);

        result[0] = message->size();
        memcpy(&result[1], message->id(), ID_SIZE);
        result[ID_SIZE+1] = message->type();

        switch(message->type()){
            case RegisterMessage::REGISTER_REQUEST:{
                break;
            }case RegisterMessage::REGISTER_RESPONSE:{
                RegisterResponse * res = reinterpret_cast<RegisterResponse*>(message);
                result[BASE_SIZE] = res->sendData();
                break;
            }case RegisterMessage::REGISTER_SERVICE_REQUEST:{
                RegisterServiceRequest * req = reinterpret_cast<RegisterServiceRequest*>(message);
                strncpy(&result[BASE_SIZE], req->serviceName(), strlen(req->serviceName()));
                break;
            }default:
                break;
        }
        return result;
    }

    static RegisterMessage* deserialize(const char * msg){
        RegisterMessage *result = 0;

        Size size = (Size) msg[0];
        ID id;
        memcpy(id, &msg[1], ID_SIZE);
        Type type = (Type) msg[ID_SIZE+1];

        switch(type){
            case RegisterMessage::REGISTER_REQUEST:{
                RegisterRequest *req = new RegisterRequest();
                result = req;
                break;
            }case RegisterMessage::REGISTER_RESPONSE:{
                RegisterResponse *res = new RegisterResponse();

                res->setSendData((bool) msg[BASE_SIZE]);
                result = res;
                break;
            }case RegisterMessage::REGISTER_SERVICE_REQUEST:{
                RegisterServiceRequest *req = new RegisterServiceRequest();
                
                unsigned char length = (size - BASE_SIZE);
                req->setServiceName(&msg[BASE_SIZE], length);
                result = req;
                break;
            }default:
                break;
        }
        if(result != 0){
            result->setSize(size);
            result->setId(id);
            result->setType(type);
        }
        return result;
    }
};

#endif
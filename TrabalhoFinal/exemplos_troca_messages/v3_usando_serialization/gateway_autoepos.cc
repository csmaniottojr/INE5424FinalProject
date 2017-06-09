#include "messages.h"
#include "serialization.h"
#include "cheats/led.h"
#include <machine.h>

using namespace EPOS;
//OStream cout;

class Receiver : public IEEE802_15_4::Observer
{
public:
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;

    typedef unsigned char STATE;
    enum {
        WAITING_REGISTER = 0,
        WAITING_SERVICE = 1,
        WAITING_PARAMETER = 2,
        WAITING_COMBO = 3
    };

    Receiver(const Protocol & p, NIC * nic) 
    : _prot(p), _nic(nic), _state(WAITING_REGISTER) {
        _nic->attach(this, _prot);
    }

    void update(Observed * o, Protocol p, Buffer * b) {
        eMoteIII::led::blink(0.1, 1);
        auto msg = b->frame()->data<char>();

        cout << "Received " << b->size() << " bytes of payload from " << b->frame()->src() << " :";
        for(int i=0; i<b->size(); i++)
            cout << " " << hex << (unsigned char) msg[i];
        cout << endl;

        RegisterMessage * message = Serialization::deserialize(msg);

        if(message->type() == RegisterMessage::REGISTER_REQUEST){
            cout << "   Type: RegisterRequest" << endl;
            cout << "   Recebido do ID:";
            for(unsigned int i = 0; i<RegisterMessage::ID_SIZE; i++)
                cout << " " << hex << message->id()[i];
            cout << endl;

            Machine::delay(1000000);//verifica no banco de dados
            cout << "   Mandando ACK!" << endl;
            _rResp.setId(message->id());
            _rResp.setSendData(true);
            _nic->send(b->frame()->src(), p, Serialization::serialize(&_rResp), _rResp.size()+2);
        }else if(message->type() == RegisterMessage::REGISTER_SERVICE_REQUEST){
            cout << "   Type: RegisterServiceRequest" << endl;

            RegisterServiceRequest * request = reinterpret_cast<RegisterServiceRequest*>(message);

            cout << "   Name: " << request->serviceName() << endl;
            cout << "   Mandando ACK!" << endl;
            _rsResp.setId(message->id());
            _nic->send(b->frame()->src(), p, Serialization::serialize(&_rsResp), _rsResp.size()+2);
        }
        cout << endl;

        _nic->free(b);
        if(message != 0)
            delete message;
    }

private:
    RegisterResponse _rResp;
    RegisterServiceResponse _rsResp;

    Protocol _prot;
    NIC * _nic;

    RegisterMessage::ID _currentId;
    STATE _state;
};

int main(){
    eMoteIII::led::blink(0.1, 1);
    cout << "Machine::id() = ";
    for(unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl;

    NIC * nic = new NIC();
    Receiver r(IEEE802_15_4::ELP, nic);

    cout << "Gateway esta esperando requests e esta enviando responses!" << endl << endl;

    while(1);
}
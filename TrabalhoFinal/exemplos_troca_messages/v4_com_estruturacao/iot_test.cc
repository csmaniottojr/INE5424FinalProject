#include "cheats/led.h"
#include <machine.h>
#include <alarm.h>
#include "IoT/objects/smartobject.h"
#include "IoT/objects/parameter_boolean.h"
#include "IoT/objects/parameter_float.h"
#include "IoT/iot_manager.h"

using namespace EPOS;

OStream cout;

//IDs: 101560330 e 101559240
int main(){
    IoT::SmartObject object("Epos");
    
    IoT::Service atuacao("Atuacao");
        object.addService(&atuacao);
    IoT::Service sensores("Sensores");
        object.addService(&sensores);
    
    IoT::ParameterBoolean _led;
    IoT::Parameter led("Led", 1, &_led);
        atuacao.addParameter(&led);
    IoT::ParameterCombo _combo;//soh pra testar...
        _combo.addOption("Baixa");
        _combo.addOption("Media");
        _combo.addOption("Alta");
    IoT::Parameter temperatura1("Temperatura", 2, &_combo);
        atuacao.addParameter(&temperatura1);
    IoT::ParameterFloat _temp(1, 2);
    IoT::Parameter temperatura2("Temperatura", 3, &_temp);
        sensores.addParameter(&temperatura2);
    
    NIC * nic = new NIC();
    IoT::IotManager manager(IEEE802_15_4::ELP, nic, &object);

    while(1){
        eMoteIII::led::blink(0.1, 2);

        cout << endl;//start print

        cout << "# Obj ID: " << object.getId() << endl;
        cout << "# Obj Name: " << object.getName() << endl;
        auto services = object.getServices();
        for(auto e = services->head(); e; e = e->next()){
            cout << "#   Service Name: " << e->object()->getName() << endl;

            auto params = e->object()->getParameters();
            for(auto e2 = params->head(); e2; e2 = e2->next()){
                cout << "#      Param Name: " << e2->object()->getName() << endl;
                cout << "#      Param RegId: " << e2->object()->getRegisterId() << endl;
                auto max = e2->object()->getMaxValue();
                cout << "#      Param maxValue: ";
                for(int i = 1; i<=((unsigned char)max[0]); i++)
                    cout << (unsigned char) max[i] << " ";
                cout << endl;
            }
        }

        manager.run();

        cout << endl;//end print
        Alarm::delay(10000000);
    }
}
#include <iostream>
#include "ApplicationClass.h"
#include "HeadClass.h"
#include "InputData.h"
#include "OutputData.h"
#include "Vehicle.h"
#include "TrafficLight.h"
#include "ControlPanel.h"
#include "Road.h"


using namespace std;

ApplicationClass::ApplicationClass(HeadClass *ptr) : HeadClass(ptr) {}

void ApplicationClass::signal(string &message) {

}

void ApplicationClass::handler(string message) {
}

void ApplicationClass::buildTree() {
    // создадим объекты ввода, вывода, пульта светофора, самого светофора и дороги
    HeadClass *controlPanelPointer = new ControlPanel(this, "Control panel");
    HeadClass *trafficLightPointer = new TrafficLight(controlPanelPointer, "Traffic light");
    HeadClass *inputPointer = new InputData(this, "Input panel");
    HeadClass *outputPointer = new OutputData(this, "Output panel");
    HeadClass *roadPointer = new Road(this, "Crossroad");

    // Установка связи между объектом текущего класса и класса, отвечающего за ввод
    //setConnection(SIGNAL_D(ApplicationClass::signal),
    //				inputPointer, HANDLER_D(InputData::handler));

    //setConnection(SIGNAL_D(InputData::signal),
    //				this, HANDLER_D(ApplicationClass::handler));
    // Установка связи между объектом ввода и 1м обработчиком дороги (initial road condition)
    inputPointer->setConnection(SIGNAL_D(InputData::signal),
                                roadPointer, HANDLER_D(Road::handlerFromInput));

    string command;
    do {
        //emitSignal(SIGNAL_D(ApplicationClass::signal), command);
        inputPointer->emitSignal(SIGNAL_D(InputData::signal), command);
    } while (command != "End of cars");

    // Удаление связи между дорогой и вводом машин
    //deleteConnection(SIGNAL_D(InputData::signal),
    //				roadPointer, HANDLER_D(Road::handlerFromInput));
}

int ApplicationClass::launchApp() {
    cout << "Ready to work";

    string command;

    HeadClass *controlPanelPointer = getChildPointer("Control panel");
    HeadClass *trafficLightPointer = controlPanelPointer->getChildPointer("Traffic light");
    HeadClass *inputPointer = getChildPointer("Input panel");
    HeadClass *outputPointer = getChildPointer("Output panel");
    HeadClass *roadPointer = getChildPointer("Crossroad");


    // Установка связи между объектом ввода и панелью управления
    inputPointer->setConnection(SIGNAL_D(InputData::signal),
                                controlPanelPointer, HANDLER_D(ControlPanel::handlerFromInput));

    // Установка связи между объектом ввода и 2м обработчиком дороги (для приема команд)
    inputPointer->setConnection(SIGNAL_D(InputData::signal),
                                roadPointer, HANDLER_D(Road::handlerFromInput2));


    // Установка связи между объектом светофора и 3м обработчиком дороги (для приема цвета светофора)
    trafficLightPointer->setConnection(SIGNAL_D(TrafficLight::signalToRoad),
                                       roadPointer, HANDLER_D(Road::handlerFromTrafficLight));


    // Установка связи между объектом светофора и вывода
    trafficLightPointer->setConnection(SIGNAL_D(TrafficLight::signalToOutput),
                                       outputPointer, HANDLER_D(OutputData::handler));


    // Установка связи между объектом дороги и вывода
    roadPointer->setConnection(SIGNAL_D(Road::signalToOutput),
                               outputPointer, HANDLER_D(OutputData::handler));

    // Установка связи между панелью управления и светофором
    controlPanelPointer->setConnection(SIGNAL_D(ControlPanel::signal),
                                       trafficLightPointer, HANDLER_D(TrafficLight::handlerFromRoad));


    string defaultCommand = "color -1";


    inputPointer->emitSignal(SIGNAL_D(InputData::signal), command);

    do {
        controlPanelPointer->emitSignal(SIGNAL_D(ControlPanel::signal), defaultCommand);
        inputPointer->emitSignal(SIGNAL_D(InputData::signal), command);
    } while (command != "Turn off the system");

    cout << endl << "Turn off the system";

    return 0;
}
#ifndef __TRAFFICLIGHT__H
#define __TRAFFICLIGHT__H

#include "HeadClass.h"

class TrafficLight : public HeadClass {
    int nTacts = 0;
    int greenDuration = 5;
    int redDuration = 5;
    int yellowDuration = 2;

    string currentColor = "green";
    string previousColor = "yellow";
public:
    TrafficLight(HeadClass* parentPointer, string newObjectName);

    void signalToRoad(string& message); // метод сигнала для дороги
    void signalToOutput(string& message); // метод сигнала для объекта вывода
    void handlerFromRoad(string message); // метод обработчика для дороги

    void changeColorDuration(string color, int duration = -1);

};


#endif

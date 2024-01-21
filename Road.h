#ifndef __ROAD__H
#define __ROAD__H

#include "HeadClass.h"

class Road : public HeadClass {
    int size;
    vector<HeadClass*> right;
    vector<HeadClass*> left;
    vector<HeadClass*> up;
    vector<HeadClass*> down;
public:
    Road(HeadClass* parentPointer, string newObjectName);

    void signalToOutput(string&); // метод сигнала объекту вывода
    void handlerFromInput(string); // метод обработчика вводимых в начале машин
    void handlerFromInput2(string); // метод обработчика вводимых данных во время самого движения
    void handlerFromTrafficLight(string); // метод обработчика данных от светофора

    string getCarPosition(string carNumber);

};

#endif

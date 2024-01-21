#ifndef __INPUTDATA__H
#define __INPUTDATA__H

#include "HeadClass.h"

class InputData : public HeadClass {
public:
    InputData(HeadClass* parentPointer, string newObjectName);

    void signal(string&); // метод сигнала
    void handler(string); // метод обработчика сигнала от объекта класса ApplicationClass
};

#endif

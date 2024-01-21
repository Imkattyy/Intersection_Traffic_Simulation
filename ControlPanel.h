#ifndef __CONTROLPANEL__H
#define __CONTROLPANEL__H

#include "HeadClass.h"

class ControlPanel : public HeadClass {
public:
    ControlPanel(HeadClass* parentPointer, string newObjectName);
    void signal(string&); // метод сигнала
    void handlerFromInput(string); // метод обработчика от объекта ввода
};

#endif

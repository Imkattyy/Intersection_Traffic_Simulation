#ifndef __OUTPUTDATA__H
#define __OUTPUTDATA__H

#include "HeadClass.h"

class OutputData : public HeadClass {
public:
    OutputData(HeadClass* parentPointer, string newObjectName);

    void handler(string message); // метод обработчика
};

#endif

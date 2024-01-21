#ifndef __VEHICLE__H
#define __VEHICLE__H

#include "HeadClass.h"

class Vehicle : public HeadClass {
public:
    Vehicle(HeadClass* parentPointer, string newObjectName);
};

#endif

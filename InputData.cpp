#include "InputData.h"

InputData::InputData(HeadClass* parentPointer, string newObjectName) : HeadClass(parentPointer, newObjectName) {}

void InputData::handler(string message)
{
}

void InputData::signal(string &message)
{
    getline(cin, message);
}
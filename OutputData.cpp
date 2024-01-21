#include "OutputData.h"
#include "ApplicationClass.h"

OutputData::OutputData(HeadClass* parentPointer, string newObjectName) : HeadClass(parentPointer, newObjectName) {}

void OutputData::handler(string message)
{
    cout << endl << message;
}
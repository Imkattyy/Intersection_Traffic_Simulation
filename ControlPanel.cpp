#include "ControlPanel.h"

ControlPanel::ControlPanel(HeadClass* parentPointer, string newObjectName) : HeadClass(parentPointer, newObjectName) {}

void ControlPanel::signal(string& message) {}

void ControlPanel::handlerFromInput(string message)
{
    if (message == "Turn off the system")
        return;

    // Проверим кол-во слов в команде
    string curWord;
    vector<string> words;
    for (int i = 0; i < message.size(); i++)
    {
        if (message[i] == ' ')
        {
            words.push_back(curWord);
            curWord = "";
        }
        else
            curWord += message[i];

        if (i == message.size() - 1)
            words.push_back(curWord);
    }

    if (words.size() == 0 || words[0] == "Car")
        return;

    if (words[0] == "Switching")
    {
        HeadClass* trafficLightPointer = getChildPointer("Traffic light");
        string newMessage = words[3] +  " " + words[4];
        emitSignal(SIGNAL_D(ControlPanel::signal), newMessage); // посылаем сигнал панели контроля
    }
    else if (words[0] == "Display")
        emitSignal(SIGNAL_D(ControlPanel::signal), words[0]); // посылаем сигнал светофору
}
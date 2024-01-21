#include "TrafficLight.h"

TrafficLight::TrafficLight(HeadClass* parentPointer, string newObjectName) : HeadClass(parentPointer, newObjectName) {}

void TrafficLight::signalToRoad(string& message) {}

void TrafficLight::changeColorDuration(string color, int duration)
{
    if (duration == -1)
    {
        if (currentColor == "green" && nTacts >= greenDuration)
        {
            currentColor = "yellow";
            previousColor = "green";
            nTacts = 0;
        }
        else if (currentColor == "yellow" && nTacts >= yellowDuration)
        {
            if (previousColor == "green")
                currentColor = "red";
            if (previousColor == "red")
                currentColor = "green";

            previousColor = "yellow";
            nTacts = 0;
        }
        else if (currentColor == "red" && nTacts >= redDuration)
        {
            currentColor = "yellow";
            previousColor = "red";
            nTacts = 0;
        }

        emitSignal(SIGNAL_D(TrafficLight::signalToRoad), currentColor);
        nTacts++;
    }
    else
    {
        if (color == "yellow" || duration < 2) // если изменения не соответствуют условиям
            return;

        if (color == "green")
            greenDuration = duration;
        else if (color == "red")
            redDuration = duration;
    }
}


void TrafficLight::signalToOutput(string& message)
{
    message = "\nTraffic light color is " + message;
}

void TrafficLight::handlerFromRoad(string message)
{
    if (message == "Display")
    {
        string color = currentColor;
        emitSignal(SIGNAL_D(TrafficLight::signalToOutput), color);
        return;
    }

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

    int newTime = stoi(words[1]);
    changeColorDuration(words[0], newTime);
}
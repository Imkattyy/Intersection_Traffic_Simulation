#include "Road.h"
#include "Vehicle.h"
#include <algorithm>
#include <sstream>
#include <stdbool.h>
#include <iostream>

Road::Road(HeadClass* parentPointer, string newObjectName) : HeadClass(parentPointer, newObjectName)
{

}

void Road::signalToOutput(string& message)
{
    message = '\n' + message;
}

void Road::handlerFromInput(string message)
{
    if (message == "End of cars")
        return;

    // Проверим кол-во слов в команде
    vector<int> wordsLen; // символов в каждом слове (по длине можно чекнуть кол-во слов)
    int nWordLen = 0;
    for (int i = 0; i < message.size(); i++)
    {
        if (message[i] == ' ')
        {
            wordsLen.push_back(nWordLen);
            nWordLen = 0;
        }
        else
            nWordLen++;

        if (i == message.size() - 1)
            wordsLen.push_back(nWordLen);
    }

    int roadSize;
    if (wordsLen.size() == 1)
    {
        roadSize = stoi(message);
        left.resize(2 * roadSize, nullptr);
        right.resize(2 * roadSize, nullptr);
        up.resize(2 * roadSize, nullptr);
        down.resize(2 * roadSize, nullptr);
    }
    else if (wordsLen.size() == 3)
    {
        string carNumber = message.substr(0, wordsLen[0]);
        message.erase(message.begin(), message.begin() + wordsLen[0] + 1);

        int x = stoi(message.substr(0, wordsLen[1]));
        message.erase(message.begin(), message.begin() + wordsLen[1] + 1);

        int y = stoi(message);

        // Определяем положение машины
        roadSize = up.size();
        if (x == 1) {
            if (y > 1)
                up[roadSize / 2 - y] = new Vehicle(this, carNumber);
            else
                up[-y + roadSize / 2 - 1] = new Vehicle(this, carNumber);
        }
        else if (x == -1)
        {
            if (y > 1)
                down[y + roadSize / 2 - 1] = new Vehicle(this, carNumber);
            else
                down[y + roadSize / 2] = new Vehicle(this, carNumber);
        }
        else if (y == 1)
        {
            if (x > 1)
                left[x + roadSize / 2 - 1] = new Vehicle(this, carNumber);
            else
                left[x + roadSize / 2] = new Vehicle(this, carNumber);
        }
        else if (y == -1)
        {
            if (x > 1)
                right[roadSize / 2 - x] = new Vehicle(this, carNumber);
            else
                right[-x + roadSize / 2 - 1] = new Vehicle(this, carNumber);
        }
    }
}

string Road::getCarPosition(string carNumber)
{
    string position;
    HeadClass* vehiclePointer = getChildPointer(carNumber);
    if (vehiclePointer != nullptr)
    {
        string vehicleName = vehiclePointer->getObjectName();

        for (int i = 0; i < left.size(); i++)
        {
            if (left[i] == vehiclePointer)
            {
                position = vehicleName;
                if (i < left.size() / 2)
                    position += " ( " + to_string((int)(i - left.size() / 2)) + ", 1 )";
                else
                    position += " ( " + to_string((int)(i - left.size() / 2 + 1)) + ", 1 )";

                return position;
            }
        }

        for (int i = 0; i < right.size(); i++)
        {
            if (right[i] == vehiclePointer)
            {
                position = vehicleName;
                if (i < right.size() / 2)
                    position += " ( " + to_string((int)(right.size() / 2 - i)) + ", -1 )";
                else
                    position += " ( " + to_string((int)right.size() / 2 - i - 1) + ", -1 )";

                return position;
            }
        }

        for (int i = 0; i < down.size(); i++)
        {
            if (down[i] == vehiclePointer)
            {
                position = vehicleName;
                if (i < down.size() / 2)
                    position += " ( -1, " + to_string((int)(i - down.size() / 2)) + " )";
                else
                    position += " ( -1, " + to_string((int)(i - down.size() / 2 + 1)) + " )";

                return position;
            }
        }
        for (int i = 0; i < up.size(); i++)
        {
            if (up[i] == vehiclePointer)
            {
                position = vehicleName;
                if (i < up.size() / 2)
                    position += " ( 1, " + to_string((int)(up.size() / 2 - i)) + " )";
                else
                    position += " ( 1, " + to_string(((int)up.size() / 2 - i - 1)) + " )";

                return position;
            }

        }
    }

    return "";
}

bool sortVehicles(HeadClass* a, HeadClass* b)
{
    return a->getObjectName() < b->getObjectName();
}

void Road::handlerFromInput2(string message)
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


    if (words.size() == 2 && words[0] == "Car")
    {
        string position = getCarPosition(words[1]);
        string vehicleCoordinate;
        if (position == "")
            vehicleCoordinate = words[1] + " the car left the road section";
        else
            vehicleCoordinate = "Car " + position;
        emitSignal(SIGNAL_D(Road::signalToOutput), vehicleCoordinate);
    }
    else if (words.size() == 0 || words[0] == "Switching")
    {
        return;
    }
    else if (message == "Display the system status")
    {
        vector<HeadClass*> subordinateObjects = getChildren();
        sort(subordinateObjects.begin(), subordinateObjects.end(), sortVehicles);
        for (auto child : subordinateObjects)
        {
            string carPosition = getCarPosition(child->getObjectName());
            emitSignal(SIGNAL_D(Road::signalToOutput), carPosition);
        }
    }

}

void Road::handlerFromTrafficLight(string message)
{
    HeadClass* currentCarPointer;
    int freeSquare = left.size();
    if (message == "green")
    {
        if (left[0] != nullptr)
        {
            currentCarPointer = left[0];
            left[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        left.erase(left.begin());
        left.push_back(nullptr);

        if (right[0] != nullptr)
        {
            currentCarPointer = right[0];
            right[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        right.erase(right.begin());
        right.push_back(nullptr);

        // Проезжают машины вниз, которые преодолели перекресток
        if (down[0] != nullptr)
        {
            currentCarPointer = down[0];
            down[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        for (int i = 0; i < down.size() / 2 - 2; i++)
            down[i] = down[i + 1];
        down[down.size() / 2 - 2] = nullptr;

        // Проезжают машины, которые не преодолели перекресток, но еще не перед светофором
        freeSquare = left.size();
        for (int i = down.size() / 2 + 1; i < down.size(); i++)
        {
            if (down[i] == nullptr && i < freeSquare)
                freeSquare = i;
        }

        if (freeSquare < down.size() - 1)
        {
            for (int i = freeSquare; i < down.size() - 1; i++)
                down[i] = down[i + 1];
            down[down.size() - 1] = nullptr;
        }


        // Проезжают машины вверх, которые преодолели перекресток
        if (up[0] != nullptr)
        {
            currentCarPointer = up[0];
            up[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        for (int i = 0; i < up.size() / 2 - 2; i++)
            up[i] = up[i + 1];
        up[up.size() / 2 - 2] = nullptr;

        // Проезжают машины, которые не преодолели перекресток, но еще не перед светофором
        freeSquare = left.size();
        for (int i = up.size() / 2 + 1; i < up.size(); i++)
        {
            if (up[i] == nullptr && i < freeSquare)
                freeSquare = i;
        }

        if (freeSquare < up.size() - 1)
        {
            for (int j = freeSquare; j < up.size() - 1; j++)
                up[j] = up[j + 1];
            up[up.size() - 1] = nullptr;
        }

    }

    if (message == "red")
    {
        if (down[0] != nullptr)
        {
            currentCarPointer = down[0];
            down[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        down.erase(down.begin());
        down.push_back(nullptr);

        if (up[0] != nullptr)
        {
            currentCarPointer = up[0];
            up[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        up.erase(up.begin());
        up.push_back(nullptr);

        // Проезжают стоящие после светофора влево
        if (left[0] != nullptr)
        {
            currentCarPointer = left[0];
            left[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        for (int i = 0; i < left.size() - 2; i++)
            left[i] = left[i + 1];
        left[left.size() - 2] = nullptr;

        // и вправо
        if (right[0] != nullptr)
        {
            currentCarPointer = right[0];
            right[0] = nullptr;
            deleteObject(currentCarPointer->getObjectName());
        }

        for (int i = 0; i < right.size() - 2; i++)
            right[i] = right[i + 1];
        right[right.size() - 2] = nullptr;

        // Проезжают машины, стоящие до светофора влево
        freeSquare = left.size();
        for (int i = left.size() / 2 + 1; i < left.size(); i++)
        {
            if (left[i] == nullptr && i < freeSquare)
                freeSquare = i;
        }

        if (freeSquare < left.size() - 1)
        {
            for (int i = freeSquare; i < left.size() - 1; i++)
                left[i] = left[i + 1];
            left[left.size() - 1] = nullptr;
        }

        // и вправо
        freeSquare = left.size();
        for (int i = right.size() / 2 + 1; i < right.size(); i++)
        {
            if (right[i] == nullptr && i < freeSquare)
                freeSquare = i;
        }

        if (freeSquare < right.size() - 1)
        {
            for (int i = freeSquare; i < right.size() - 1; i++)
                right[i] = right[i + 1];
            right[right.size() - 1] = nullptr;
        }
    }

    if (message == "yellow")
    {

        vector<vector<HeadClass*>*> directions = {&left, &right, &up, &down};

        for (int i = 0; i < directions.size(); i++)
        {
            freeSquare = directions[i]->size();
            if ((*directions[i])[0] != nullptr)
            {
                currentCarPointer = (*directions[i])[0];
                (*directions[i])[0] = nullptr;
                deleteObject(currentCarPointer->getObjectName());
            }

            for (int j = 0; j < directions[i]->size() / 2; j++)
                (*directions[i])[j] = (*directions[i])[j + 1];
            (*directions[i])[directions[i]->size() / 2] = nullptr;

            for (int k = directions[i]->size() / 2 + 1; k < directions[i]->size(); k++)
            {
                if ((*directions[i])[k] == nullptr && k < freeSquare)
                    freeSquare = i;
            }

            if (freeSquare < directions[i]->size() - 1)
            {
                for (int f = freeSquare; f < directions[i]->size() - 1; f++)
                    (*directions[i])[f] = (*directions[i])[f + 1];
                (*directions[i])[directions[i]->size() - 1] = nullptr;
            }
        }
    }
}


/*
// ситуация у едущих влево
if (left[0] != nullptr)
{
    currentCarPointer = left[0];
    left[0] = nullptr;
    deleteObject(currentCarPointer->getObjectName());
}

for (int i = 0; i < left.size() / 2; i++)
    left[i] = left[i + 1];
left[left.size() / 2] = nullptr;

freeSquare = left.size();
for (int i = left.size() / 2 + 1; i < left.size(); i++)
{
    if (left[i] == nullptr)
        if (i < freeSquare)
            freeSquare = i;
}

if (freeSquare < left.size() - 1)
{
    for (int i = freeSquare; i < left.size() - 1; i++)
        left[i] = left[i + 1];
    left[left.size() - 1] = nullptr;
}

// ситуация у едущих вправо
if (right[0] != nullptr)
{
    currentCarPointer = right[0];
    right[0] = nullptr;
    deleteObject(currentCarPointer->getObjectName());
}

for (int i = 0; i < right.size() / 2; i++)
    right[i] = right[i + 1];
right[right.size() / 2] = nullptr;

freeSquare = left.size();
for (int i = right.size() / 2 + 1; i < right.size(); i++)
{
    if (right[i] == nullptr)
        if (i < freeSquare)
            freeSquare = i;
}

if (freeSquare < right.size() - 1)
{
    for (int i = freeSquare; i < right.size() - 1; i++)
        right[i] = right[i + 1];
    right[right.size() - 1] = nullptr;
}

// ситуация у едущих вниз
if (down[0] != nullptr)
{
    currentCarPointer = down[0];
    down[0] = nullptr;
    deleteObject(currentCarPointer->getObjectName());
}

for (int i = 0; i < down.size() / 2; i++)
    down[i] = down[i + 1];
down[down.size() / 2] = nullptr;

freeSquare = left.size();
for (int i = down.size() / 2 + 1; i < down.size(); i++)
{
    if (down[i] == nullptr)
        if (i < freeSquare)
            freeSquare = i;
}

if (freeSquare < down.size() - 1)
{
    for (int i = freeSquare; i < down.size() - 1; i++)
        down[i] = down[i + 1];
    down[down.size() - 1] = nullptr;
}

// ситуация у едущих вверх
if (up[0] != nullptr)
{
    currentCarPointer = up[0];
    up[0] = nullptr;
    deleteObject(currentCarPointer->getObjectName());
}

for (int i = 0; i < up.size() / 2; i++)
    up[i] = up[i + 1];
up[up.size() / 2] = nullptr;

freeSquare = left.size();
for (int i = up.size() / 2 + 1; i < up.size(); i++)
{
    if (up[i] == nullptr)
        if (i < freeSquare)
            freeSquare = i;
}

if (freeSquare < up.size() - 1)
{
    for (int i = freeSquare; i < up.size() - 1; i++)
        up[i] = up[i + 1];
    up[up.size() - 1] = nullptr;
}
}
}
*/

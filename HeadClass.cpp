#include <iostream>
#include <vector>
#include "HeadClass.h"

using namespace std;

HeadClass::HeadClass(HeadClass* parentPointer, string newObjectName)
{
    this->objectName = newObjectName; // наименование нового объекта
    this->parentPointer = parentPointer; // указатель на головной объект

    if (parentPointer)
        parentPointer->children.push_back(this); // добавление подчиненных в состав головного объекта
}

void HeadClass::setClassNumber(int classNumber)
{
    this->classNumber = classNumber;
}

void HeadClass::setReadyMark(int isReady)
{
    if (parentPointer != nullptr && parentPointer->readyMark == 0 || isReady == 0) {
        readyMark = 0;
        for (auto & child : children)
            child->setReadyMark(0);
    } else {
        readyMark = isReady;
    }
}

void HeadClass::setDefaultReadyMark()
{
    if (parentPointer == nullptr || parentPointer != nullptr && parentPointer->readyMark != 0)
    {
        readyMark = 1;
        for (auto child : children)
            child->setDefaultReadyMark();
    }
}

bool HeadClass::renameObject(string newName)
{
    if (parentPointer == nullptr) // если это головной объект
        objectName = newName;
    else if (parentPointer->getChildPointer(newName) == nullptr) // проверяем, нет ли уже такого имени
        objectName = newName; // новое наименование объекта

    return objectName == newName;
}

bool HeadClass::changeParent(HeadClass* newParentPointer)
{
    // хотим переопределить родителя для головного объекта
    if (parentPointer == nullptr)
        return false;

    // хотим создать еще один корневой или объекта попросту нет на дереве
    if (newParentPointer == nullptr)
        return false;

    // у нового родителя уже есть ребенок с таким именем
    if (newParentPointer->getChildPointer(objectName) != nullptr)
        return false;

    // если объект найден на ветке у текущего
    HeadClass* parentPointerExistOnBranch = newParentPointer->parentPointer;
    while (parentPointerExistOnBranch != nullptr)
    {
        if (parentPointerExistOnBranch == this)
            return false;
        else
            parentPointerExistOnBranch = parentPointerExistOnBranch->parentPointer;
    }

    HeadClass* pointerToRemember = this;
    for (int i = 0; i < parentPointer->children.size(); i++)
        if (parentPointer->children[i]->objectName == objectName)
            parentPointer->children.erase(parentPointer->children.begin() + i);
    newParentPointer->children.push_back(pointerToRemember);

    return true;
}

int HeadClass::getClassNumber()
{
    return classNumber;
}

string HeadClass::getObjectName()
{
    return objectName;
}

HeadClass* HeadClass::getHeadObjectPointer()
{
    return parentPointer;
}

string HeadClass::getAbsoluteCoordinate()
{
    if (parentPointer == nullptr)
        return "/";

    string absoluteLink;
    HeadClass* curObjectPointer = this;

    while (curObjectPointer->parentPointer != nullptr)
    {
        absoluteLink = "/" + curObjectPointer->objectName + absoluteLink;
        curObjectPointer = curObjectPointer->parentPointer;
    }

    return absoluteLink;
}

vector<HeadClass*> HeadClass::getChildren()
{
    return children;
}

HeadClass* HeadClass::getChildPointer(string childName)
{
    for (auto & child : children) { // смотрим всех детей
        if (childName == child->objectName) // проверяем, есть ли такое имя в списке детей
            return child;
    }

    return nullptr;
}

HeadClass* HeadClass::getAnyPointer(string coordinate)
{
    unsigned int coordinateLen = coordinate.size();

    // если ссылки нет
    if (coordinateLen == 0)
        return nullptr;

    // если это корневой или текущий объект
    if (coordinateLen == 1)
    {
        if (coordinate[0] == '/')
        {
            HeadClass* toFindRootPointer = this;
            while (toFindRootPointer->parentPointer != nullptr)
                toFindRootPointer = toFindRootPointer->parentPointer;
            return toFindRootPointer;
        }
        else if (coordinate[0] == '.')
            return this;
        else
            return nullptr;
    }

    string curObjectName;

    // объект уникален на ветке
    if (coordinate[0] == '.')
    {
        for (int i = 1; i < coordinateLen; i++)
            curObjectName += coordinate[i];
        return findObjectOnBranch(curObjectName);
    }

    // объект уникален на дереве
    if (coordinateLen > 2 && coordinate[0] == '/' && coordinate[1] == '/')
    {
        for (int i = 2; i < coordinateLen; i++)
            curObjectName += coordinate[i];
        return findObjectOnTree(curObjectName);
    }

    // ссылка от текущего объекта
    HeadClass* firstObject;
    if (coordinate[0] != '/' && coordinate[0] != '.')
    {
        firstObject = this;
        for (int i = 0; i < coordinateLen; i++)
        {
            if (coordinate[i] != '/') {
                curObjectName += coordinate[i];
                if (i == coordinateLen - 1)
                    return firstObject->getChildPointer(curObjectName);
            } else {
                firstObject = firstObject->getChildPointer(curObjectName);;
                if (firstObject == nullptr)
                    return nullptr;
                else
                    curObjectName = "";
            }
        }
    }

    // абсолютная ссылка
    if (coordinate[0] == '/')
    {
        firstObject = getAnyPointer("/");
        for (int i = 1; i < coordinateLen; i++)
        {
            if (coordinate[i] != '/')
            {
                curObjectName += coordinate[i];
                if (i == coordinateLen - 1)
                    return firstObject->getChildPointer(curObjectName);
            } else {
                firstObject = firstObject->getChildPointer(curObjectName);
                if (firstObject == nullptr)
                    return nullptr;
                else
                    curObjectName = "";
            }
        }
    }

    return nullptr;
}

HeadClass* HeadClass::findObjectOnBranch(string nameToFind)
{
    int nCopies = 0;
    HeadClass* objectWeNeed = nullptr;
    queue <HeadClass*> objects;
    objects.push(this);
    while (!objects.empty())
    {
        for (auto child : objects.front()->children)
            objects.push(child);
        if (objects.front()->objectName == nameToFind)
            nCopies++;
        objects.pop();
    }

    if (nCopies == 1)
        return objectWeNeed;
    else
        return nullptr;

}

HeadClass* HeadClass::findObjectOnTree(string nameToFind)
{
    if (parentPointer == nullptr)
        return findObjectOnBranch(nameToFind);

    return parentPointer->findObjectOnTree(nameToFind);
}

void HeadClass::outputObjectTree(int level)
{
    cout << objectName;
    for (auto & child : children) {
        cout << endl;
        for (int i = 0; i < level; i++)
            cout << "    ";

        child->outputObjectTree(level + 1);
    }
}

void HeadClass::outputAllSystem(int level)
{
    if (level == 1)
        cout << endl << "The tree of objects and their readiness" << endl;

    cout << objectName;
    if (readyMark == 0)
        cout << " is not ready";
    else
        cout << " is ready";

    for (auto & child : children) {
        cout << endl;
        for (int j = 0; j < level; j++)
            cout << "    ";

        child->outputAllSystem(level + 1);
    }
}

void HeadClass::setConnection(TYPE_SIGNAL signalPointer, HeadClass* targetObjectPointer, TYPE_HANDLER handlerPointer)
{
    objectSymbolHandler* pointerValue;
    for (auto connection : connections)
    {
        if (connection->signalPointer == signalPointer &&
            connection->targetObjectPointer == targetObjectPointer &&
            connection->handlerPointer == handlerPointer)
            return;
    }

    pointerValue = new objectSymbolHandler();

    pointerValue->signalPointer = signalPointer;
    pointerValue->targetObjectPointer = targetObjectPointer;
    pointerValue->handlerPointer = handlerPointer;

    connections.push_back(pointerValue);
}

void HeadClass::deleteConnection(TYPE_SIGNAL signalPointer, HeadClass* targetObjectPointer, TYPE_HANDLER handlerPointer)
{
    vector<objectSymbolHandler*>::iterator pointerIterator;
    for (pointerIterator = connections.begin(); pointerIterator < connections.end(); pointerIterator++)
    {
        if ((*pointerIterator)->signalPointer == signalPointer &&
            (*pointerIterator)->targetObjectPointer == targetObjectPointer &&
            (*pointerIterator)->handlerPointer == handlerPointer)
        {
            delete *pointerIterator;
            connections.erase(pointerIterator);
            break;
        }
    }
}

void HeadClass::emitSignal(TYPE_SIGNAL signalPointer, string &message)
{
    TYPE_HANDLER handlerPointer;
    HeadClass* targetObjectPointer;

    (this->*signalPointer)(message);
    for (auto connection : connections)
    {
        if (connection->signalPointer == signalPointer)
        {
            handlerPointer = connection->handlerPointer;
            targetObjectPointer = connection->targetObjectPointer;

            (targetObjectPointer->*handlerPointer)(message);
        }
    }
}

HeadClass::~HeadClass()
{
    queue <HeadClass*> allPointers;
    allPointers.push(getAnyPointer("/"));

    while(!allPointers.empty())
    {
        HeadClass* currentPointer = allPointers.front();
        for (auto connection : currentPointer->connections)
            if (connection->targetObjectPointer == this)
            {
                currentPointer->deleteConnection(connection->signalPointer,
                                                 connection->targetObjectPointer, connection->handlerPointer);
                break;
            }
        allPointers.pop();
    }

    while (!connections.empty())
        deleteConnection(connections[0]->signalPointer,
                         connections[0]->targetObjectPointer, connections[0]->handlerPointer);

    for (auto & child : children)
        delete child;
}

void HeadClass::deleteObject(string nameToDelete)
{
    for (int i = 0; i < children.size(); i++)
        if (children[i]->objectName == nameToDelete)
        {
            delete children[i];
            children.erase(children.begin() + i);
            break;
        }
}
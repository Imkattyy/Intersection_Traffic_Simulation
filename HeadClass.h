#ifndef HEAD_CLASS_H
#define HEAD_CLASS_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define SIGNAL_D(signal_f) (TYPE_SIGNAL)(&signal_f)
#define HANDLER_D(handler_f) (TYPE_HANDLER)(&handler_f)


class HeadClass;
typedef void (HeadClass::*TYPE_SIGNAL) (string& message); // тип указателя на метод-сигнал
typedef void (HeadClass::*TYPE_HANDLER) (string message); // тип указателя на метод-обработчика

struct objectSymbolHandler
{
    TYPE_SIGNAL signalPointer; // указатель на метод-сигнал
    HeadClass* targetObjectPointer; // указатель на целевой объект
    TYPE_HANDLER handlerPointer; // указатель на метод-обработчик
};


class HeadClass {
private:
    string objectName; // наименование объекта
    HeadClass *parentPointer; // указатель на головной объект для текущего объекта

    int readyMark = 0; // отметка о готовности объекта
    int classNumber; //номер класса

    vector <HeadClass*> children; // динамический массив указателей на объекты
    vector <objectSymbolHandler*> connections; // динамический массив указателей на связи
public:
    HeadClass(HeadClass* parentPointer, string newObjectName = "Crossroad system"); // конструктор

    void setClassNumber(int classNumber); // установка номера класса
    void setReadyMark(int isReady); // установка текущего состояния объекта
    void setDefaultReadyMark(); // установка состояний всех объектов

    bool renameObject(string newName); // переименование объекта
    bool changeParent(HeadClass* newParentPointer); // смена родителя

    int getClassNumber(); // получение номера класса
    string getObjectName(); // получение имени объекта
    string getAbsoluteCoordinate(); // получение абсолютной координаты до текущего объекта
    HeadClass* getHeadObjectPointer(); // получение указателя на головной объект

    vector<HeadClass*> getChildren(); // получения вектора детей
    HeadClass* getChildPointer(string childName); // получение указателя на подчиненный объект
    HeadClass* getAnyPointer(string coordinate); // поиск объекта по координате

    HeadClass* findObjectOnBranch(string nameToFind); // поиск объекта на ветке
    HeadClass* findObjectOnTree(string nameToFind); // поиск объекта на всем дереве

    void setConnection(TYPE_SIGNAL signalPointer, HeadClass* targetObjectPointer, TYPE_HANDLER handlerPointer);
    void deleteConnection(TYPE_SIGNAL signalPointer, HeadClass* targetObjectPointer, TYPE_HANDLER handlerPointer);
    void emitSignal(TYPE_SIGNAL signalPointer, string& message);

    void outputObjectTree(int level = 1); // вывод дерева от текущего объекта
    void outputAllSystem(int level = 1); // вывод дерева и отметок о готовности

    void deleteObject(string nameToDelete); // удаление подчиненного объекта
    ~HeadClass(); // деструктор
};

#endif // HEAD_CLASS_H
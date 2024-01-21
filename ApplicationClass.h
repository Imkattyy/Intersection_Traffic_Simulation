#ifndef APPLICATION_CLASS
#define APPLICATION_CLASS

#include <iostream>
#include <vector>
#include "HeadClass.h"

using namespace std;


class ApplicationClass : public HeadClass {
public:
    ApplicationClass(HeadClass* ptr = nullptr);

    void buildTree(); // построение дерева иерархии объектов
    int launchApp(); // запуск приложения

    void signal(string& message); // метод сигнала обработчику вводимых сообщений
    void handler(string message); // прием вводимых сообщений
};

#endif // APPLICATION_CLASS
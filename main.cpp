#include <stdlib.h>
#include <stdio.h>
#include "ApplicationClass.h"
int main()
{
    ApplicationClass ob_app_class = ApplicationClass(nullptr);
    ob_app_class.buildTree();
    return ob_app_class.launchApp();
}
#include <iostream>
#include <locale>
#include <windows.h>
#include "AppConnector.h"

using namespace std;

int main(void) {

    SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, ".UTF8");
    
    AppConnector app;
    app.showMainMenu();


    return 0;
}




#include "DuplicateSymbolsLib.h"

#include <iostream>

int a = 10;
void helperFunc() { std::cout << "In helperFunc, a = " << a << std::endl; }

int someFunc() { return 4; }

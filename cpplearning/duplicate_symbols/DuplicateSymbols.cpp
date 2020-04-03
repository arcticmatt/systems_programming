#include "DuplicateSymbolsLib.h"
#include <iostream>
using namespace std;

constexpr int a = 5;
// int a = 5;

namespace {
int someFunc() { return 5; }
} // namespace

int main() {
  helperFunc();

  std::cout << "result of someFunc() = " << someFunc() << std::endl;
  return 0;
}

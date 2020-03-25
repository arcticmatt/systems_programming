#include <iostream>
#include <memory>

#include "TestClasses.h"

int main() {
  auto a = std::make_unique<int>(5);
  std::cout << a.get() << std::endl;
  auto b = std::move(a);
  std::cout << a.get() << std::endl;
}

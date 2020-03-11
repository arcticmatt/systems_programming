#include <iostream>
#include <unordered_map>

#include "TestClasses.h"

int main() {
  std::cout << "Constructing map" << std::endl;
  std::unordered_map<int, TestClass> map;

  std::cout << "Constructing t1" << std::endl;
  TestClass t1(10);
  TestClass t2(15);
  std::cout << std::endl;

  std::cout << "map[5] = t1" << std::endl;
  map[5] = t1;
  std::cout << "map[5].a() = " << map[5].a() << std::endl;
  std::cout << std::endl;

  std::cout << "map[6] = std::move(t1)" << std::endl;
  map[6] = std::move(t1);
  std::cout << "map[6].a() = " << map[6].a() << std::endl;
  std::cout << std::endl;

  std::cout << "emplace" << std::endl;
  map.emplace(7, std::move(t2));
}

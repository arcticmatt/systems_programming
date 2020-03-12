#include <iostream>
#include <vector>

#include "TestClasses.h"

int main() {
  std::cout << "Constructing map" << std::endl;
  std::vector<TestClass> vec;
  vec.reserve(10);

  std::cout << "Constructing t1, t2, t3" << std::endl;
  TestClass t1(10);
  TestClass t2(15);
  TestClass t3(20);
  std::cout << std::endl;

  std::cout << "vec.push_back(std::move(t1))" << std::endl;
  vec.push_back(std::move(t1));
  std::cout << std::endl;

  std::cout << "vec.emplace_back(t2)" << std::endl;
  vec.emplace_back(std::move(t2));
  std::cout << std::endl;
}

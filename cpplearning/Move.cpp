#include <iostream>
#include <memory>

#include "classes/DefaultMove.h"
#include "classes/TestClasses.h"

///
/// Moving STL
///

void moveString() {
  std::string strA = "strA";
  std::string strB = "strB";

  std::cout << "strA = " << strA << std::endl;
  std::cout << "strB = " << strB << std::endl;

  std::cout << std::endl;

  std::cout << "strA = std::move(strB)" << std::endl;

  strA = std::move(strB);
  std::cout << "strA = " << strA << std::endl;
  std::cout << "strB = " << strB << std::endl;
}

///
/// Moving custom objects
///

void defaultMove() {
  std::cout << "===== defaultMove =====" << std::endl;
  DefaultMove a;
  a.sharedPtr = std::make_shared<int>(5);
  std::cout << "a.sharedPtr = " << a.sharedPtr << std::endl;
  std::cout << "a.str = " << a.str << std::endl;

  std::cout << std::endl;

  std::cout << "b(std::move(a))" << std::endl;
  DefaultMove b(std::move(a));
  std::cout << "a.sharedPtr = " << a.sharedPtr << std::endl;
  std::cout << "a.str = " << a.str << std::endl;
  std::cout << "b.sharedPtr = " << b.sharedPtr << std::endl;
  std::cout << "b.str = " << b.str << std::endl;
}

void testClass() {
  std::cout << "===== testClass =====" << std::endl;
  TestClass a;
  a.sharedPtr = std::make_shared<int>(5);
  std::cout << "a.sharedPtr = " << a.sharedPtr << std::endl;

  std::cout << "b(std::move(a))" << std::endl;
  TestClass b(std::move(a));
  std::cout << "a.sharedPtr = " << a.sharedPtr << std::endl;
  std::cout << "b.sharedPtr = " << b.sharedPtr << std::endl;
}

int main() {
  moveString();

  defaultMove();
  testClass();
}

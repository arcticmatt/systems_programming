#include <iostream>

#include "TestClasses.h"

void useAll() {
  // Default constructor
  TestClass t1;

  // One arg constructor
  TestClass t2(10);

  // Copy constructor
  TestClass t3 = t2;
  TestClass t4(t2);

  // Move constructor
  TestClass t5 = std::move(t1);
  TestClass t6 = std::move(t2);

  // Copy assignment
  t3 = t2;

  // Move assignment
  t4 = std::move(t2);
}

int main() {
  //   useAll();
  TestClass t1;
  TestClass2 t2(t1);
}

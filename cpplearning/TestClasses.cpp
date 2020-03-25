#include <iostream>
#include <vector>

#include "TestClasses.h"

/**
 * Having
 * const std::string str_;
 * in TestClass3 creates problems when using it in containers.
 *
 * For example, since vectors use a contiguous piece of memory, if
 * you delete an element, it will move everything over by one.
 * Which equates to copying to a pre-existing element.
 */
void constMemberInContainer() {
  std::vector<TestClass3> vec;
  vec.emplace_back("hi", 10); // call this a
  vec.emplace_back("hi", 15); // call this b
  vec.erase(vec.begin());     // equivalent to a = b, which doesn't work
}

int main() {
  // Doesn't compile
  constMemberInContainer();
}

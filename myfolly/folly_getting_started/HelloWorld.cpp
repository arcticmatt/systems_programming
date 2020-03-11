#include <folly/FBString.h>
#include <iostream>

int main() {
  folly::fbstring str("Hello, Folly World!");
  std::cout << str << "\n";
}

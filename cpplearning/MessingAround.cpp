#include <iostream>
#include <memory>
#include <vector>

void yo(const std::vector<const char *> &strs) {
  for (auto s : strs) {
    std::cout << s << std::endl;
  }
}

int main() {
  yo({"hi", "there", "friend"});
  auto b = {"hello", "again"};
  yo(b);
}

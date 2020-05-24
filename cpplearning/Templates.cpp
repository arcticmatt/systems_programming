#include <iostream>

template <typename... Ts> void func(Ts... args) {
  std::cout << __FUNCTION__ << std::endl;
  const int size = sizeof...(args) + 2;
  std::cout << "size = " << size << std::endl;
  int res[size] = {1, args..., 2};
  // since initializer lists guarantee sequencing, this can be used to
  // call a function on each element of a pack, in order:
  int dummy[sizeof...(Ts)] = {(std::cout << args << std::endl, 0)...};
}

template <typename T> static void appendArgs(std::ostream &oss, T v) {
  oss << v;
}

template <typename T, typename... Args>
static void appendArgs(std::ostream &oss, T first, const Args &... args) {
  oss << first;
  appendArgs(oss, args...);
}

int main() {
  func(1, 2, 3);
  appendArgs(std::cout, "hi ", "there ", "friend");
}

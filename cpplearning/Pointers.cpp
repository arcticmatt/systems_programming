#include <iostream>
#include <memory>

void casting() {
  std::cout << "===== casting =====" << std::endl;
  uint8_t *bytePtr = (uint8_t *)malloc(4);
  *bytePtr = 1;
  *(bytePtr + 1) = 1;
  *(bytePtr + 2) = 1;
  *(bytePtr + 3) = 1;

  uint8_t bytePtrVal = *bytePtr;
  uint32_t bytePtrVal2 = *(uint32_t *)bytePtr;
  std::cout << "bytePtrVal = " << (int)bytePtrVal << std::endl;
  std::cout << "bytePtrVal2 = " << bytePtrVal2 << std::endl;
}

void move() {
  std::cout << "===== move =====" << std::endl;
  auto a = std::make_unique<int>(5);
  std::cout << "a before move = " << a.get() << std::endl;

  std::cout << "moving a into b" << std::endl;
  auto b = std::move(a);
  std::cout << "a after move = " << a.get() << std::endl;
  std::cout << "b after move = " << b.get() << std::endl;
}

int main() {
  move();
  casting();
}

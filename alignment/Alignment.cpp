#include <iostream>
// https://developer.apple.com/documentation/code_diagnostics/undefined_behavior_sanitizer/misaligned_pointer

// https://stackoverflow.com/questions/1898153/how-to-determine-if-memory-is-aligned
#define IS_ALIGNED(POINTER, BYTE_COUNT)                                        \
  (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

void isAligned() {
  std::cout << "=== isAligned ===" << std::endl;
  int8_t *buffer = (int8_t *)malloc(64);
  std::cout << "buffer = " << (int32_t *)buffer << std::endl;

  int32_t *pointer = (int32_t *)(buffer + 4);
  std::cout << "pointer = " << pointer << std::endl;
  std::cout << "sizeof(*pointer) = " << sizeof(*pointer) << std::endl;
  std::cout << IS_ALIGNED(pointer, sizeof(*pointer)) << std::endl;
}

void isNotAligned() {
  std::cout << "=== isNotAligned ===" << std::endl;
  int8_t *buffer = (int8_t *)malloc(64);
  std::cout << "buffer = " << (int32_t *)buffer << std::endl;

  int32_t *pointer = (int32_t *)(buffer + 2);
  std::cout << "pointer = " << pointer << std::endl;
  std::cout << "sizeof(*pointer) = " << sizeof(*pointer) << std::endl;
  std::cout << IS_ALIGNED(pointer, sizeof(*pointer)) << std::endl;
}

struct TestStruct24Bytes {
  uint64_t a = 3;
  uint64_t b = 3;
  uint64_t c = 3;
};

void testAlignmentOfDifferentTypes() {
  std::cout << "=== testAlignmentOfDifferentTypes === " << std::endl;

  std::cout << "testing uint8_t" << std::endl;
  for (int i = 0; i < 10; i++) {
    uint8_t *pointer = new uint8_t();
    std::cout << "pointer = " << (uint16_t *)pointer
              << ", aligned = " << IS_ALIGNED(pointer, sizeof(*pointer))
              << std::endl;
  }

  std::cout << "testing uint16_t" << std::endl;
  for (int i = 0; i < 10; i++) {
    uint16_t *pointer = new uint16_t();
    std::cout << "pointer = " << pointer
              << ", aligned = " << IS_ALIGNED(pointer, sizeof(*pointer))
              << std::endl;
  }

  std::cout << "testing uint32_t" << std::endl;
  for (int i = 0; i < 10; i++) {
    uint32_t *pointer = new uint32_t();
    std::cout << "pointer = " << pointer
              << ", aligned = " << IS_ALIGNED(pointer, sizeof(*pointer))
              << std::endl;
  }

  std::cout << "testing uint64_t" << std::endl;
  for (int i = 0; i < 10; i++) {
    uint64_t *pointer = new uint64_t();
    std::cout << "pointer = " << pointer
              << ", aligned = " << IS_ALIGNED(pointer, sizeof(*pointer))
              << std::endl;
  }

  std::cout << "testing TestStruct24Bytes" << std::endl;
  std::cout << "sizeof(TestStruct24Bytes) = " << sizeof(TestStruct24Bytes)
            << std::endl;
  for (int i = 0; i < 10; i++) {
    TestStruct24Bytes *pointer = new TestStruct24Bytes();
    std::cout << "pointer = " << pointer
              << ", aligned = " << IS_ALIGNED(pointer, sizeof(*pointer))
              << std::endl;
  }
}

int main() {
  std::cout << "Hello" << std::endl;
  isAligned();
  isNotAligned();
  testAlignmentOfDifferentTypes();
}

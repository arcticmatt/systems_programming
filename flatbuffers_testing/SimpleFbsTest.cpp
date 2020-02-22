#include "simple_generated.h"

#include <iostream>
#include <vector>

using namespace MyFbs;

void deserializeTestStruct() {
  std::cout << "=== Deserialize TestStruct ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);

  auto testStruct = TestStruct(15);
  auto buffer = (uint8_t *)malloc(sizeof(testStruct));
  auto serialized = memcpy(buffer, &testStruct, sizeof(testStruct));
  auto testStructDeserialized = (TestStruct *)buffer;
  std::cout << "testStructDeserialized a = " << testStructDeserialized->a()
            << std::endl;
}

void printTestStructBufSize() {
  std::cout << "=== Print TestStruct ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);
  // 0 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  auto testStruct = TestStruct(15);
  auto offset = builder.CreateStruct(testStruct);
  // 2 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  // Calls PushElement(ReferTo(root)) + PreAlign
  // ReferTo returns value of type flatbuffers::uoffset_t
  // sizeof(flatbuffers::uoffset_t) == 4
  // Thus, data is aligned to 4 bytes, becomes 8 bytes total
  builder.Finish(offset);
  // 8 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;
}

void printTestTableBufSize() {
  std::cout << "=== Print TestTable ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);

  // Equivalent to:
  // auto testTable = CreateTestTable(builder, 15);
  TestTableBuilder ttBuilder(builder);
  // 0 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  ttBuilder.add_a(15);
  // 2 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  // Inside TestTableBuilder::Finish, FlatBufferBuilder::EndTable is called.
  // This function "finishes one serialized object by generating the vtable if
  // it's a table, comparing it against existing vtables, and writing the
  // resulting vtable offset." The vtable is what is taking up the extra space.
  auto testTable = ttBuilder.Finish();
  // 14 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  // Se above. The root offset is what takes up the extra space.
  builder.Finish(testTable);
  // 20 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;
}

int main() {
  std::cout << "=== Main ===" << std::endl;
  std::cout << "sizeof(flatbuffers::uoffset_t) = "
            << sizeof(flatbuffers::uoffset_t) << std::endl;
  std::cout << "sizeof(TestStruct) = " << sizeof(TestStruct) << std::endl;
  std::cout << "sizeof(TestTable) = " << sizeof(TestTable) << std::endl;
  std::cout << "sizeof(TestTableT) = " << sizeof(TestTableT) << std::endl;

  deserializeTestStruct();
  printTestStructBufSize();
  printTestTableBufSize();
}

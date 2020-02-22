//
// Run:
// ./../../flatbuffers/flatc --cpp --gen-object-api ../simple.fbs
//

#include "simple_generated.h"

#include <fstream>
#include <iostream>
#include <vector>

using namespace MyFbs;

constexpr auto kFieldA = 15;
constexpr auto kFieldB = 8;

void deserializeTestStruct() {
  std::cout << "=== Deserialize TestStruct ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);

  auto testStruct = TestStruct(kFieldA, kFieldB);
  auto buffer = (uint8_t *)malloc(sizeof(testStruct));
  auto serialized = memcpy(buffer, &testStruct, sizeof(testStruct));
  auto testStructDeserialized = (TestStruct *)buffer;
  std::cout << "testStructDeserialized a = "
            << (int)testStructDeserialized->field_a()
            << ", b = " << (int)testStructDeserialized->field_b() << std::endl;
}

void printTestStructBufSize() {
  std::cout << "=== Print TestStruct Buf Size ===" << std::endl;
  auto testStruct = TestStruct(kFieldA, kFieldB);
  std::cout << "buf size = " << sizeof(testStruct) << std::endl;
}

void printTestStructAsRootBufSize() {
  std::cout << "=== Print TestStruct (as root) Buf Size ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);
  // 0 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  auto testStruct = TestStruct(kFieldA, kFieldB);
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
  std::cout << "=== Print TestTable Buf Size ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);

  // Equivalent to:
  // auto testTable = CreateTestTable(builder, kFieldA, kFieldB);
  TestTableBuilder ttBuilder(builder);
  // 0 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  ttBuilder.add_field_a(kFieldA);
  // 1 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  ttBuilder.add_field_b(kFieldB);
  // 2 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  // Inside TestTableBuilder::Finish, FlatBufferBuilder::EndTable is called.
  // This function "finishes one serialized object by generating the vtable if
  // it's a table, comparing it against existing vtables, and writing the
  // resulting vtable offset." The vtable is what is taking up the extra space.
  auto testTable = ttBuilder.Finish();
  // 16 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;

  // Se above. The root offset is what takes up the extra space.
  builder.Finish(testTable);
  // 20 bytes
  std::cout << "buf size = " << builder.GetSize() << std::endl;
}

void testStructToFile() {
  auto testStruct = TestStruct(kFieldA, kFieldB);
  std::ofstream outFile;
  outFile.open("test_struct_binary.out",
               std::ios::out | std::ios::binary | std::ios::trunc);
  outFile.write((char *)&testStruct, sizeof(testStruct));
  outFile.close();
}

void testStructAsRootToFile() {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto testStruct = TestStruct(kFieldA, kFieldB);
  auto offset = builder.CreateStruct(testStruct);
  builder.Finish(offset);

  std::ofstream outFile;
  outFile.open("test_struct_as_root_binary.out",
               std::ios::out | std::ios::binary | std::ios::trunc);
  outFile.write((char *)builder.GetBufferPointer(), builder.GetSize());
  outFile.close();
}

void testTableToFile() {
  flatbuffers::FlatBufferBuilder builder(1024);
  TestTableBuilder ttBuilder(builder);
  ttBuilder.add_field_a(kFieldA);
  ttBuilder.add_field_b(kFieldB);
  auto testTable = ttBuilder.Finish();
  builder.Finish(testTable);

  std::ofstream outFile;
  outFile.open("test_table_binary.out",
               std::ios::out | std::ios::binary | std::ios::trunc);
  outFile.write((char *)builder.GetBufferPointer(), builder.GetSize());
  outFile.close();
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
  printTestStructAsRootBufSize();
  printTestTableBufSize();
  testStructToFile();
  testStructAsRootToFile();
  testTableToFile();
}

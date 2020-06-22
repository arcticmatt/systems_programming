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
constexpr auto kStructField = 11;

constexpr auto kTestTableFileName = "test_table_binary.out";
constexpr auto kTestTable3FileName = "test_table3_binary.out";

void fileToTestTable(const std::string &fileName) {
  std::cout << "=== fileToTestTable for " << fileName << " ===" << std::endl;
  std::ifstream infile;
  infile.open(fileName, std::ios::binary | std::ios::in);
  infile.seekg(0, std::ios::end);
  int length = infile.tellg();
  infile.seekg(0, std::ios::beg);
  char *data = new char[length];
  infile.read(data, length);
  infile.close();

  const TestTable *testTable = flatbuffers::GetRoot<TestTable>(data);
  std::cout << "testTable->field_a() = " << (int)testTable->field_a()
            << std::endl;
  std::cout << "testTable->field_b() = " << (int)testTable->field_b()
            << std::endl;
  if (testTable->field_struct()) {
    std::cout << "testTable->field_struct()->field_a() = "
              << (int)testTable->field_struct()->field_a() << std::endl;
  } else {
    std::cout << "testTable->field_struct() = "
              << (void *)testTable->field_struct() << std::endl;
  }
  std::cout << std::endl;

  const TestTable2 *testTable2 = flatbuffers::GetRoot<TestTable2>(data);
  std::cout << "testTable2->field_a() = " << (int)testTable2->field_a()
            << std::endl;
  std::cout << "testTable2->field_b() = " << (int)testTable2->field_b()
            << std::endl;
  if (testTable2->field_struct()) {
    std::cout << "testTable2->field_struct()->field_a() = "
              << (int)testTable2->field_struct()->field_a() << std::endl;
  } else {
    std::cout << "testTable2->field_struct() = "
              << (void *)testTable2->field_struct() << std::endl;
  }
  std::cout << std::endl;

  const TestTable3 *testTable3 = flatbuffers::GetRoot<TestTable3>(data);
  std::cout << "testTable3->field_a() = " << (int)testTable3->field_a()
            << std::endl;
}

void testTableToFile(const std::string &fileName) {
  std::cout << "=== testTableToFile ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);
  TestTableBuilder ttBuilder(builder);
  ttBuilder.add_field_a(kFieldA);
  ttBuilder.add_field_b(kFieldB);
  SimpleStruct simpleStruct{kStructField};
  ttBuilder.add_field_struct(&simpleStruct);
  auto testTable = ttBuilder.Finish();
  builder.Finish(testTable);

  std::ofstream outFile;
  outFile.open(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
  outFile.write((char *)builder.GetBufferPointer(), builder.GetSize());
  outFile.close();
}

void testTable3ToFile(const std::string &fileName) {
  std::cout << "=== testTable3ToFile ===" << std::endl;
  flatbuffers::FlatBufferBuilder builder(1024);
  TestTable3Builder ttBuilder(builder);
  ttBuilder.add_field_a(kFieldA);
  ttBuilder.add_field_b(kFieldB);
  auto testTable = ttBuilder.Finish();
  builder.Finish(testTable);

  std::ofstream outFile;
  outFile.open(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
  outFile.write((char *)builder.GetBufferPointer(), builder.GetSize());
  outFile.close();
}

int main() {
  std::cout << "=== Main ===" << std::endl;
  testTableToFile(kTestTableFileName);
  testTable3ToFile(kTestTable3FileName);

  fileToTestTable(kTestTableFileName);
  fileToTestTable(kTestTable3FileName);
}

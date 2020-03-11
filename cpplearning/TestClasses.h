#pragma once

#include <iostream>

class TestClass {
public:
  // Default constructor
  TestClass() { std::cout << "Default Constructor: TestClass()" << std::endl; }

  // One arg constructor
  explicit TestClass(int a) : a_(a) {
    std::cout << "One Arg Constructor: TestClass(int a)" << std::endl;
  }

  // Destructor
  ~TestClass() { std::cout << "Destructor: ~TestClass()" << std::endl; }

  // Copy constructor
  TestClass(const TestClass &t) {
    std::cout << "Copy Constructor: TestClass(const TestClass &t)" << std::endl;
    a_ = t.a_;
  }

  // Move constructor
  TestClass(const TestClass &&t) {
    std::cout << "Move Constructor: TestClass(const TestClass &&t)"
              << std::endl;
    a_ = t.a_;
  }

  // Copy assignment
  TestClass &operator=(const TestClass &t) {
    std::cout << "Copy Assignment: TestClass &operator=(const TestClass &t)"
              << std::endl;
    a_ = t.a_;
    return *this;
  }

  // Move assignment
  TestClass &operator=(const TestClass &&t) {
    std::cout << "Move Assignment: TestClass &operator=(const TestClass &&t)"
              << std::endl;
    a_ = t.a_;
    return *this;
  }

  int a() const { return a_; }

private:
  int a_ = 5;
};

class TestClass2 {
public:
  TestClass2(TestClass t) : t1(std::move(t)) {
    std::cout << "TestClass2(TestClass t)" << std::endl;
  }

  TestClass t1;
};

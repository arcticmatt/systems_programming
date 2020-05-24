#pragma once

#include <iostream>
#include <memory>

class DefaultMove {
public:
  // Default constructor
  DefaultMove() {
    std::cout << "Default Constructor: DefaultMove()" << std::endl;
  }

  // One arg constructor
  explicit DefaultMove(int a) : a_(a) {
    std::cout << "One Arg Constructor: DefaultMove(int a)" << std::endl;
  }

  // Destructor
  ~DefaultMove() {
    std::cout << "Destructor: ~DefaultMove()" << std::endl;
    a_ = -1;
  }

  // Copy constructor
  DefaultMove(const DefaultMove &t) {
    std::cout << "Copy Constructor: DefaultMove(const DefaultMove &t)"
              << std::endl;
    a_ = t.a_;
  }

  // Move constructor
  DefaultMove(DefaultMove &&t) = default;

  // Copy assignment
  DefaultMove &operator=(const DefaultMove &t) {
    std::cout << "Copy Assignment: DefaultMove &operator=(const DefaultMove &t)"
              << std::endl;
    a_ = t.a_;
    return *this;
  }

  // Move assignment
  DefaultMove &operator=(DefaultMove &&t) = default;

  int a() const { return a_; }

  std::shared_ptr<int> sharedPtr = nullptr;
  std::string str = "default";

private:
  int a_ = 5;
};

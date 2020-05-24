#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

int main() {
  // Redirect stdout -> example.txt
  freopen("example.txt", "w", stdout);

  // Disable bufferring
  setbuf(stdout, NULL);

  auto t1 = std::thread([]() {
    auto count = 0;
    while (true) {
      printf("Printing from thread 1, count = %d\n", count++);
      std::this_thread::sleep_for(100ms);
    }
  });

  auto t2 = std::thread([]() {
    auto count = 0;
    while (true) {
      printf("Printing from thread 2, count = %d\n", count++);
      std::this_thread::sleep_for(100ms);
    }
  });

  auto fileThread = std::thread([]() {
    auto lastPos = 0;
    while (true) {
      std::this_thread::sleep_for(10ms);

      std::ifstream myfile("example.txt");

      myfile.seekg(lastPos);

      std::string line;
      while (getline(myfile, line)) {
        auto time = std::chrono::steady_clock::now().time_since_epoch().count();
        std::cerr << "%" << time << "%" << line << std::endl;
        lastPos = myfile.tellg();
      }
      myfile.close();
    }
  });

  t1.join();
  t2.join();

  return 0;
}

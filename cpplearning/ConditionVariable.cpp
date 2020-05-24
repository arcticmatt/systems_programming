#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

std::mutex globalMutex;
std::condition_variable globalCv;

int main() {
  std::cout << "Main start" << std::endl;

  auto pred = false;

  auto thread = std::thread([&] {
    std::cout << "Thread start" << std::endl;
    std::lock_guard<std::mutex> lock(globalMutex);
    std::this_thread::sleep_for(1s);
    pred = true;
    globalCv.notify_all();
    std::cout << "Thread end" << std::endl;
  });

  std::cout << "Main, before wait_for" << std::endl;
  {
    std::unique_lock<std::mutex> lock(globalMutex);
    globalCv.wait_for(lock, 30s, [&]() {
      std::cout << "Returning pred = " << pred << std::endl;
      return pred;
    });
  }
  std::cout << "Main, after wait_for" << std::endl;
  thread.join();
}

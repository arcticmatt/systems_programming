//
// A demo showing how timeouts + folly Futures work.
//

#include "ThreadUtils.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>

using namespace std::chrono_literals;

enum class UseExecutor { NO = 1, YES = 0 };

constexpr auto kGlobalLabel = "kGlobal";
auto [kGlobalPromise, kGlobalSf] = folly::makePromiseContract<int>();
auto kGlobalThread =
    std::thread([&kGlobalPromise = kGlobalPromise, label = kGlobalLabel]() {
      while (!kGlobalPromise.isFulfilled()) {
        printf("%sLooping...\n",
               tidLabelStr(std::this_thread::get_id(), label).c_str());
        std::this_thread::sleep_for(3s);
      }
    });

folly::SemiFuture<int> deferredWhileLoop() {
  const std::string label = "deferredWhileLoop";
  printf("%sStart\n", tidLabelStr(std::this_thread::get_id(), label).c_str());
  return folly::makeSemiFuture().defer([label](auto) {
    printf("%sIn defer block\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
    while (1) {
      printf("%sLooping...\n",
             tidLabelStr(std::this_thread::get_id(), label).c_str());
      std::this_thread::sleep_for(1s);
    }
    return 1;
  });
}

void testDeferredWhileLoop(UseExecutor useExecutor) {
  const std::string label = "testDeferredWhileLoop";
  printf("%sStart, useExecutor = %d\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str(),
         static_cast<std::underlying_type<UseExecutor>::type>(useExecutor));

  if (useExecutor == UseExecutor::YES) {
    // folly::ThreadedExecutor exec;
    folly::CPUThreadPoolExecutor exec{1};

    std::cout << "Before calling deferredWhileLoop" << std::endl;
    try {
      deferredWhileLoop().via(&exec).get(1s);
      printf("%sAfter calling deferredWhileLoop\n",
             tidLabelStr(std::this_thread::get_id(), label).c_str());
    } catch (folly::FutureTimeout &ex) {
      printf("%s===== folly FutureTimeout exception: %s =====\n",
             tidLabelStr(std::this_thread::get_id(), label).c_str(), ex.what());
    }
  } else {
    printf("%sBefore calling deferredWhileLoop\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
    deferredWhileLoop().get(1s);
    printf("%sAfter calling deferredWhileLoop\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
  }
}

void testGlobalPromise() {
  const std::string label = "testGlobalPromise";
  printf("%sStart\n", tidLabelStr(std::this_thread::get_id(), label).c_str());

  try {
    std::move(kGlobalSf).within(1s).get();
    printf("%sAfter calling kGlobalSf.via()\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
  } catch (folly::FutureTimeout &ex) {
    printf("%s===== folly FutureTimeout exception: %s =====\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str(), ex.what());
  }
}

int main(int argc, char *argv[]) {
  const std::string label = "main";
  // NOT CALLED
  // - get(timeout)
  // YES CALLED
  // - within(timeout).get()
  // - via(&exec).wait(timeout) + cancel()
  kGlobalPromise.setInterruptHandler([&kGlobalPromise = kGlobalPromise, label](
                                         const folly::exception_wrapper &e) {
    printf("%sIn kGlobalPromise's interrupt handler!\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
    kGlobalPromise.setValue(10);
  });

  if (argc > 1 && std::string(argv[1]) == "promise") {
    testGlobalPromise();
    kGlobalThread.join();
  } else {
    kGlobalPromise.setValue(0);
    testDeferredWhileLoop(std::string(argv[1]) == "executor" ? UseExecutor::YES
                                                             : UseExecutor::NO);
  }
}

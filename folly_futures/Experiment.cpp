///
/// Contains scratch work.
///

#include <chrono>
#include <iostream>
#include <thread>

#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>

using namespace std::chrono_literals;

enum class UseExecutor { NO = 1, YES = 0 };
enum class WithPromise { NO = 1, YES = 0 };

folly::SemiFuture<int> deferredWhileLoop() {
  std::cout << "deferredWhileLoop thread_id = " << std::this_thread::get_id()
            << std::endl;
  return folly::makeSemiFuture()
      .defer([](auto) {
        std::cout << "first defer! deferredWhileLoop defer thread_id = "
                  << std::this_thread::get_id() << std::endl;
      })
      .defer([](auto) {
        std::cout << "second defer! deferredWhileLoop defer thread_id = "
                  << std::this_thread::get_id() << std::endl;
        while (1) {
          std::cout << "looping..." << std::endl;
          std::this_thread::sleep_for(2s);
        }
        return 1;
      });
}

folly::SemiFuture<int> launchAsyncWork(folly::Promise<int> promise) {
  std::cout << "launchAsyncWork thread_id = " << std::this_thread::get_id()
            << std::endl;
  return folly::makeSemiFuture().defer([&](auto) {
    std::cout << "launchAsyncWork defer thread_id = "
              << std::this_thread::get_id() << std::endl;
    while (1) {
      std::cout << "looping..." << std::endl;
      std::this_thread::sleep_for(2s);
    }
    promise.setValue(5);
    return 5;
  });
}

folly::SemiFuture<int> deferredWhileLoopWithPromise() {
  std::cout << "deferredWhileLoopWithPromise thread_id = "
            << std::this_thread::get_id() << std::endl;
  auto [promise, sf] = folly::makePromiseContract<int>();
  std::cout << "Launching async work" << std::endl;
  folly::CPUThreadPoolExecutor exec(1);
  launchAsyncWork(std::move(promise)).via(&exec);
  std::cout << "Returning SemiFuture" << std::endl;
  return std::move(sf);
}

void testDeferredWhileLoop(UseExecutor useExecutor, WithPromise withPromise) {
  std::cout << "testDeferredWhileLoop: useExecutor = "
            << static_cast<std::underlying_type<UseExecutor>::type>(useExecutor)
            << ", withPromise = "
            << static_cast<std::underlying_type<WithPromise>::type>(withPromise)
            << ", thread_id = " << std::this_thread::get_id() << std::endl;

  if (useExecutor == UseExecutor::YES) {
    folly::ThreadedExecutor exec;

    std::cout << "Before calling deferredWhileLoop" << std::endl;
    try {
      if (withPromise == WithPromise::YES) {
        deferredWhileLoopWithPromise().via(&exec).get(1s);
      } else {
        deferredWhileLoop().via(&exec).get(1s);
      }
      std::cout << "After calling deferredWhileLoop" << std::endl;
    } catch (folly::FutureTimeout &ex) {
      std::cout << "folly FutureTimeout exception: " << ex.what() << std::endl;
    }
  } else {
    std::cout << "Before calling deferredWhileLoop" << std::endl;
    if (withPromise == WithPromise::YES) {
      deferredWhileLoopWithPromise().get(1s);
    } else {
      deferredWhileLoop().get(1s);
    }
    std::cout << "After calling deferredWhileLoop" << std::endl;
  }
}

void testDefer() {
  std::cout << "testDefer thread_id = " << std::this_thread::get_id()
            << std::endl;
  folly::CPUThreadPoolExecutor exec(1);
  auto sf = folly::makeSemiFuture().defer([&](auto) {
    std::cout << "testDefer, inside defer thread_id = "
              << std::this_thread::get_id() << std::endl;
    while (1) {
      std::cout << "looping..." << std::endl;
      std::this_thread::sleep_for(1s);
    }
  });
  std::cout << "Got SemiFuture" << std::endl;

  std::move(sf).via(&exec);

  std::cout << "Done with testDefer" << std::endl;
}

void testThen() {
  std::cout << "testThen thread_id = " << std::this_thread::get_id()
            << std::endl;
  folly::CPUThreadPoolExecutor exec(1);
  auto fut = folly::makeSemiFuture().via(&exec).then([&](auto) {
    std::cout << "testThen, inside then thread_id = "
              << std::this_thread::get_id() << std::endl;
    while (1) {
      std::cout << "looping..." << std::endl;
      std::this_thread::sleep_for(1s);
    }
  });
  std::cout << "Done with then!" << std::endl;
}

int main() {
  //   testDefer();
  //   testThen();
  //   testDeferredWhileLoop(UseExecutor::YES, WithPromise::YES);
  //   testDeferredWhileLoop(UseExecutor::NO, WithPromise::YES);
  testDeferredWhileLoop(UseExecutor::YES, WithPromise::NO);
}

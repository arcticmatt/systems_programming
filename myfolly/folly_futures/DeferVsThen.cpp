//
// Short example showing how to run code on an executor using
// defer and then.
//
// The behavior is basically the same. The difference is that when using
// defer, the caller has more control over the executor that is used.
//

#include "ThreadUtils.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>

using namespace std::chrono_literals;

folly::SemiFuture<int> testDefer() {
  const std::string label = "testDefer";
  printf("%sStart\n", tidLabelStr(std::this_thread::get_id(), label).c_str());

  auto sf = folly::makeSemiFuture().defer([&, label](auto) {
    printf("%sInside defer block\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
    while (1) {
      printf("%sLooping...\n",
             tidLabelStr(std::this_thread::get_id(), label).c_str());
      std::this_thread::sleep_for(1s);
    }

    return 1;
  });

  return std::move(sf);
}

folly::Future<int> testThen(folly::CPUThreadPoolExecutor &exec) {
  const std::string label = "testThen";
  printf("%sStart\n", tidLabelStr(std::this_thread::get_id(), label).c_str());

  // Don't create exec here! Otherwise, when we exit this function's scope,
  // exec would get destroyed, but it could not because we're holding a
  // KeepAlive token.
  //
  // Could also use ThreadedExecutor, which doesn't do any KeepAlive checking
  // in its destructor.
  auto fut = folly::makeSemiFuture().via(&exec).then([&, label](auto) {
    printf("%sInside then block\n",
           tidLabelStr(std::this_thread::get_id(), label).c_str());
    while (1) {
      printf("%sLooping...\n",
             tidLabelStr(std::this_thread::get_id(), label).c_str());
      std::this_thread::sleep_for(1s);
    }
    return 1;
  });

  // If this function was void and we didn't return... we would just hang here.
  return std::move(fut);
}

int main(int argc, char *argv[]) {
  const std::string label = "main";
  printf("%sStart\n", tidLabelStr(std::this_thread::get_id(), label).c_str());

  // Create here to avoid hanging on this executor's KeepAlive token.
  // Try changing number of threads to 1 and see what happens.
  folly::CPUThreadPoolExecutor exec(2);

  auto sf = testDefer();
  printf("%sAfter calling testDefer\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str());
  std::move(sf).via(&exec);
  printf("%sAfter running sf testDefer\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str());

  // auto fut = testThen(exec);
  // printf("%sAfter calling testThen\n",
  //        tidLabelStr(std::this_thread::get_id(), label).c_str());
}

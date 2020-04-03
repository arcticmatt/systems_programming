//
// A demo showing how timeouts + folly Futures work.
//

#include "ThreadUtils.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/futures/Future.h>

#include <CLI/CLI11.hpp>

using namespace std::chrono_literals;

int perFutureTimeout = 500;
int collectTimeout = 500;

folly::SemiFuture<folly::Unit> unitSf() {
  const std::string label = "unitSf";
  printf("%sStart of unitSf\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str());
  return folly::futures::sleep(5s)
      .within(std::chrono::milliseconds(perFutureTimeout))
      .deferValue([label](auto) {
        printf("%sEnd of unitSf\n",
               tidLabelStr(std::this_thread::get_id(), label).c_str());
        // Note: returning nothing is same as returning folly::unit
      })
      .deferError([label](folly::exception_wrapper &&e) {
        printf("%sError for unitSf, %s\n",
               tidLabelStr(std::this_thread::get_id(), label).c_str(),
               e.what().c_str());
      });
}

void runAsync() {
  const std::string label = "runAsync";
  auto startTime = std::chrono::steady_clock::now();
  printf("===== %sRun async =====\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str());
  folly::CPUThreadPoolExecutor exec{3};
  unitSf().via(&exec);
  unitSf().via(&exec);
  unitSf().via(&exec);

  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
  printf("===== %sRun async took %d seconds =====\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str(),
         (int)elapsed.count());
}

void runCollect() {
  const std::string label = "runCollect";
  auto startTime = std::chrono::steady_clock::now();
  printf("===== %sRun collect =====\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str());
  folly::CPUThreadPoolExecutor exec{3};

  std::vector<folly::SemiFuture<folly::Unit>> semiFutures = {};
  for (int i = 0; i < 3; i++) {
    semiFutures.push_back(unitSf());
  }

  folly::collectSemiFuture(std::move(semiFutures))
      .within(std::chrono::milliseconds(collectTimeout))
      .deferValue([label](auto) {
        printf("%sEnd of collect\n",
               tidLabelStr(std::this_thread::get_id(), label).c_str());
      })
      .deferError([label](folly::exception_wrapper &&e) {
        printf("%sError for collect, %s\n",
               tidLabelStr(std::this_thread::get_id(), label).c_str(),
               e.what().c_str());
      })
      .via(&exec)
      .get();

  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
  printf("===== %sRun collect took %d seconds =====\n",
         tidLabelStr(std::this_thread::get_id(), label).c_str(),
         (int)elapsed.count());
}

void runSerial() {
  printf("===== %sRun serial =====\n",
         tidLabelStr(std::this_thread::get_id(), "runSerial").c_str());
  unitSf().get();
  unitSf().get();
  unitSf().get();
}

int main(int argc, char *argv[]) {
  CLI::App app{"Collect Timeouts"};
  app.add_option("-t,--timeout", perFutureTimeout, "Timeout for each Future");
  app.add_option("-c,--collect-timeout", collectTimeout,
                 "Timeout for the collected Future");
  CLI11_PARSE(app, argc, argv);
  printf("perFutureTimeout = %dms\n", perFutureTimeout);
  printf("collectTimeout = %dms\n", collectTimeout);

  //   runSerial();
  // runAsync();
  runCollect();
}

#include "ThreadUtils.h"

#include <chrono>
#include <iostream>
#include <thread>

#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/futures/Future.h>

using namespace std::chrono_literals;

/**
 * Since the code inside deferValue is non-blocking, multiple calls to this
 * function can run concurrently on a CPUThreadPoolExecutor with only one
 * thread.
 *
 * The CPUThreadPoolExecutor will switch to a different call every time the
 * folly::makeSemiFuture().deferValue part is hit, excluding the first time.
 */
folly::SemiFuture<folly::Unit> loopSemiFuture(int label) {
  printf("%sStart of loopSemiFuture %d\n",
         tidLabelStr(std::this_thread::get_id(), "loopSemiFuture").c_str(),
         label);

  return folly::makeSemiFuture().deferValue([label](auto) {
    printf("%sIn deferValue %d\n",
           tidLabelStr(std::this_thread::get_id(), "deferValue").c_str(),
           label);
    std::this_thread::sleep_for(1s);
    return loopSemiFuture(label);
  });
}

/**
 * Since the code inside deferValue blocks forever, multiple calls to this
 * function will only run concurrently provided the CPUThreadPoolExecutor they
 * run on has enough threads.
 *
 * E.g. if 5 calls are made but the CPUThreadPoolExecutor has only 1 thread,
 * only 1 call will execute.
 */
folly::SemiFuture<folly::Unit> loopSemiFutureBlock(int label) {
  printf(
      "%sStart of loopSemiFutureBlock\n",
      tidLabelStr(std::this_thread::get_id(), "loopSemiFutureBlock").c_str());

  return folly::makeSemiFuture().deferValue([label](auto) {
    while (true) {
      printf("%sIn deferValue %d\n",
             tidLabelStr(std::this_thread::get_id(), "deferValue").c_str(),
             label);
      std::this_thread::sleep_for(1s);
    }
  });
}

int main(int argc, char *argv[]) {
  folly::CPUThreadPoolExecutor exec(1);
  loopSemiFuture(1).via(&exec);
  loopSemiFuture(2).via(&exec);
  loopSemiFuture(3).via(&exec);
  loopSemiFuture(4).via(&exec);
  loopSemiFuture(5).via(&exec);
}

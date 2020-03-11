# Folly Futures

## Overview

[https://engineering.fb.com/developer-tools/futures-for-c-11-at-facebook/](https://engineering.fb.com/developer-tools/futures-for-c-11-at-facebook/) has a good explanation.

Here's my really shortened explanation.

- A *future* represents the value of a potentially long running, usually asynchronous computation. E.g. `Future<int>` is an object that will contain an `int` if the computation has completed, but will be "empty" otherwise. Futures are read-only.
- A *promise* is the other end of a future. Once a promise is fulfilled, the corresponding future will contain a value. For example, let's say we have `auto p = Promise<int>()` and `auto f = Future<int>()`. Once we do `p.setValue(5)`, then `f.value() == 5`.
- Here's an analogy. Bill promises Sam $5. However, Bill doesn't have $5. Bill does have $4 in his bank account though, and eventually via increase he will have $5. Instead of waiting around for that sweet interest to come rolling in, Bill instead gives Sam a magic thing called a "cashfuture." It's a block of wood that will magically turn into a five dollar bill when Bill has $5. Thus, Sam can just take the "cashfuture" and continue on with his day.

```
// Takes 5 minutes...
Output fooSync(Input);
    
// Returns immediately!
Future<Output> fooAsync(Input);
    
// Could also do this, but callbacks are annoying.
void fooAsyncCallback(Input, Callback<Output>);
```

## Defer vs. Then

### Run the Demo

    mlim@mlim-mbp systems_programming % buck run //myfolly/folly_futures:defer_vs_then
    Building: finished in 1.2 sec (100%) 7/7 jobs, 0 updated
    Total time: 1.6 sec
    [0x10bce1dc0 main] Start
    [0x10bce1dc0 testDefer] Start
    [0x10bce1dc0 main] After calling testDefer
    [0x10bce1dc0 main] After running sf testDefer
    [0x10bce1dc0 testThen] Start
    [0x70000abf2000 testDefer] Inside defer block
    [0x70000abf2000 testDefer] Looping...
    [0x70000ac75000 testThen] Inside then block
    [0x70000ac75000 testThen] Looping...
    [0x10bce1dc0 main] After calling testThen
    [0x70000ac75000 testThen] Looping...
    [0x70000abf2000 testDefer] Looping...
    [0x70000abf2000 testDefer] Looping...
    [0x70000ac75000 testThen] Looping...
    [0x70000ac75000 testThen] Looping...
    [0x70000abf2000 testDefer] Looping...
    [0x70000abf2000 testDefer] Looping...
    [0x70000ac75000 testThen] Looping...
    [0x70000ac75000 testThen] Looping...

### Some Explanation

`SemiFuture::defer` and `Future::then` are quite similar. The main things to remember are that

1. `SemiFuture::defer` returns another `SemiFuture` and `Future::then` returns another `Future`.
2.  The code in the the function passed to `SemiFuture::defer` will run an executor defined by the caller (e.g. there needs to be a call to `via` (or `get`, `wait`, etc.), since `SemiFuture::defer` returns a `SemiFuture`). The code in the function passed to `Future::then` runs on the executor already attached to the `Future`. 

Some other things to note about this demo:

1. Even though the code in the `defer` and `then` blocks loops forever, we still return immediately from `testDefer` and `testThen`. This is what futures are for! The long running operation is deferred and does not block the main thread.
2. `testThen` takes in an executor, otherwise `testThen` would block. See comments for more details.

## Timeouts

### Run the Demo

    mlim@mlim-mbp systems_programming % buck run //myfolly/folly_futures:timeouts promise
    Building: finished in 4.2 sec (100%) 11/11 jobs, 2 updated
      Total time: 4.7 sec
    [0x70000a02f000 kGlobal] Looping...
    [0x1034c2dc0 testGlobalPromise] Start
    [0x1034c2dc0 main] In kGlobalPromise's interrupt handler!
    [0x1034c2dc0 testGlobalPromise] ===== folly FutureTimeout exception: Timed out =====

    mlim@mlim-mbp systems_programming % buck run //myfolly/folly_futures:timeouts executor
    Building: finished in 3.0 sec (100%) 7/7 jobs, 0 updated
      Total time: 3.5 sec
    [0x7000083b6000 kGlobal] Looping...
    [0x10cedbdc0 testDeferredWhileLoop] Start, useExecutor = 0
    Before calling deferredWhileLoop
    [0x10cedbdc0 deferredWhileLoop] Start
    [0x7000084bc000 deferredWhileLoop] In defer block
    [0x7000084bc000 deferredWhileLoop] Looping...
    [0x7000084bc000 deferredWhileLoop] Looping...
    [0x10cedbdc0 testDeferredWhileLoop] ===== folly FutureTimeout exception: Timed out =====
    [0x7000084bc000 deferredWhileLoop] Looping...
    [0x7000084bc000 deferredWhileLoop] Looping...
    [0x7000084bc000 deferredWhileLoop] Looping...

    mlim@mlim-mbp systems_programming % buck run //myfolly/folly_futures:timeouts noexecutor 
    Building: finished in 3.0 sec (100%) 7/7 jobs, 0 updated
      Total time: 3.6 sec
    [0x700006905000 kGlobal] Looping...
    [0x1178d8dc0 testDeferredWhileLoop] Start, useExecutor = 1
    [0x1178d8dc0 testDeferredWhileLoop] Before calling deferredWhileLoop
    [0x1178d8dc0 deferredWhileLoop] Start
    [0x1178d8dc0 deferredWhileLoop] In defer block
    [0x1178d8dc0 deferredWhileLoop] Looping...
    [0x1178d8dc0 deferredWhileLoop] Looping...
    [0x1178d8dc0 deferredWhileLoop] Looping...
    [0x1178d8dc0 deferredWhileLoop] Looping...

### Some Explanation

#### `testGlobalPromise`

- We can have a timeout without using an `executor` because there is no extra work attached to the `kGlobalSf` (e.g. no `defer` block that would block the main thread).
- Note that simply calling `get` with a timeout does not cause the interrupt handler to be called. However, calling `within(timeout).get()` does cause the interrupt handler to be called.

#### `testDeferredWhileLoop`

- In this case, we need to use an executor in order to use a timeout because the code in the `defer` block blocks. I.e. if we don't use an executor, the code in the deferred while loop runs on the main thread (`0x1178d8dc0` in the example run). When we use `ThreadedExecutor`, we can see that the main thread is `0x10cedbdc0` and the deferred while loop runs on thread `0x7000084bc000`, so the timeout actually works.

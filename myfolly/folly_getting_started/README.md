# Installing Folly on Mac

This is what I did to a) get Folly installed my computer b) get it up and running with Buck. I used [https://github.com/facebook/folly](https://github.com/facebook/folly) as a reference.

## Step 1: Install Folly

`brew install folly`

Then, run `brew info folly` to see where `folly` is installed. For me, it was installed at `/Users/mlim/.brew/Cellar/folly/2020.02.10.00`.

## Step 2: Use Folly in a Test Program

Mimicking [http://www.martinbroadhurst.com/setting-up-facebook-folly-on-ubuntu.html](http://www.martinbroadhurst.com/setting-up-facebook-folly-on-ubuntu.html), let's create a simple test program to try `folly` out. 

Our test program looks like this:

    #include <iostream>
    #include <folly/FBString.h>
     
    int main()
    {
        folly::fbstring str("Hello, Folly World!");
        std::cout << str << "\n";
    }

We can compile it like this:

    clang++ hello.cpp -o hello -I/Users/mlim/.brew/Cellar/folly/2020.02.10.00/include/ -I/Users/mlim/.brew/Cellar/boost/1.72.0/include/ -L/Users/mlim/.brew/Cellar/folly/2020.02.10.00/lib -lfolly --std=c++17

## Step 3: Use Folly With Buck

Now, let's copy in the `folly` and `boost` folders so we can use them with Buck (Buck doesn't allow absolute pathnames, so if we didn't do this, we would get errors if we used a path like `/Users/mlim/.brew/Cellar/folly/2020.02.10.00/include` with Buck). 

    cd systems_programming
    cp -r /Users/mlim/.brew/Cellar/folly folly
    cp -r /Users/mlim/.brew/Cellar/boost boost

Then, we'll create our top-level Buck files.

    touch BUCK
    touch .buckversion && echo last >> .buckversion
    touch .buckconfig

Our top-level `BUCK` file will look like this.

    prebuilt_cxx_library(
      name = "boost",
      header_dirs = [
        "boost/1.72.0/include/",
      ],
      # True for most Boost libraries.
      # Should get a linker error if we use a Boost library that must be built separately.
      header_only = True,
      visibility = [
        'PUBLIC',
      ],
    )
    
    prebuilt_cxx_library(
      name="folly",
      header_dirs = [
        "folly/2020.02.10.00/include/",
      ],
      static_lib = "folly/2020.02.10.00/lib/libfolly.a",
      visibility = [
        'PUBLIC',
      ],
    )

Our `.buckconfig` file will look like this, since [`folly` requires at least C++14](https://github.com/facebook/folly/blob/master/folly/Portability.h). Might as well use the newest one since this is a spiffy new project. 

    [cxx]
      cxxppflags="--std=c++17"

Now, let's build the example we built above with `clang++`, but now using Buck! 

    mkdir folly_getting_started
    cd folly_getting_started
    touch HelloWorld.cpp
    touch BUCK

`HelloWorld.cpp` will contain the same example as above.

The `BUCK` file in `folly_test/` will look like this.

    cxx_binary(
      name = 'hello_world',
      srcs = [
        'HelloWorld.cpp',
      ],
      deps = [
        "//:boost",
        "//:folly",
      ],
    )

And now we can run it using Buck.

    $ buck run //folly_test:hello_world                                                                          -- INSERT --
    Building: finished in 1.3 sec (100%) 4/4 jobs, 0 updated
      Total time: 1.7 sec
    Hello, Folly World!

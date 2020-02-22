# Messing Around With Systems Programming 
This repository contains some code I've written to help me better understand systems programming APIs and concepts, and things that are within the realm of systems programming. Here's some stuff that's covered:

* Unix Domain Sockets
* FlatBuffers
* Folly (Facebook's open source C++ library)
  * Folly Futures
  * Folly Async Sockets

## Running the Code

NOTE: I've only tested this on Mac!

This project uses [Buck](https://buck.build/setup/getting_started.html) to build the code. To install Buck, follow the instructions listed [here](https://buck.build/setup/getting_started.html). TBH though, their instructions aren't the best, so here's what I did:

```
brew tap facebook/fb
// Oracle Java 8 is no longer available for public download b/c of a license change
brew cask install homebrew/cask-versions/adoptopenjdk8
brew install buck

// Set JAVA_HOME environment variable to correct Java version
echo export JAVA_HOME=`/usr/libexec/java_home -v 1.8` >> ~/.zshrc
```

Once you've installed Buck, try running:

```
buck run //sockets:unix_server_socket
```

If that works, you should be good to go.

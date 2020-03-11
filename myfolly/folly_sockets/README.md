# Folly Async Sockets

## Overview

This directory contains code related to Folly Async Sockets. For example, in the simplest demo, a client  `AsyncSocket` connects to an `AsyncServerSocket`. The `AsyncServerSocket` creates a new `AsyncSocket` to talk to the peer `AsyncSocket`. The client socket then sends data obtained via STDIN to its peer, and its peer writes it to STDOUT. More details on this demo can be found below.

Here is a (very) high-level summary of the components used in these demos, obtained from the Folly headers.

- [`AsyncSocket`](https://github.com/facebook/folly/blob/master/folly/io/async/AsyncSocket.h) - allows users to asynchronously wait for data on a socket, and to asynchronously send data.
- [`AsyncServerSocket`](https://github.com/facebook/folly/blob/master/folly/io/async/AsyncServerSocket.h) - a listening socket that asynchronously informs a callback whenever a new connection has been accepted.
- [`AsyncReader::ReadCallback`](https://github.com/facebook/folly/blob/master/folly/io/async/AsyncTransport.h) - invoked by `AsyncSocket` when data is available.
- [`AsyncWriter::WriteCallback`](https://github.com/facebook/folly/blob/master/folly/io/async/AsyncTransport.h) - invoked by `AsyncSocket` when writes succeed/error.
- [`EventBase`](https://github.com/facebook/folly/blob/master/folly/io/async/EventBase.h) - provides a main loop that notifies `EventHandler` callback objects when I/O is ready on a file descriptor, and notifies `AsyncTimeout` objects when a specified timeout has expired. More complex, higher-level callback mechanisms can then be built on top of `EventHandler` and `AsyncTimeout`.

## STDIN / STDOUT Demo

This is the demo described above, in which a client `AsyncSocket` sends data obtained via STDIN to its peer, and its peer writes it to STDOUT. You can run this demo like so:

    $ buck run //folly_sockets:async_server_socket
    Building: finished in 1.6 sec (100%) 9/9 jobs, 0 updated
      Total time: 2.2 sec
    Server socket bound to address = [::1]:52280
    [DEBUG] acceptStarted
    
    // After client is run
    
    [DEBUG] connectionAccepted, fd = 9, clientAddr = [::1]:56917
    [DEBUG] getReadBuffer
    [DEBUG] readDataAvailable, len = 28
    [::1]:56917 sent data: hello, this should be echoed
    [DEBUG] getReadBuffer
    [DEBUG] readDataAvailable, len = 2
    [::1]:56917 sent data: yo

    $ buck run //folly_sockets:async_client_socket
    Building: finished in 2.8 sec (100%) 12/12 jobs, 5 updated
      Total time: 3.2 sec
    Client socket is at address = [::1]:56917
    [DEBUG] connectSuccess
    Waiting for user input...
    hello, this should be echoed
    [DEBUG] writeSuccess
    yo
    [DEBUG] writeSuccess

Note that multiple clients can be run at the same time.

## How It Works

Here's a more in-depth explanation of how this demo works. 

### On the Client Side

1. The client `AsyncSocket` (I'll refer to this socket as "the client" from now on) is constructed with an `EventBase` using `AsyncSocket::newSocket`. The `EventBase` runs the callbacks associated with the `AsyncSocket` in its thread.
2. The client connects to the `AsyncServerSocket` using [`AsyncSocket::connect`](https://our.internmc.facebook.com/intern/codex/symbol/fbcode:folly/AsyncSocket/connect;ConnectCallback*,const_std::string&,uint16/).
3. The client writes to the socket using `AsyncSocket::write`. A callback is passed to `AsyncSocket::write` - on success, that callback's `writeSuccess` function is called on the `EventBase`'s thread. 

### On the Server Side

#### Setup

1. The server `AsyncServerSocket` (I'll refer to this socket as "the server" from now on) is constructed with an `EventBase` using `AsyncServerSocket::newSocket`. The `EventBase` runs the callbacks associated with the `AsyncServerSocket` in its thread.
2. The server is bound to a well-known address.
3. The server listens for incoming connections by calling `AsyncServerSocket::listen`. 
4. The server adds an `AcceptCallback`, which determines what happens when a connection is accepted.
5. The server starts accepting connections by calling `AsyncServerSocket::startAccepting`. 
6. The `EventBase` is put onto a thread and starts running, which means the `AcceptCallback` can start running.

#### Accepting Connections

1. Inside of `AcceptCallback::connectionAccepted`, a new `AsyncSocket` is created. The `connectionAccepted` callback gives us a file descriptor to use. We re-use the `EventBase` from "Setup." This socket will be used to communicate with the client.
2. After constructing the new `AsyncSocket`, we set its read callback to `ReadCallback`.

#### Reading Data

1. When data from the client becomes available, `ReadCallback::getReadBuffer` is invoked to determine where the data gets read to. We just read it into a buffer that belongs to `ReadCallback`.
2. When data has been successfully read into the buffer returned by `ReadCallback::getReadBuffer`, `ReadCallback::readDataAvailable` is called. In this function, we write to STDOUT. 

### How the Callbacks Get Called

By utilizing `AsyncSocket` and `AsyncServerSocket`, our communication is non-blocking. Writes are queued and written asynchronously, and reads are processed via `ReadCallback`. You can compare this to how communication traditionally happens over sockets; there, the call to `read` blocks, as does the call to `write`. I think the difference when reading data is the clearest. Here's what our traditional implementation looks like in `unix_server_socket.c`:

    // Read at most BUF_SIZE bytes from the socket into buf.
    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
      // Then, write those bytes from buf into STDOUT.
      if (write(STDOUT_FILENO, buf, numRead) != numRead) {
        fatal("partial/failed write");
      }
    }

With `AsyncServerSocket`, the corresponding code looks like this:

    void readDataAvailable(size_t len) noexcept override {
      std::cout << kDebugPrefix << "readDataAvailable, len = " << len
                << std::endl;
      buf_[len] = '\0';
      std::cout << label_ << " sent data: " << buf_ << std::endl;
    }

and it gets run by the `EventBase` in a separate thread.

How does this asynchronous stuff all work under the hood? I won't get into too many details, but here's the gist. `EventBase` loops forever on some thread (not the main thread). `EventBase` has a set of file descriptors it is interested in, and a callback associated with each FD. Each time through the loop, we check each FD to see if it is "ready." If so, we call the associated callback. However, we don't use raw file descriptors. `EventHandler` is an object wrapper for file descriptors. Any class we want to receive callbacks on will inherit from `EventHandler`. `AsyncSocket` and `AsyncServerSocket` "implement" `EventHandler`, and so they can receive callbacks. We can think of `AsyncSocket` and `AsyncServerSocket` as fancy file descriptor wrappers. Every time the `EventBase` runs a loop, it checks to see if `AsyncSocket` (or some other FD) is "ready," and if so calls some callback on it (like `ReadCallback`).

///
/// Runs an AsyncServerSocket that can receive connections from multiple
/// client AsyncSockets. It writes the data it receives to STDOUT.
///

#include "AsyncSocket.h"

#include <cstdio>
#include <iostream>
#include <map>

#include <folly/io/async/AsyncServerSocket.h>
#include <folly/io/async/AsyncSocket.h>

class AcceptCallback;
class ReadCallback;

/// A global map of AsyncSockets created in AcceptCallback::connectionAccepted.
/// Keyed by the address of the client socket.
std::unordered_map<std::string, std::shared_ptr<folly::AsyncSocket>>
    commSockets;

/// A global map of ReadCallbacks created in AcceptCallback::connectionAccepted.
/// Keyed by the address of the client socket.
std::unordered_map<std::string, ReadCallback> readCallbacks;

/// Reads data into a privately owned buffer and writes it to STDOUT.
class ReadCallback final : public folly::AsyncReader::ReadCallback {
public:
  ReadCallback(std::string label) : label_(label) {}

  void getReadBuffer(void **bufReturn, size_t *lenReturn) override {
    std::cout << kDebugPrefix << "getReadBuffer" << std::endl;

    *bufReturn = new char[2];
    *lenReturn = 2;
    buf_ = (char *)*bufReturn;
  }

  void readDataAvailable(size_t len) noexcept override {
    std::cout << kDebugPrefix << "readDataAvailable, len = " << len
              << std::endl;
    buf_[len] = '\0';
    std::cout << label_ << " sent data: " << buf_ << std::endl;
  }

  void readEOF() noexcept override {
    std::cout << kDebugPrefix << "readEOF" << std::endl;

    commSockets.erase(label_);
    readCallbacks.erase(label_);
  }

  void readErr(const folly::AsyncSocketException &ex) noexcept override {
    std::cout << kDebugPrefix << "readErr: " << ex.getType() << std::endl;
  }

private:
  /// Label of the socket using this callback.
  const std::string label_;

  /// The buffer data is read into.
  char *buf_ = nullptr;
};

/// Accepts connections and adds them to a global map.
class AcceptCallback final : public folly::AsyncServerSocket::AcceptCallback {
public:
  AcceptCallback(folly::EventBase *evb) : evb_(evb) {}
  void
  connectionAccepted(folly::NetworkSocket fd,
                     const folly::SocketAddress &clientAddr) noexcept override {
    auto clientAddrStr = clientAddr.describe();
    std::cout << kDebugPrefix << "connectionAccepted, fd = " << fd.toFd()
              << ", clientAddr = " << clientAddrStr << std::endl;

    readCallbacks.emplace(clientAddrStr, ReadCallback(clientAddrStr));

    auto commSocket = folly::AsyncSocket::newSocket(evb_, fd);
    commSockets[clientAddrStr] = commSocket;
    commSocket->setReadCB(&readCallbacks.at(clientAddrStr));
  }

  void acceptError(const std::exception &ex) noexcept override {
    std::cout << kDebugPrefix << "acceptError" << std::endl;
  }

  void acceptStarted() noexcept override {
    std::cout << kDebugPrefix << "acceptStarted" << std::endl;
  }

  void acceptStopped() noexcept override {
    std::cout << kDebugPrefix << "acceptStopped" << std::endl;
  }

private:
  /// When a connection is accepted, we create a new AsyncSocket using this
  /// EventBase.
  folly::EventBase *evb_;
};

int main() {
  folly::EventBase evb;

  // Create server socket
  auto serverSocket = folly::AsyncServerSocket::newSocket(&evb);
  folly::SocketAddress serverSocketAddr(kServerAddr, kServerPort);
  std::cout << "Server socket bound to address = "
            << serverSocketAddr.describe() << std::endl;

  // Bind and listen. After this, socket is ready to start accepting
  // connections.
  serverSocket->bind(serverSocketAddr);
  serverSocket->listen(kSocketBacklog);

  // Add the accept callback and start accepting connections.
  AcceptCallback acceptCallback(&evb);
  serverSocket->addAcceptCallback(&acceptCallback, &evb);
  serverSocket->startAccepting();

  // Run the EventBase so our callbacks can run.
  auto thread = std::thread([&] { evb.loopForever(); });
  evb.waitUntilRunning();

  // Block the main thread.
  while (1) {
  }
}

#include <cstdio>
#include <iostream>
#include <map>

#include <folly/io/async/AsyncServerSocket.h>
#include <folly/io/async/AsyncSocket.h>

constexpr auto kServerAddr = "::1";
constexpr auto kServerPort = 52280;
constexpr auto kSocketBacklog = 5;

class AcceptCallback;
class ReadCallback;

std::string kDebugPrefix = "[DEBUG] ";
folly::EventBase evb;
char *buf = new char[500];

std::unordered_map<std::string, std::shared_ptr<folly::AsyncSocket>>
    commSockets;
std::unordered_map<std::string, ReadCallback> readCallbacks;

class ReadCallback final : public folly::AsyncReader::ReadCallback {
public:
  ReadCallback(std::string descriptor) : descriptor_(descriptor) {}

  void getReadBuffer(void **bufReturn, size_t *lenReturn) override {
    std::cout << kDebugPrefix << "getReadBuffer" << std::endl;

    *bufReturn = buf;
    *lenReturn = 1000;
  }

  void readDataAvailable(size_t len) noexcept override {
    std::cout << kDebugPrefix << "readDataAvailable, len = " << len
              << std::endl;
    buf[len] = '\0';
    std::cout << descriptor_ << " sent data: " << buf << std::endl;
  }

  void readEOF() noexcept override {
    std::cout << kDebugPrefix << "readEOF" << std::endl;

    commSockets.erase(descriptor_);
    readCallbacks.erase(descriptor_);
  }

  void readErr(const folly::AsyncSocketException &ex) noexcept override {
    std::cout << kDebugPrefix << "readErr: " << ex.getType() << std::endl;
  }

  /// Descriptor for socket using this callback.
  const std::string descriptor_;
};

class AcceptCallback final : public folly::AsyncServerSocket::AcceptCallback {
  void
  connectionAccepted(folly::NetworkSocket fd,
                     const folly::SocketAddress &clientAddr) noexcept override {
    auto clientAddrStr = clientAddr.describe();
    std::cout << kDebugPrefix << "connectionAccepted, fd = " << fd.toFd()
              << ", clientAddr = " << clientAddrStr << std::endl;

    readCallbacks.emplace(clientAddrStr, ReadCallback(clientAddrStr));
    auto commSocket = folly::AsyncSocket::newSocket(&evb, fd);
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
};

int main() {
  auto serverSocket = folly::AsyncServerSocket::newSocket(&evb);
  folly::SocketAddress serverSocketAddr(kServerAddr, kServerPort);
  std::cout << "Server socket bound to address = "
            << serverSocketAddr.describe() << std::endl;

  serverSocket->bind(serverSocketAddr);
  serverSocket->listen(kSocketBacklog);

  AcceptCallback acceptCallback;
  serverSocket->addAcceptCallback(&acceptCallback, &evb);
  serverSocket->startAccepting();

  auto thread = std::thread([] { evb.loopForever(); });
  evb.waitUntilRunning();
  while (1) {
  }
}

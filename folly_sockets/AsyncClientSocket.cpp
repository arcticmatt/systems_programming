///
/// Runs a client AsyncSocket that connects to an AsyncServerSocket.
/// The AsyncSocket will write data obtained from the user via STDIN.
///

#include "AsyncSocket.h"

#include <iostream>
#include <string>

#include <folly/io/async/AsyncSocket.h>

class ConnectCallback final : public folly::AsyncSocket::ConnectCallback {
public:
  void connectErr(const folly::AsyncSocketException &ex) noexcept override {
    std::cout << kDebugPrefix << "connectError: " << ex.what() << std::endl;
    state = State::ERROR;
    error = ex.what();
  }

  void connectSuccess() noexcept override {
    std::cout << kDebugPrefix << "connectSuccess" << std::endl;
    state = State::SUCCESS;
  }

  enum class State { WAITING, SUCCESS, ERROR };

  State state{State::WAITING};
  std::string error;
};

class WriteCallback final : public folly::AsyncWriter::WriteCallback {
public:
  void writeSuccess() noexcept override {
    std::cout << kDebugPrefix << "writeSuccess" << std::endl;
  }

  void writeErr(size_t nBytesWritten,
                const folly::AsyncSocketException &ex) noexcept override {
    std::cout << kDebugPrefix << "writeErr" << std::endl;
  }
};

int main() {
  folly::EventBase evb;

  // Create the client socket.
  auto clientSocket = folly::AsyncSocket::newSocket(&evb);

  // Connect the socket.
  ConnectCallback connectCallback;
  folly::SocketAddress serverSocketAddr(kServerAddr, kServerPort);
  clientSocket->connect(&connectCallback, serverSocketAddr, 1000);

  // Print the socket's local address for debugging.
  folly::SocketAddress clientSocketAddr;
  clientSocket->getLocalAddress(&clientSocketAddr);
  std::cout << "Client socket is at address = " << clientSocketAddr.describe()
            << std::endl;

  // Connect synchronously! loops until we process the connect callback.
  evb.loop();
  if (connectCallback.state != ConnectCallback::State::SUCCESS) {
    std::cerr << "ERROR! Failed to connect: " << connectCallback.error
              << std::endl;
    return -1;
  }

  // Now, run the EventBase in another thread.
  auto thread = std::thread([&] { evb.loopForever(); });
  evb.waitUntilRunning();

  // Read from STDIN and write to the socket.
  WriteCallback writeCallback;
  std::cout << "Waiting for user input..." << std::endl;
  for (std::string line; std::getline(std::cin, line);) {
    const uint8_t *data = reinterpret_cast<const uint8_t *>(line.c_str());
    clientSocket->write(&writeCallback, data, line.size());
  }

  // Block the main thread.
  while (1) {
  }
}

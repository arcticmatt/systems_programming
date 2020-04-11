#include <chrono>
#include <iostream>
#include <string>

class PrefixTimeBuf : public std::streambuf {
  std::streambuf *sbuf;
  bool needPrefix;

  std::string getPrefix() const {
    return "%" +
           std::to_string(
               std::chrono::steady_clock::now().time_since_epoch().count()) +
           "%";
  }

  int sync() { return this->sbuf->pubsync(); }

  int overflow(int c) {
    if (c != std::char_traits<char>::eof()) {
      auto prefix = getPrefix();
      if (this->needPrefix && !prefix.empty() &&
          prefix.size() != this->sbuf->sputn(&prefix[0], prefix.size())) {
        return std::char_traits<char>::eof();
      }
      this->needPrefix = c == '\n';
    }
    return this->sbuf->sputc(c);
  }

public:
  PrefixTimeBuf(std::streambuf *sbuf) : sbuf(sbuf), needPrefix(true) {}
};

int main() {
  std::cout << "hello world" << std::endl;
  std::streambuf *backup = std::cout.rdbuf();
  std::streambuf *prefixBuf = new PrefixTimeBuf(backup);
  std::cout.rdbuf(prefixBuf);
  std::cout << "testing" << std::endl;
  std::cout << "one more time" << std::endl;
}

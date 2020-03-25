#include <cctype>
#include <iostream>
#include <memory>

#include <folly/Range.h>
#include <folly/io/IOBuf.h>

#include <gtest/gtest.h>

void append(std::unique_ptr<folly::IOBuf> &buf, folly::StringPiece str) {
  EXPECT_LE(str.size(), buf->tailroom());
  memcpy(buf->writableData(), str.data(), str.size());
  buf->append(str.size());
}

void prepend(std::unique_ptr<folly::IOBuf> &buf, folly::StringPiece str) {
  EXPECT_LE(str.size(), buf->headroom());
  memcpy(buf->writableData() - str.size(), str.data(), str.size());
  buf->prepend(str.size());
}

void printBufInfo(std::unique_ptr<folly::IOBuf> &buf, const std::string &name,
                  bool printPointers = false) {
  std::cout << "===== " << name << " =====" << std::endl;

  if (printPointers) {
    std::cout << "buf->data()      = " << (int *)buf->data() << std::endl;
    std::cout << "buf->buffer()    = " << (int *)buf->buffer() << std::endl;
    std::cout << "buf->tail()      = " << (int *)buf->tail() << std::endl;
    std::cout << "buf->bufferEnd() = " << (int *)buf->bufferEnd() << std::endl;
    std::cout << std::endl;
  }

  std::cout << "buf->capacity() = " << buf->capacity() << std::endl;
  std::cout << "buf->length()   = " << buf->length() << std::endl;
  std::cout << "buf->headroom() = " << buf->headroom() << std::endl;
  std::cout << "buf->tailroom() = " << buf->tailroom() << std::endl;
  std::cout << std::endl;

  std::cout << "buf->isShared() = " << (buf->isShared() ? "true" : "false")
            << std::endl;
  std::cout << "buf->isChained() = " << (buf->isChained() ? "true" : "false")
            << std::endl;
  std::cout << std::endl;

  for (int i = 0; i < buf->capacity(); i++) {
    if (std::isalnum((int)buf->buffer()[i])) {
      std::cout << (char)buf->buffer()[i] << " ";
    } else {
      std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right
                << std::hex << (int)buf->buffer()[i] << std::dec << " ";
    }
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

TEST(IOBufTest, Simple) {
  std::size_t originalCapacity = 20;
  std::unique_ptr<folly::IOBuf> buf(folly::IOBuf::create(originalCapacity));

  printBufInfo(buf, "buf");

  uint32_t cap = buf->capacity();
  std::cout << "memset buf to 0" << std::endl;
  memset(buf->writableBuffer(), 0, cap);
  printBufInfo(buf, "buf");

  EXPECT_LE(originalCapacity, cap);
  EXPECT_EQ(0, buf->headroom());
  EXPECT_EQ(0, buf->length());
  EXPECT_EQ(cap, buf->tailroom());

  std::cout << "append world to buf" << std::endl;
  append(buf, "world");
  printBufInfo(buf, "buf");
  std::cout << "advance buf by 10" << std::endl;
  buf->advance(10);
  printBufInfo(buf, "buf");
  EXPECT_EQ(10, buf->headroom());
  EXPECT_EQ(5, buf->length());
  EXPECT_EQ(cap - 15, buf->tailroom());

  std::cout << "prepend hello to buf" << std::endl;
  prepend(buf, "hello ");
  printBufInfo(buf, "buf");
  EXPECT_EQ(4, buf->headroom());
  EXPECT_EQ(11, buf->length());
  EXPECT_EQ(cap - 15, buf->tailroom());

  const char *p = reinterpret_cast<const char *>(buf->data());
  EXPECT_EQ("hello world", std::string(p, buf->length()));

  std::cout << "clear buf" << std::endl;
  buf->clear();
  printBufInfo(buf, "buf");
  EXPECT_EQ(0, buf->headroom());
  EXPECT_EQ(0, buf->length());
  EXPECT_EQ(cap, buf->tailroom());
}

TEST(IOBufTest, Clone) {
  std::size_t originalCapacity = 20;
  std::unique_ptr<folly::IOBuf> buf(folly::IOBuf::create(originalCapacity));

  printBufInfo(buf, "buf");

  uint32_t cap = buf->capacity();
  std::cout << "memset buf to 0" << std::endl;
  memset(buf->writableBuffer(), 0, cap);
  printBufInfo(buf, "buf");

  EXPECT_LE(originalCapacity, cap);
  EXPECT_EQ(0, buf->headroom());
  EXPECT_EQ(0, buf->length());
  EXPECT_EQ(cap, buf->tailroom());

  std::cout << "append world to buf" << std::endl;
  append(buf, "world");
  printBufInfo(buf, "buf");
  std::cout << "advance buf by 10" << std::endl;
  buf->advance(10);
  printBufInfo(buf, "buf");
  EXPECT_EQ(10, buf->headroom());
  EXPECT_EQ(5, buf->length());
  EXPECT_EQ(cap - 15, buf->tailroom());

  std::cout << "prepend hello to buf" << std::endl;
  prepend(buf, "hello ");
  printBufInfo(buf, "buf");
  EXPECT_EQ(4, buf->headroom());
  EXPECT_EQ(11, buf->length());
  EXPECT_EQ(cap - 15, buf->tailroom());

  auto cloneBuf = buf->clone();
  printBufInfo(cloneBuf, "cloneBuf");
  cloneBuf->trimStart(3);
  cloneBuf->append(3);
  printBufInfo(buf, "buf");
  printBufInfo(cloneBuf, "cloneBuf");

  auto ptr = cloneBuf->writableData();
  *ptr = 's';
  printBufInfo(buf, "buf");
  printBufInfo(cloneBuf, "cloneBuf");

  buf->unshare();
  printBufInfo(buf, "buf");
  printBufInfo(cloneBuf, "cloneBuf");
}

TEST(IOBufTest, Wrap) {
  uint8_t *buffer = (uint8_t *)malloc(12);
  auto iobuf = folly::IOBuf::wrapBuffer(buffer, 0);
  printBufInfo(iobuf, "iobuf");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

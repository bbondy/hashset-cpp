#include "math.h"
#include <string.h>

class ExampleData {
public:
  uint64_t hash() const {
    uint64_t total = 0;
    int prime = 19;
    for (int i = 0; i < dataLen; i++) {
      total += data[i] * pow(prime, dataLen - i - 1);
    }
    return total;
  }

  ~ExampleData() {
    if (data && !borrowedMemory) {
      delete[] data;
    }
  }
  ExampleData(const char *data) {
    dataLen = strlen(data) + 1;
    this->data = new char[dataLen];
    memcpy(this->data, data, dataLen);
    borrowedMemory = false;
  }

  ExampleData(const char *data, int dataLen) {
    this->dataLen = dataLen;
    this->data = new char[dataLen];
    memcpy(this->data, data, dataLen);
    borrowedMemory = false;
  }

  ExampleData(const ExampleData &rhs) {
    this->dataLen = rhs.dataLen;
    data = new char[dataLen];
    memcpy(data, rhs.data, dataLen);
    borrowedMemory = rhs.borrowedMemory;
  }

  ExampleData() : data(nullptr), dataLen(0), borrowedMemory(false) {
  }

  bool operator==(const ExampleData &rhs) const {
    if (dataLen != rhs.dataLen) {
      return false;
    }

    return !memcmp(data, rhs.data, dataLen);
  }

  bool operator!=(const ExampleData &rhs) const {
    return !(*this == rhs);
  }

  uint32_t serialize(char *buffer) {
    uint32_t totalSize = 0;
    char sz[32];
    uint32_t dataLenSize = 1 + sprintf(sz, "%x", dataLen);
    if (buffer) {
      memcpy(buffer + totalSize, sz, dataLenSize);
    }
    totalSize += dataLenSize;
    if (buffer) {
      memcpy(buffer + totalSize, data, dataLen);
    }
    totalSize += dataLen;
    return totalSize;
  }

  uint32_t deserialize(char *buffer, uint32_t bufferSize) {
    dataLen = 0;
    if (!hasNewlineBefore(buffer, bufferSize)) {
      return 0;
    }
    sscanf(buffer, "%x", &dataLen);
    uint32_t consumed = strlen(buffer) + 1;
    if (consumed + dataLen >= bufferSize) {
      return 0;
    }
    data = buffer + consumed;
    borrowedMemory = true;
    memcpy(data, buffer + consumed, dataLen);
    consumed += dataLen;
    return consumed;
  }

private:
  bool hasNewlineBefore(char *buffer, uint32_t bufferSize) {
    char *p = buffer;
    for (uint32_t i = 0; i < bufferSize; ++i) {
      if (*p == '\0')
        return true;
      p++;
    }
    return false;
  }

  char *data;
  uint32_t dataLen;
  bool borrowedMemory;
};


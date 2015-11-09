class HashItem {
public:
  HashItem() : next(nullptr), data(nullptr), dataLen(0), borrowedMemory(false) {
  }

  ~HashItem() {
    if (!borrowedMemory && data) {
      delete[] data;
    }
  }

  HashItem *next;
  char *data;
  uint32_t dataLen;
  bool borrowedMemory;
};

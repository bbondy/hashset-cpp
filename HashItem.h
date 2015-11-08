class HashItem {
public:
  HashItem() : next(nullptr), data(nullptr), dataLen(0) {
  }

  HashItem *next;
  char *data;
  uint32_t dataLen;
};

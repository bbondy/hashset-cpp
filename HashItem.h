template<class T>
class HashItem {
public:
  HashItem() : next(nullptr), hashItemStorage(nullptr) {
  }

  ~HashItem() {
    if (hashItemStorage) {
      delete hashItemStorage;
    }
  }

  HashItem *next;
  T *hashItemStorage;
};

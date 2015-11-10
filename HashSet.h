// You should probably not be using this.  This is only useful in environments
// without std lib and having specific serialization and memory requirements.
// Instead consider using `hash_set` which is a more generic implementation with templates.

#pragma once

#include <stdint.h>
#include <math.h>
#include "hashFn.h"
#include "hashItem.h"

extern HashFn defaultHashSetFn;

class HashSet {
public:
  HashSet(uint32_t bucketCount = 256, HashFn hashFn = defaultHashSetFn);
  HashSet(char *buffer, HashFn hashFn = defaultHashSetFn);

  ~HashSet();

  bool add(const char *data, uint32_t len);
  bool add(const char *sz);
  bool exists(const char *data, uint32_t len);
  bool exists(const char *sz);

  uint32_t size() {
    return _size;
  }

  // Serializes a the parsed data and bloom filter data into a single buffer.
  // The returned buffer should be deleted by the caller.
  // The size is returned in the out parameter if it's needed to write to a file.
  char *serialize(int &size);
  // Deserializes the buffer, a size is not needed since a serialized buffer is self described
  // Memory passed in will be used by this instance directly without copying it in.
  void deserialize(char *);

private:
  void cleanup();
  uint32_t serializeBuckets(char *buffer);
  uint32_t bucketCount;
  HashFn hashFn;
  HashItem **buckets;
  uint32_t _size;
};

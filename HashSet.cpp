#include "HashSet.h"
#include <string.h>
#include <stdio.h>

HashFn defaultHashSetFn = HashFn(17);

HashSet::HashSet(uint32_t bucketCount, HashFn hashFn)
  : bucketCount(bucketCount), hashFn(hashFn), buckets(nullptr), _size(0) {
  buckets = new HashItem*[bucketCount];
  memset(buckets, 0, sizeof(HashItem*) * bucketCount);
}

HashSet::~HashSet() {
  cleanup();
}

void HashSet::cleanup() {
  if (buckets) {
    for (uint32_t i = 0; i < bucketCount; i++) {
       HashItem *hashItem = buckets[i];
       while (hashItem) {
         HashItem *tempHashItem = hashItem;
         hashItem = hashItem->next;
         delete tempHashItem;
       }
    }
    delete[] buckets;
    buckets = nullptr;
    _size = 0;
  }
}

/**
 * Adds the specified data if it doesn't exist
 * A copy of the data will be created, so the memory used to do the add doesn't need to stick around.
 *
 * @param data The binary data to add
 * @param len The length of the binary data to add
 * @return true if the data was added
 */
bool HashSet::add(const char *data, uint32_t len) {
  if (!data) {
    return false;
  }

  uint64_t hash = hashFn(data, len);
  HashItem *hashItem = buckets[hash % bucketCount];
  if (!hashItem) {
    hashItem = new HashItem();
    buckets[hash % bucketCount] = hashItem;
    hashItem->data = new char[len];
    memcpy(hashItem->data, data, len);
    hashItem->dataLen = len;
    _size++;
    return true;
  }

  while (true) {
    if (hashItem->dataLen == len && !memcmp(hashItem->data, data, len)) {
      return false;
    }
    if (!hashItem->next) {
      HashItem *newHashItem = new HashItem();
      newHashItem->data = new char[len];
      memcpy(newHashItem->data, data, len);
      newHashItem->dataLen = len;
      hashItem->next = newHashItem;
      break;
    }
    hashItem = hashItem->next;
  }

  _size++;
  return true;
}

/**
 * Same as above but assumes data is a null terminated string
 */
bool HashSet::add(const char *sz) {
  return add(sz, strlen(sz) + 1);
}

/**
 * Determines if the specified data exists in the set or not`
 * @param data The binary data to check
 * @param len The length of the binary data to acheck
 * @return true if the data found
 */
bool HashSet::exists(const char *data, uint32_t len) {
  if (!data) {
    return false;
  }

  uint64_t hash = hashFn(data, len);
  HashItem *hashItem = buckets[hash % bucketCount];
  if (!hashItem) {
    return false;
  }

  while (hashItem) {
    if (hashItem->dataLen == len && !memcmp(hashItem->data, data, len)) {
      return true;
    }
    hashItem = hashItem->next;
  }

  return false;
}

/**
 * Same as above but assumes data is a null terminated string
 */
bool HashSet::exists(const char *sz) {
  return exists(sz, strlen(sz) + 1);
}


uint32_t HashSet::serializeBuckets(char *buffer) {
  uint32_t totalSize = 0;
  char sz[512];
  totalSize += 1 + sprintf(sz, "%x", bucketCount);
  if (buffer) {
    memcpy(buffer, sz, totalSize);
  }
  for (uint32_t i = 0; i < bucketCount; i++) {
    HashItem *hashItem = buckets[i];
    while (hashItem) {
      uint32_t dataLenSize = 1 + sprintf(sz, "%x", hashItem->dataLen);
      if (buffer) {
        memcpy(buffer + totalSize, sz, dataLenSize);
      }
      totalSize += dataLenSize;
      if (buffer) {
        memcpy(buffer + totalSize, hashItem->data, hashItem->dataLen);
      }
      totalSize += hashItem->dataLen;
      hashItem = hashItem->next;
    }
    if (buffer) {
      buffer[totalSize] = '\0';
    }
    // Second null terminator to show next bucket
    totalSize++;
  }
  return totalSize;
}

/**
 * Serializes a the parsed data and bloom filter data into a single buffer.
 * @param size The size is returned in the out parameter if it's needed to write to a file.
 * @return The returned buffer should be deleted by the caller.
 */
char * HashSet::serialize(int &size) {
  size = 0;
  size += serializeBuckets(nullptr);
  char *buffer = new char[size];
  memset(buffer, 0, size);
  serializeBuckets(buffer);
  return buffer;
}

/**
 * Deserializes the buffer, a size is not needed since a serialized buffer is self described
 * Memory passed in will be used by this instance directly without copying it in.
 */
void HashSet::deserialize(char *buffer) {
  cleanup();
  uint32_t pos = 0;
  sscanf(buffer + pos, "%x", &bucketCount);
  buckets = new HashItem *[bucketCount];
  memset(buckets, 0, sizeof(HashItem*) * bucketCount);
  pos += strlen(buffer + pos) + 1;
  for (uint32_t i = 0; i < bucketCount; i++) {
    HashItem *lastHashItem = nullptr;
    while (*(buffer + pos) != '\0') {
      uint32_t dataLen = 0;
      sscanf(buffer + pos, "%x", &dataLen);
      pos += strlen(buffer + pos) + 1;
      HashItem *hashItem = new HashItem();
      _size++;
      hashItem->dataLen = dataLen;
      hashItem->data = buffer + pos;
      hashItem->borrowedMemory = true;
      memcpy(hashItem->data, buffer + pos, dataLen);
      pos += dataLen;
      if (lastHashItem) {
        lastHashItem->next = hashItem;
      } else {
        buckets[i] = hashItem;
      }
      lastHashItem = hashItem;
    }
    pos++;
  }
}

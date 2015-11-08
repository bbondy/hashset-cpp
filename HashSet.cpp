#include "HashSet.h"
#include <string.h>

HashFn defaultHashSetFn = HashFn(17);

HashSet::HashSet(uint32_t bucketCount, HashFn hashFn)
  : bucketCount(bucketCount), hashFn(hashFn), buckets(nullptr), _size(0) {
  buckets = new HashItem*[bucketCount];
  memset(buckets, 0, sizeof(HashItem*) * bucketCount);
}

HashSet::~HashSet() {
  if (buckets) {
    for (uint32_t i = 0; i < bucketCount; i++) {
       HashItem *hashItem = buckets[i];
       while (hashItem) {
         HashItem *tempHashItem = hashItem;
         hashItem = hashItem->next;
         delete[] tempHashItem->data;
         delete tempHashItem;
       }
    }
    delete[] buckets;
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


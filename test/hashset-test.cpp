#include "CppUnitLite/TestHarness.h"
#include "CppUnitLite/Test.h"
#include "HashSet.h"
#include <iostream>

using namespace std;

TEST(hashSet, test1)
{
  HashSet hashSets[] = {HashSet(1), HashSet(2), HashSet(500)};
  for (unsigned int i = 0; i < sizeof(hashSets) / sizeof(hashSets[0]); i++) {
    HashSet &hashSet = hashSets[i];
    CHECK(!hashSet.exists("test"));
    CHECK(hashSet.size() == 0);
    hashSet.add("test");
    CHECK(hashSet.size() == 1);
    CHECK(hashSet.exists("test"));
    hashSet.add("test");
    CHECK(hashSet.exists("test"));
    CHECK(hashSet.size() == 1);
    hashSet.add("test2");
    CHECK(hashSet.exists("test2"));
    CHECK(hashSet.size() == 2);
    hashSet.add("test3");
    CHECK(hashSet.exists("test3"));
    hashSet.add("test4");
    CHECK(hashSet.exists("test4"));

    // Check that a smaller substring of something that exists, doesn't exist
    CHECK(!hashSet.exists("tes"));
    // Check that a longer string of something that exists, doesn't exist
    CHECK(!hashSet.exists("test22"));
    CHECK(!hashSet.exists("test5"));

    // Check that binary data works
    CHECK(hashSet.size() == 4);
    hashSet.add("a\0b\0\0c", 6);
    CHECK(hashSet.size() == 5);
    CHECK(!hashSet.exists("a"));
    CHECK(!hashSet.exists("a", 1));
    CHECK(hashSet.exists("a\0b\0\0c", 6));
    CHECK(!hashSet.exists("a\0b\0\0c", 7));
  }

  int len = 0;
  for (unsigned int i = 0; i < sizeof(hashSets) / sizeof(hashSets[0]); i++) {
    HashSet &hs1 = hashSets[i];
    char *buffer = hs1.serialize(len);
    HashSet hs2;
    hs2.deserialize(buffer);
    HashSet hs3(buffer);
    HashSet *deserializedHashSets[2] = {&hs2, &hs3};
    for (unsigned int i = 0; i < sizeof(deserializedHashSets) / sizeof(deserializedHashSets[0]); i++) {
      HashSet &dhs = *deserializedHashSets[i];
      CHECK(dhs.exists("test"));
      CHECK(dhs.exists("test2"));
      CHECK(dhs.exists("test3"));
      CHECK(dhs.exists("test4"));
      CHECK(!dhs.exists("tes"));
      CHECK(!dhs.exists("test22"));
      CHECK(!dhs.exists("test5"));
      CHECK(!dhs.exists("a"));
      CHECK(!dhs.exists("a", 1));
      CHECK(dhs.exists("a\0b\0\0c", 6));
      CHECK(!dhs.exists("a\0b\0\0c", 7));
      CHECK(dhs.size() == 5);
    }
    delete[] buffer;
  }
}

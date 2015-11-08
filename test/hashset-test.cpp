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
}

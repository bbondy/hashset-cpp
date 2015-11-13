#include "CppUnitLite/TestHarness.h"
#include "CppUnitLite/Test.h"
#include "HashSet.h"
#include "exampleData.h"
#include <iostream>

using namespace std;

TEST(hashSet, test1)
{
  {
    HashSet<ExampleData> hashSet(2);
    hashSet.add(ExampleData("test"));
    uint32_t len;
    char *buffer = hashSet.serialize(len);
    HashSet<ExampleData> hashSet2(0);
    hashSet2.deserialize(buffer, len);
    hashSet2.exists("test");
  }

  HashSet<ExampleData> hashSets[] = {HashSet<ExampleData>(1), HashSet<ExampleData>(2), HashSet<ExampleData>(500)};
  for (unsigned int i = 0; i < sizeof(hashSets) / sizeof(hashSets[0]); i++) {
    HashSet<ExampleData> &hashSet = hashSets[i];
    CHECK(hashSet.size() == 0);
    hashSet.add(ExampleData("test"));
    CHECK(hashSet.size() == 1);
    CHECK(hashSet.exists(ExampleData("test")));
    hashSet.add(ExampleData("test"));
    CHECK(hashSet.exists(ExampleData("test")));
    CHECK(hashSet.size() == 1);
    hashSet.add(ExampleData("test2"));
    CHECK(hashSet.exists(ExampleData("test2")));
    CHECK(hashSet.size() == 2);
    hashSet.add(ExampleData("test3"));
    CHECK(hashSet.exists(ExampleData("test3")));
    hashSet.add(ExampleData("test4"));
    CHECK(hashSet.exists(ExampleData("test4")));

    // Check that a smaller substring of something that exists, doesn't exist
    CHECK(!hashSet.exists(ExampleData("tes")));
    // Check that a longer string of something that exists, doesn't exist
    CHECK(!hashSet.exists(ExampleData("test22")));
    CHECK(!hashSet.exists(ExampleData("test5")));

    CHECK(hashSet.size() == 4);
    hashSet.add(ExampleData("a\0b\0\0c", 6));
    CHECK(hashSet.size() == 5);
    CHECK(!hashSet.exists("a"));
    CHECK(!hashSet.exists(ExampleData("a", 1)));
    CHECK(hashSet.exists(ExampleData("a\0b\0\0c", 6)));
    CHECK(!hashSet.exists(ExampleData("a\0b\0\0c", 7)));
  }

  uint32_t len = 0;
  for (unsigned int i = 0; i < sizeof(hashSets) / sizeof(hashSets[0]); i++) {
    HashSet<ExampleData> &hs1 = hashSets[i];
    char *buffer = hs1.serialize(len);
    HashSet<ExampleData> dhs(0);
    // Deserializing some invalid data should fail
    CHECK(!dhs.deserialize(const_cast<char*>("31131"), 2));
    CHECK(dhs.deserialize(buffer, len));
    CHECK(dhs.exists("test"));
    CHECK(dhs.exists("test2"));
    CHECK(dhs.exists("test3"));
    CHECK(dhs.exists("test4"));
    CHECK(!dhs.exists("tes"));
    CHECK(!dhs.exists("test22"));
    CHECK(!dhs.exists("test5"));
    CHECK(!dhs.exists("a"));
    CHECK(!dhs.exists(ExampleData("a", 1)));
    CHECK(dhs.exists(ExampleData("a\0b\0\0c", 6)));
    CHECK(!dhs.exists(ExampleData("a\0b\0\0c", 7)));
    CHECK(dhs.size() == 5);
    delete[] buffer;
  }
}

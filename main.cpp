#include "HashSet.h"
#include <iostream>

#include "test/exampleData.h"

using namespace std;

int main(int argc, char **argv) {
  HashSet<ExampleData> set(256);
  set.add(ExampleData("test"));

  // Prints true
  cout << "test exists: " << (set.exists(ExampleData("test")) ? "true" : "false") << endl;
  // Prints false
  cout << "test2 exists: " << (set.exists(ExampleData("test2")) ? "true" : "false") << endl;

  uint32_t len;
  char * buffer = set.serialize(len);
  HashSet<ExampleData> set2(0);
  set2.deserialize(buffer, len);
  // Prints true
  cout << "test exists: " << (set2.exists("test") ? "true" : "false") << endl;
  // Prints false
  cout << "test2 exists: " << (set2.exists("test2") ? "true" : "false") << endl;

  delete[] buffer;
  return 0;
}

#include "HashSet.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  HashSet set;
  set.add("test");
  // Prints true
  cout << "test exists: " << (set.exists("test") ? "true" : "false") << endl;
  // Prints false
  cout << "test2 exists: " << (set.exists("test2") ? "true" : "false") << endl;
  return 0;
}

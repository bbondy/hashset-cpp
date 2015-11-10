#hashset-cpp

Implements a simple HashSet for strings in environments where you don't have the std lib available.
You should probably not be using this. Instead consider using `hash_set` which is a more generic implementation with templates.
This is only useful for very specific use cases having specific memory layout requirements.

## Setup

```
npm install --save hashset-cpp
```

## Sample

```c++
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

  int len;
  HashSet set2(set.serialize(len));
  // Prints true
  cout << "test exists: " << (set2.exists("test") ? "true" : "false") << endl;
  // Prints false
  cout << "test2 exists: " << (set2.exists("test2") ? "true" : "false") << endl;
  return 0;
}
```

## Build everything in release

```
make
```

## Build everything in debug

```
make build-debug
```

## Running sample

```
make sample
```

## Running tests

```
make test
```

## Clearing build files
```
make clean
```

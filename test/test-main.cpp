#include "CppUnitLite/TestHarness.h"
#include <string>

SimpleString StringFrom(const std::string& value)
{
    return SimpleString(value.c_str());
}

int main()
{
  TestResult tr;
  TestRegistry::runAllTests(tr);
  return 0;
}

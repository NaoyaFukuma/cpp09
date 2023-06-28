#include <iostream>

template <typename T>
void assert_equal(T expected, T actual, const char *func, int line) {
  if (expected == actual) {
    std::cout << "\033[32mOK\033[0m: " << func << ", line: " << line << std::endl;
  } else {
    std::cout << "\033[31mNG\033[0m: " << func << ", line: " << line << std::endl;
    std::cout << "  expected: " << expected << ", actual: " << actual << std::endl;
  }
}

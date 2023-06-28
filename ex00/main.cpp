#include "define.hpp"
#include "assert_equal.tpp"
#include <iostream>

int main() {
  std::cout << "Hello, World!" << std::endl;
  DEBUG_PRINT("DEBUGGGGG!\n");
  #ifdef UNIT_TEST
  assert_equal(1, 1, __func__, __LINE__);
  assert_equal(1, 2, __func__, __LINE__);
  #endif // UNIT_TEST
  return 0;
}

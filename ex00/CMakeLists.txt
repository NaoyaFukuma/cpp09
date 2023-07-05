cmake_minimum_required(VERSION 3.10)

project(BitcoinExchange)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

find_package(GTest REQUIRED)

add_executable(BitcoinExchangeTest test.cpp BitcoinExchange.cpp)
target_link_libraries(BitcoinExchangeTest ${GTEST_BOTH_LIBRARIES})

GTEST_ADD_TESTS(BitcoinExchangeTest "" test.cpp)

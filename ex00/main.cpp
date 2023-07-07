#include "BitcoinExchange.hpp"
#include <iostream>

#define DEFAULT_DB "./DB/data.csv"

static void printUsage();

int main(int argc, char *argv[]) {
  std::string db_file = DEFAULT_DB;
  switch (argc) {
  case 1:
    printUsage();
    return 1;

  case 2:
    break;

  case 3:
    db_file = argv[2];
    break;

  default:
    break;
  }

  try {
    BitcoinExchange bitcoinExchange(db_file);
    bitcoinExchange.processInputFile(argv[1]);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}

static void printUsage() {
  std::cout << "Usage: ./bitcoin_exchange [input_file]" << std::endl;
  std::cout << "\t# If the data file is omitted, the relative path "
               "\"DB/data.csv\" is used."
            << std::endl;
  std::cout << "Usage: ./bitcoin_exchange [input_file] [DB_file]" << std::endl;
}

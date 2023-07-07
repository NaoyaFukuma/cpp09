#include "PmergeMe.hpp"
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Please provide a sequence of positive integers as arguments." << std::endl;
        return 1;
    }

    std::vector<int> input;
    for (int i = 1; i < argc; ++i) {
        std::string argument(argv[i]);
        std::istringstream iss(argument);
        int number;
        if (!(iss >> number) || number < 0) {
            std::cerr << "Error: Invalid input. Positive integers only." << std::endl;
            return 1;
        }
        input.push_back(number);
    }

    PmergeMe pmergeMe(input);
    pmergeMe.sortAndDisplay();

    return 0;
}

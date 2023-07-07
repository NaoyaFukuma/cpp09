#include "RPN.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./RPN \"expression\"" << std::endl;
        return 1;
    }

    std::string expression = argv[1];
    RPN rpn;
    double result;

    if (rpn.evaluate(expression, result)) {
        std::cout << result << std::endl;
        return 0;
    } else {
        std::cerr << "Error: invalid expression." << std::endl;
        return 1;
    }
}

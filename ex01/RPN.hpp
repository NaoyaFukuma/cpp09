#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <stack>
#include <sstream>
#include <iostream>

class RPN {
public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    bool evaluate(const std::string& expression, double& result);

private:
    bool isOperator(const char c);
    bool performOperation(const char op, double a, double b, double& result);
};

#endif // RPN_HPP

#include "RPN.hpp"

// Default Constructor
RPN::RPN() {}

// Copy Constructor
RPN::RPN(const RPN &other) { *this = other; }

// Assignment Operator
RPN &RPN::operator=(const RPN &other) {
  (void)other;
  return *this;
}

// Destructor
RPN::~RPN() {}

bool RPN::isOperator(const char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

bool RPN::performOperation(const char op, double a, double b, double &result) {
  switch (op) {
  case '+':
    result = a + b;
    return true;
  case '-':
    result = a - b;
    return true;
  case '*':
    result = a * b;
    return true;
  case '/':
    if (b != 0) {
      result = a / b;
      return true;
    } else {
      return false;
    }
  default:
    return false;
  }
}

bool RPN::evaluate(const std::string &expression, double &result) {
  std::stack<double> stack;
  std::string::const_iterator it = expression.begin();
  std::string::const_iterator end = expression.end();

  // 数字は0 ~ 9が保証される
  // 数字及び演算子はスペースで区切られている
  while (it != end) {
    while (isspace(*it)) {
      ++it;
    }

    if (isdigit(*it)) {
      stack.push(*it - '0');
    } else if (isOperator(*it)) {
      if (stack.size() < 2) {
        return false;
      }
      double b = stack.top();
      stack.pop();
      double a = stack.top();
      stack.pop();
      double tmp;
      if (performOperation(*it, a, b, tmp)) {
        stack.push(tmp);
      } else {
        return false;
      }
    } else {
      return false;
    }
    ++it;
  }

  if (stack.size() == 1) {
    result = stack.top();
    return true;
  } else {
    return false;
  }
}

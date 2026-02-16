// ArithmeticOps.cpp
#include "ArithmeticOps.h"
#include <cmath>
#include <iostream>

using namespace std;
namespace ops {
    double add(double a, double b) {
        return a + b;
    }
    double subtract(double a, double b) {
        return a - b;
    }
    double multiply(double a, double b) {
        return a * b;
    }
    double divide(double a, double b) {
        if (b == 0.0) {
            cerr << "Error: Division by zero!" << endl;
            return 0.0;}
        return a / b;
    }

    //to num
    double toNumber(const Variable& var) {
        return var.getAsNumber();
    }

    //error
    bool isDivisionByZero(const Variable& b) {
        return b.getAsNumber() == 0.0;
    }

    //on variable
    Variable add(const Variable& a, const Variable& b) {
        double result = toNumber(a) + toNumber(b);
        return Variable::createNumber("result", result);
    }
    Variable subtract(const Variable& a, const Variable& b) {
        double result = toNumber(a) - toNumber(b);
        return Variable::createNumber("result", result);
    }
    Variable multiply(const Variable& a, const Variable& b) {
        double result = toNumber(a) * toNumber(b);
        return Variable::createNumber("result", result);
    }
    Variable divide(const Variable& a, const Variable& b) {
        if (isDivisionByZero(b)) {
            cerr << "Error: Cannot divide by zero!" << endl;
            return Variable::createNumber("error", 0.0);
        }
        double result = toNumber(a) / toNumber(b);
        return Variable::createNumber("result", result);
    }

    //calculate
    Variable calculate(const Variable& a, const Variable& b, char op) {
        switch (op) {
            case '+': return add(a, b);
            case '-': return subtract(a, b);
            case '*': return multiply(a, b);
            case '/': return divide(a, b);
            default:
                cerr << "Error: Unknown operator '" << op << "'" << endl;
                return Variable::createNumber("error", 0.0);
        }
    }

}
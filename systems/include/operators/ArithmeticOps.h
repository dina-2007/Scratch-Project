// ArithmeticOps.h
#ifndef ARITHMETIC_OPS_H
#define ARITHMETIC_OPS_H

#include "variable.h"
#include <string>
#include <iostream>

namespace ops {

    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);

    //on variables

    Variable add(const Variable& a, const Variable& b);
    Variable subtract(const Variable& a, const Variable& b);
    Variable multiply(const Variable& a, const Variable& b);
    Variable divide(const Variable& a, const Variable& b);

    //helping func
    Variable calculate(const Variable& a, const Variable& b, char op);

    double toNumber(const Variable& var);

    bool isDivisionByZero(const Variable& b);

}

#endif
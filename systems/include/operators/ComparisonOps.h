// ComparisonOps.h
#ifndef COMPARISON_OPS_H
#define COMPARISON_OPS_H

#include "../variables/variable.h"
#include <string>

namespace ops {

    bool equal(double a, double b);
    bool notEqual(double a, double b);
    bool greaterThan(double a, double b);
    bool lessThan(double a, double b);
    bool greaterOrEqual(double a, double b);
    bool lessOrEqual(double a, double b);


    bool equal(const Variable& a, const Variable& b);
    bool notEqual(const Variable& a, const Variable& b);
    bool greaterThan(const Variable& a, const Variable& b);
    bool lessThan(const Variable& a, const Variable& b);
    bool greaterOrEqual(const Variable& a, const Variable& b);
    bool lessOrEqual(const Variable& a, const Variable& b);


    bool compare(const Variable& a, const Variable& b, const std::string& op);

}

#endif
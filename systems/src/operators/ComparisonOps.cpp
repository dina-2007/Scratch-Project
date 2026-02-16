// ComparisonOps.cpp
#include "ComparisonOps.h"
#include <iostream>

using namespace std;

namespace ops {

    bool equal(double a, double b) {
        return a == b;}
    bool notEqual(double a, double b) {
        return a != b;}
    bool greaterThan(double a, double b) {
        return a > b;}
    bool lessThan(double a, double b) {
        return a < b;}
    bool greaterOrEqual(double a, double b) {
        return a >= b;}
    bool lessOrEqual(double a, double b) {
        return a <= b;}



    double toComparableNumber(const Variable& var) {
        return var.getAsNumber();}



    bool equal(const Variable& a, const Variable& b) {
        if (a.type == VarNumber && b.type == VarNumber) {
            return equal(a.getAsNumber(), b.getAsNumber());
        }
        if (a.type == VarString && b.type == VarString) {
            return a.getAsString() == b.getAsString();
        }
        if (a.type == VarBool && b.type == VarBool) {
            return a.getAsBool() == b.getAsBool();
        }
        return equal(a.getAsNumber(), b.getAsNumber());}
    bool notEqual(const Variable& a, const Variable& b) {
        return !equal(a, b);}
    bool greaterThan(const Variable& a, const Variable& b) {
        return greaterThan(a.getAsNumber(), b.getAsNumber());}
    bool lessThan(const Variable& a, const Variable& b) {
        return lessThan(a.getAsNumber(), b.getAsNumber());}

    bool greaterOrEqual(const Variable& a, const Variable& b) {
        return greaterOrEqual(a.getAsNumber(), b.getAsNumber());}
    bool lessOrEqual(const Variable& a, const Variable& b) {
        return lessOrEqual(a.getAsNumber(), b.getAsNumber());}



    bool compare(const Variable& a, const Variable& b, const string& op) {
        if (op == "=" || op == "==") return equal(a, b);
        if (op == "!=") return notEqual(a, b);
        if (op == ">") return greaterThan(a, b);
        if (op == "<") return lessThan(a, b);
        if (op == ">=") return greaterOrEqual(a, b);
        if (op == "<=") return lessOrEqual(a, b);

        cerr << "Error: Unknown comparison operator '" << op << "'" << endl;
        return false;
    }

}
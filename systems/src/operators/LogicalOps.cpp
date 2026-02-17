// LogicalOps.cpp
#include "LogicalOps.h"
#include <iostream>

using namespace std;

namespace ops {


    bool toBoolean(const Variable& var){
        return var.getAsBool();}

    string boolToString(bool value){
        return value ? "true" : "false";}



    bool logicNot(bool a) {
        return !a;}
    bool logicAnd(bool a, bool b){
        return a && b;}
    bool logicOr(bool a, bool b) {
        return a || b;}
    bool logicXor(bool a, bool b) {
        return a != b;}



    Variable not_(const Variable& a) {
        bool result = logicNot(toBoolean(a));
        return Variable::createBool("not_result", result);}
    Variable and_(const Variable& a, const Variable& b) {
        bool result = logicAnd(toBoolean(a), toBoolean(b));
        return Variable::createBool("and_result", result);}
    Variable or_(const Variable& a, const Variable& b) {
        bool result = logicOr(toBoolean(a), toBoolean(b));
        return Variable::createBool("or_result", result);}
    Variable xor_(const Variable& a, const Variable& b) {
        bool result = logicXor(toBoolean(a), toBoolean(b));
        return Variable::createBool("xor_result", result);}




    Variable logicalOperation(const Variable& a, const string& op){
        if (op == "not" || op == "!"){
            return not_(a);}
        cerr << "Error: Unknown unary logical operator '" << op << "'" << endl;
        return Variable::createBool("error", false);}

    Variable logicalOperation(const Variable& a, const Variable& b, const string& op){
        if (op == "and" || op == "&&"){
            return and_(a, b);}
        if (op == "or" || op == "||"){
            return or_(a, b);}
        if (op == "xor"){
            return xor_(a, b);}
        cerr << "Error: Unknown binary logical operator '" << op << "'" << endl;
        return Variable::createBool("error", false);
    }

}
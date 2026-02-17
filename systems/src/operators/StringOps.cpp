// StringOps.cpp
#include "StringOps.h"
#include <sstream>
#include <iostream>

using namespace std;

namespace ops {


    string toString(const Variable& var) {
        return var.getAsString();}

    size_t stringLength(const string& str) {
        return str.length();}

    string charAtIndex(const string& str, size_t index) {
//start : 1
        if (index < 1 || index > str.length()) {
            return "";  // er
		}
        return string(1, str[index - 1]);}

    string concat(const string& a, const string& b){
        return a + b;}

    bool isValidIndex(const string& str, size_t index) {
        return index >= 1 && index <= str.length();}



    Variable stringLength(const Variable& var) {
        string str = toString(var);
        double len = stringLength(str);
        return Variable::createNumber("length", len);}

    Variable charAtIndex(const Variable& var, const Variable& index) {
        string str = toString(var);
        double idx = index.getAsNumber();

        if (!isValidIndex(str, static_cast<size_t>(idx))) {
            cerr << "Warning: Index " << idx << " out of range for string of length "
                 << str.length() << endl;
            return Variable::createString("char", "");}

        string result = charAtIndex(str, static_cast<size_t>(idx));
        return Variable::createString("char", result);}

    Variable concat(const Variable& a, const Variable& b) {
        string strA = toString(a);
        string strB = toString(b);
        string result = concat(strA, strB);
        return Variable::createString("concat", result);
    }



    Variable stringOperation(const Variable& a, const string& op){
        if (op == "length") {
            return stringLength(a);}
        cerr << "Error: Unknown unary string operator '" << op << "'" << endl;
        return Variable::createString("error", "");}

    Variable stringOperation(const Variable& a, const Variable& b, const string& op) {
        if (op == "join" || op == "concat") {
            return concat(a, b);}

        if (op == "charAt" || op == "letter") {
            return charAtIndex(a, b);}

        cerr << "Error: Unknown binary string operator '" << op << "'" << endl;
        return Variable::createString("error", "");
    }

}
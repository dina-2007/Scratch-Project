//variable.cpp
//1404/11/09
#include "variable.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

// creation funcs
Variable Variable::createNumber(const string& name, double value) {
    Variable var;
    var.name = name;
    var.type = VarNumber;
    var.value = value;
    return var;
}

Variable Variable::createString(const string& name, const string& value) {
    Variable var;
    var.name = name;
    var.type = VarString;
    var.value = value;
    return var;
}

Variable Variable::createBool(const string& name, bool value) {
    Variable var;
    var.name = name;
    var.type = VarBool;
    var.value = value;
    return var;
}

// getter funcs
double Variable::getAsNumber() const{
    if (type != VarNumber) {
        cerr << "Warning: Variable '" << name << "' is not a number!" << endl;
        return 0.0;}
    return get<double>(value);
}

string Variable::getAsString() const{
    if (type != VarString) {
        if (type == VarNumber) {
            return to_string(get<double>(value));}
        if (type == VarBool) {
            return get<bool>(value) ? "true" : "false";}
        return "";}
    return get<string>(value);
}

bool Variable::getAsBool() const {
    if (type != VarBool) {
        if (type == VarNumber) {
            return get<double>(value) != 0.0;}
        if (type == VarString) {
            return !get<string>(value).empty();}
        return false;}
    return get<bool>(value);
}

//display funcs

string Variable::toString() const{
    stringstream ss;
    ss << name << " [";
    switch (type) {
        case VarNumber:
            ss << "Number] = " << get<double>(value);
            break;
        case VarString:
            ss << "String] = \"" << get<string>(value) << "\"";
            break;
        case VarBool:
            ss << "Bool] = " << (get<bool>(value) ? "true" : "false");
            break;
        case VarUndefined:
            ss << "Undefined] = ???";
            break;
        default:
            ss << "Unknown]";
    }

    return ss.str();
}



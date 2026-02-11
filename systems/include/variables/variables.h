//variable.h
//1404/11/09
#ifndef VARIABLES_H
#define VARIABLES_H

#include <string>
#include <variant>

using namespace std;

enum VariableType{
    VarNumber,
    VarString,
    VarBool,
    VarUndefined};
struct Variable{
    string name;
    VariableType type;
    variant<double , string , bool> value;
    //simple funcs
    static Variable createNumber(const string& name, double value);
    static Variable createString(const string& name , const string& value);
    static Variable createBool(const string& name, bool value);
    double getAsNumber() const;
    string getAsString() const;
    bool getAsBool() const;
    string toString() const; //for debug
};
#endif
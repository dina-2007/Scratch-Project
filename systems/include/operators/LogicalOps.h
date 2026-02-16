// LogicalOps.h
#ifndef LOGICAL_OPS_H
#define LOGICAL_OPS_H

#include "variable.h"
#include <string>

namespace ops {


    bool logicNot(bool a);
    bool logicAnd(bool a, bool b);
    bool logicOr(bool a, bool b);
    bool logicXor(bool a, bool b);


    Variable not_(const Variable& a);
    Variable and_(const Variable& a, const Variable& b);
    Variable or_(const Variable& a, const Variable& b);
    Variable xor_(const Variable& a, const Variable& b);


    bool toBoolean(const Variable& var);
    std::string boolToString(bool value);


    Variable logicalOperation(const Variable& a, const Variable& b, const std::string& op);
    Variable logicalOperation(const Variable& a, const std::string& op); // not

}

#endif
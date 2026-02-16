// StringOps.h
#ifndef STRING_OPS_H
#define STRING_OPS_H

#include "variable.h"
#include <string>

namespace ops {


    size_t stringLength(const std::string& str);
    std::string charAtIndex(const std::string& str, size_t index);
    std::string concat(const std::string& a, const std::string& b);



    Variable stringLength(const Variable& var);
    Variable charAtIndex(const Variable& var, const Variable& index);
    Variable concat(const Variable& a, const Variable& b);


    std::string toString(const Variable& var);
    bool isValidIndex(const std::string& str, size_t index);

    Variable stringOperation(const Variable& a, const Variable& b, const std::string& op);
    Variable stringOperation(const Variable& a, const std::string& op); // برای عملیات تک‌متغیره

}

#endif
// VariableManager.h
#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "variable.h"

struct VariableStore {
    std::unordered_map<std::string, Variable> variables;
};

//manage funcs

//new store
VariableStore createVariableStore();

void storeSet(VariableStore& store, const Variable& var);

Variable* storeGet(VariableStore& store, const std::string& name);
const Variable* storeGetConst(const VariableStore& store, const std::string& name);

bool storeExists(const VariableStore& store, const std::string& name);

bool storeRemove(VariableStore& store, const std::string& name);

void storeClear(VariableStore& store);

//info funcs

std::vector<std::string> storeGetAllNames(const VariableStore& store);

size_t storeCount(const VariableStore& store);

void storePrintAll(const VariableStore& store);

//easy

//create
void storeSetNumber(VariableStore& store, const std::string& name, double value);
void storeSetString(VariableStore& store, const std::string& name, const std::string& value);
void storeSetBool(VariableStore& store, const std::string& name, bool value);

//get
double storeGetNumber(const VariableStore& store, const std::string& name,
                     double defaultValue = 0.0);
std::string storeGetString(const VariableStore& store, const std::string& name,
                          const std::string& defaultValue = "");
bool storeGetBool(const VariableStore& store, const std::string& name,
                 bool defaultValue = false);

#endif
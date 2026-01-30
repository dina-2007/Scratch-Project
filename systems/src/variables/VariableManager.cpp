// VariableManager.cpp
#include "VariableManager.h"
#include <iostream>

using namespace std;

// main funcs

VariableStore createVariableStore() {
    VariableStore store;
    store.variables = {};
    return store;
}

void storeSet(VariableStore& store, const Variable& var) {
    store.variables[var.name] = var;
}

Variable* storeGet(VariableStore& store, const string& name) {
    auto it = store.variables.find(name);
    return (it != store.variables.end()) ? &(it->second) : nullptr;
}

const Variable* storeGetConst(const VariableStore& store, const string& name) {
    auto it = store.variables.find(name);
    return (it != store.variables.end()) ? &(it->second) : nullptr;
}

bool storeExists(const VariableStore& store, const string& name) {
    return store.variables.find(name) != store.variables.end();
}

bool storeRemove(VariableStore& store, const string& name) {
    return store.variables.erase(name) > 0;
}

void storeClear(VariableStore& store) {
    store.variables.clear();
}

//info funcs

vector<string> storeGetAllNames(const VariableStore& store) {
    vector<string> names;
    for (const auto& pair : store.variables) {
        names.push_back(pair.first);
    }
    return names;
}

size_t storeCount(const VariableStore& store) {
    return store.variables.size();
}

void storePrintAll(const VariableStore& store) {
    if (store.variables.empty()) {
        cout << "No variables in store." << endl;
        return;
    }

    cout << "=== Variables (" << storeCount(store) << ") ===" << endl;
    for (const auto& pair : store.variables) {
        cout << "  " << pair.second.toString() << endl;
    }
}

// easy funcs

void storeSetNumber(VariableStore& store, const string& name, double value) {
    storeSet(store, Variable::createNumber(name, value));
}

void storeSetString(VariableStore& store, const string& name, const string& value) {
    storeSet(store, Variable::createString(name, value));
}

void storeSetBool(VariableStore& store, const string& name, bool value) {
    storeSet(store, Variable::createBool(name, value));
}

double storeGetNumber(const VariableStore& store, const string& name,
                     double defaultValue) {
    const Variable* var = storeGetConst(store, name);
    return var ? var->getAsNumber() : defaultValue;
}

string storeGetString(const VariableStore& store, const string& name,
                     const string& defaultValue) {
    const Variable* var = storeGetConst(store, name);
    return var ? var->getAsString() : defaultValue;
}

bool storeGetBool(const VariableStore& store, const string& name,
                 bool defaultValue) {
    const Variable* var = storeGetConst(store, name);
    return var ? var->getAsBool() : defaultValue;
}
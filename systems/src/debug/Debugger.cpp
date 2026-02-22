// Debugger.cpp
#include "Debugger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;
using namespace debug;

static string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%H:%M:%S");
    return ss.str();
}

static bool evaluateSimpleCondition(const string& cond, const VariableManager& varMan) {
    if (cond.empty()) return true;

    size_t spacePos = cond.find(' ');
    if (spacePos == string::npos) return false;

    string varName = cond.substr(0, spacePos);
    string op = cond.substr(spacePos + 1, cond.find(' ', spacePos + 1) - spacePos - 1);
    string valueStr = cond.substr(cond.rfind(' ') + 1);

    const Variable* var = storeGetConst(varMan, varName);
    if (!var) return false;

    double varValue = var->getAsNumber();
    double compareValue = stod(valueStr);

    if (op == ">") return varValue > compareValue;
    if (op == "<") return varValue < compareValue;
    if (op == "==") return varValue == compareValue;
    if (op == ">=") return varValue >= compareValue;
    if (op == "<=") return varValue <= compareValue;

    return false;
}

Debugger debug::createDebugger() {
    Debugger db;
    db.enabled = false;
    db.mode = DEBUG_OFF;
    db.level = LEVEL_NORMAL;
    db.maxLogSize = 500;
    db.paused = false;
    db.currentLine = -1;
    db.stepCount = 0;
    db.totalSteps = 0;
    db.totalBreakpointsHit = 0;
    db.totalErrors = 0;
    db.startTime = chrono::steady_clock::now();
    db.outputFunction = nullptr;
    return db;
}

Debugger debug::createDebuggerWithLevel(DebugLevel level) {
    Debugger db = createDebugger();
    db.level = level;
    return db;
}

void debug::startDebug(Debugger& db) {
    db.enabled = true;
    db.paused = false;
    db.currentLine = -1;
    db.startTime = chrono::steady_clock::now();
    log(db, "Debugger", "Debugging started", LEVEL_NORMAL);
}

void debug::stopDebug(Debugger& db) {
    db.enabled = false;
    db.paused = false;
    log(db, "Debugger", "Debugging stopped", LEVEL_NORMAL);
}

void debug::pauseDebug(Debugger& db) {
    if (!db.enabled) return;
    db.paused = true;
    log(db, "Debugger", "Paused at line " + to_string(db.currentLine), LEVEL_NORMAL);
    printCurrentState(db);
}

void debug::resumeDebug(Debugger& db) {
    if (!db.enabled) return;
    db.paused = false;
    log(db, "Debugger", "Resumed", LEVEL_NORMAL);
}

void debug::stepOver(Debugger& db) {
    if (!db.enabled || !db.paused) return;
    db.stepCount = 1;
    db.paused = false;
    log(db, "Debugger", "Step over", LEVEL_VERBOSE);
}

void debug::stepInto(Debugger& db) {
    if (!db.enabled || !db.paused) return;
    db.stepCount = 1;
    db.paused = false;
    log(db, "Debugger", "Step into", LEVEL_VERBOSE);
}

void debug::stepOut(Debugger& db) {
    if (!db.enabled || !db.paused) return;
    db.stepCount = 9999;
    db.paused = false;
    log(db, "Debugger", "Step out", LEVEL_VERBOSE);
}

void debug::continue_(Debugger& db) {
    if (!db.enabled || !db.paused) return;
    db.stepCount = 0;
    db.paused = false;
    log(db, "Debugger", "Continue", LEVEL_NORMAL);
}

void debug::addBreakpoint(Debugger& db, int line) {
    Breakpoint bp(line);
    db.breakpoints.push_back(bp);
    log(db, "Debugger", "Breakpoint added at line " + to_string(line), LEVEL_NORMAL);
}

void debug::addBreakpoint(Debugger& db, int line, const string& condition) {
    Breakpoint bp(line);
    bp.condition = condition;
    db.breakpoints.push_back(bp);
    log(db, "Debugger", "Breakpoint added at line " + to_string(line) +
        " [" + condition + "]", LEVEL_NORMAL);
}

void debug::removeBreakpoint(Debugger& db, int line) {
    auto it = remove_if(db.breakpoints.begin(), db.breakpoints.end(),
        [line](const Breakpoint& bp) { return bp.lineNumber == line; });

    if (it != db.breakpoints.end()) {
        db.breakpoints.erase(it, db.breakpoints.end());
        log(db, "Debugger", "Breakpoint removed at line " + to_string(line), LEVEL_NORMAL);
    }
}

void debug::clearAllBreakpoints(Debugger& db) {
    db.breakpoints.clear();
    log(db, "Debugger", "All breakpoints cleared", LEVEL_NORMAL);
}

bool debug::shouldBreak(const Debugger& db, int line, const string& file) {
    for (auto& bp : db.breakpoints) {
        if (bp.lineNumber == line && bp.enabled) {
            bp.hitCount++;
            const_cast<Debugger&>(db).totalBreakpointsHit++;
            return true;
        }
    }
    return false;
}


void debug::watchVariable(Debugger& db, const string& varName) {
    if (find(db.watchVariables.begin(), db.watchVariables.end(), varName)
        == db.watchVariables.end()) {
        db.watchVariables.push_back(varName);
        log(db, "Debugger", "Watching variable: " + varName, LEVEL_NORMAL);
    }
}

void debug::unwatchVariable(Debugger& db, const string& varName) {
    auto it = find(db.watchVariables.begin(), db.watchVariables.end(), varName);
    if (it != db.watchVariables.end()) {
        db.watchVariables.erase(it);
        log(db, "Debugger", "Stopped watching: " + varName, LEVEL_NORMAL);
    }
}

void debug::printWatchedVariables(const Debugger& db, const VariableManager& varMan) {
    if (db.watchVariables.empty()) {
        cout << "  No watched variables" << endl;
        return;
    }

    cout << "  Watched variables:" << endl;
    for (const auto& varName : db.watchVariables) {
        const Variable* var = storeGetConst(varMan, varName);
        if (var) {
            cout << "    " << var->toString() << endl;
        } else {
            cout << "    " << varName << " = undefined" << endl;
        }
    }
}

void debug::log(Debugger& db, const string& component,
               const string& message, DebugLevel level) {
    if (!db.enabled || !shouldLog(db, level)) return;

    LogEntry entry;
    entry.timestamp = getCurrentTimestamp();
    entry.component = component;
    entry.message = message;
    entry.level = level;

    db.logs.push_back(entry);

    if (db.logs.size() > db.maxLogSize) {
        db.logs.erase(db.logs.begin());
    }

    if (db.outputFunction) {
        stringstream ss;
        ss << "[" << entry.timestamp << "][" << component << "] " << message;
        db.outputFunction(ss.str());
    }
}

void debug::logWithContext(Debugger& db, const string& component,
                          const string& message,
                          const map<string, string>& context,
                          DebugLevel level) {
    if (!db.enabled || !shouldLog(db, level)) return;

    LogEntry entry;
    entry.timestamp = getCurrentTimestamp();
    entry.component = component;
    entry.message = message;
    entry.level = level;

    db.logs.push_back(entry);

    if (db.logs.size() > db.maxLogSize) {
        db.logs.erase(db.logs.begin());
    }

    if (db.outputFunction) {
        stringstream ss;
        ss << "[" << entry.timestamp << "][" << component << "] " << message;
        if (!context.empty()) {
            ss << " {";
            for (const auto& pair : context) {
                ss << " " << pair.first << "=" << pair.second;
            }
            ss << " }";
        }
        db.outputFunction(ss.str());
    }
}

void debug::printLogs(const Debugger& db, int count) {
    if (db.logs.empty()) {
        cout << "  No logs" << endl;
        return;
    }

    int start = 0;
    if (count > 0 && count < static_cast<int>(db.logs.size())) {
        start = db.logs.size() - count;
    }

    cout << "  Recent logs:" << endl;
    for (size_t i = start; i < db.logs.size(); i++) {
        const auto& log = db.logs[i];
        cout << "    [" << log.timestamp << "][" << log.component << "] "
             << log.message << endl;
    }
}

void debug::clearLogs(Debugger& db) {
    db.logs.clear();
    log(db, "Debugger", "Logs cleared", LEVEL_NORMAL);
}


void debug::printStatus(const Debugger& db) {
    cout << "\n=== Debugger Status ===" << endl;
    cout << "  Enabled: " << (db.enabled ? "YES" : "NO") << endl;
    cout << "  Mode: " << modeToString(db.mode) << endl;
    cout << "  Level: " << levelToString(db.level) << endl;
    cout << "  Paused: " << (db.paused ? "YES" : "NO") << endl;
    cout << "  Current Line: " << db.currentLine << endl;
    if (!db.currentFile.empty()) {
        cout << "  Current File: " << db.currentFile << endl;
    }
    cout << "  Breakpoints: " << db.breakpoints.size() << endl;
    cout << "  Watch Variables: " << db.watchVariables.size() << endl;
    cout << "  Logs: " << db.logs.size() << "/" << db.maxLogSize << endl;
    cout << "  Elapsed: " << getElapsedSeconds(db) << "s" << endl;
}

void debug::printBreakpoints(const Debugger& db) {
    if (db.breakpoints.empty()) {
        cout << "  No breakpoints" << endl;
        return;
    }

    cout << "  Breakpoints:" << endl;
    for (const auto& bp : db.breakpoints) {
        cout << "    Line " << bp.lineNumber;
        if (!bp.fileName.empty()) {
            cout << " [" << bp.fileName << "]";
        }
        if (!bp.condition.empty()) {
            cout << " condition: \"" << bp.condition << "\"";
        }
        cout << (bp.enabled ? " [enabled]" : " [disabled]");
        cout << " (hit " << bp.hitCount << ")" << endl;
    }
}

void debug::printStats(const Debugger& db) {
    cout << "\n=== Debugger Statistics ===" << endl;
    cout << "  Total steps: " << db.totalSteps << endl;
    cout << "  Breakpoints hit: " << db.totalBreakpointsHit << endl;
    cout << "  Errors: " << db.totalErrors << endl;
}

void debug::printCurrentState(const Debugger& db) {
    cout << "\n=== Current State ===" << endl;
    cout << "  Line: " << db.currentLine << endl;
    if (!db.currentFile.empty()) {
        cout << "  File: " << db.currentFile << endl;
    }
}

bool debug::shouldLog(const Debugger& db, DebugLevel msgLevel) {
    return static_cast<int>(msgLevel) <= static_cast<int>(db.level);
}

string debug::getCurrentTimestamp() {
    return ::getCurrentTimestamp();
}

string debug::modeToString(DebugMode mode) {
    switch(mode) {
        case DEBUG_OFF: return "OFF";
        case DEBUG_STEP: return "STEP";
        case DEBUG_CONTINUOUS: return "CONTINUOUS";
        case DEBUG_BREAKPOINT: return "BREAKPOINT";
        default: return "UNKNOWN";
    }
}

string debug::levelToString(DebugLevel level) {
    switch(level) {
        case LEVEL_MINIMAL: return "MINIMAL";
        case LEVEL_NORMAL: return "NORMAL";
        case LEVEL_VERBOSE: return "VERBOSE";
        case LEVEL_DETAILED: return "DETAILED";
        default: return "UNKNOWN";
    }
}

double debug::getElapsedSeconds(const Debugger& db) {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - db.startTime);
    return elapsed.count() / 1000.0;
}

void debug::incrementStep(Debugger& db) {
    db.totalSteps++;
    if (db.level >= LEVEL_VERBOSE) {
        log(db, "Debugger", "Step executed", LEVEL_VERBOSE);
    }
}

void debug::incrementBreakpointHit(Debugger& db) {
    db.totalBreakpointsHit++;
}

void debug::incrementError(Debugger& db) {
    db.totalErrors++;
    log(db, "Debugger", "Error recorded", LEVEL_NORMAL);
}

void debug::resetStats(Debugger& db) {
    db.totalSteps = 0;
    db.totalBreakpointsHit = 0;
    db.totalErrors = 0;
    log(db, "Debugger", "Stats reset", LEVEL_NORMAL);
}

bool debug::evaluateCondition(const Debugger& db, const string& condition,
                             const VariableManager& varMan) {
    return evaluateSimpleCondition(condition, varMan);
}
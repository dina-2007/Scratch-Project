// Debugger.h
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <chrono>
#include "../variables/VariableManager.h"

namespace debug {

    enum DebugMode {
        DEBUG_OFF,
        DEBUG_STEP,
        DEBUG_CONTINUOUS,
        DEBUG_BREAKPOINT
    };

    enum DebugLevel {
        LEVEL_MINIMAL,
        LEVEL_NORMAL,
        LEVEL_VERBOSE,
        LEVEL_DETAILED
    };

    struct Breakpoint {
        int lineNumber;
        std::string fileName;
        std::string condition;
        bool enabled;
        int hitCount;

        Breakpoint() : lineNumber(-1), enabled(true), hitCount(0) {}
        Breakpoint(int line) : lineNumber(line), enabled(true), hitCount(0) {}
    };

    struct LogEntry {
        std::string timestamp;
        std::string component;
        std::string message;
        DebugLevel level;

        LogEntry() : level(LEVEL_NORMAL) {}
    };


    struct Debugger {
        bool enabled;
        DebugMode mode;
        DebugLevel level;

        std::vector<Breakpoint> breakpoints;

        std::vector<LogEntry> logs;
        size_t maxLogSize;

        std::vector<std::string> watchVariables;

        bool paused;
        int currentLine;
        std::string currentFile;
        int stepCount;

        int totalSteps;
        int totalBreakpointsHit;
        int totalErrors;

        std::chrono::steady_clock::time_point startTime;

        std::function<void(const std::string&)> outputFunction;
    };

    Debugger createDebugger();
    Debugger createDebuggerWithLevel(DebugLevel level);

    void startDebug(Debugger& db);
    void stopDebug(Debugger& db);
    void pauseDebug(Debugger& db);
    void resumeDebug(Debugger& db);
    void stepOver(Debugger& db);
    void stepInto(Debugger& db);
    void stepOut(Debugger& db);
    void continue_(Debugger& db);

    void addBreakpoint(Debugger& db, int line);
    void addBreakpoint(Debugger& db, int line, const std::string& condition);
    void removeBreakpoint(Debugger& db, int line);
    void clearAllBreakpoints(Debugger& db);
    bool shouldBreak(const Debugger& db, int line, const std::string& file);


    void watchVariable(Debugger& db, const std::string& varName);
    void unwatchVariable(Debugger& db, const std::string& varName);
    void printWatchedVariables(const Debugger& db, const VariableManager& varMan);


    void log(Debugger& db, const std::string& component,
             const std::string& message, DebugLevel level = LEVEL_NORMAL);
    void logWithContext(Debugger& db, const std::string& component,
                        const std::string& message,
                        const std::map<std::string, std::string>& context,
                        DebugLevel level = LEVEL_NORMAL);
    void printLogs(const Debugger& db, int count = -1);
    void clearLogs(Debugger& db);

    void printStatus(const Debugger& db);
    void printBreakpoints(const Debugger& db);
    void printStats(const Debugger& db);
    void printCurrentState(const Debugger& db);

    bool shouldLog(const Debugger& db, DebugLevel msgLevel);
    std::string getCurrentTimestamp();
    std::string modeToString(DebugMode mode);
    std::string levelToString(DebugLevel level);
    double getElapsedSeconds(const Debugger& db);


    void incrementStep(Debugger& db);
    void incrementBreakpointHit(Debugger& db);
    void incrementError(Debugger& db);
    void resetStats(Debugger& db);

    bool evaluateCondition(const Debugger& db, const std::string& condition,
                          const VariableManager& varMan);
}

#endif
// EventManager.h
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../variables/variable.h"

namespace events {


    enum EventType {
        EVENT_GREEN_FLAG,
        EVENT_KEY_PRESSED,
        EVENT_SPRITE_CLICKED,
        EVENT_BROADCAST,
        EVENT_TIMER,
        EVENT_BACKDROP_SWITCH
    };


    struct Event {
        EventType type;
        std::string name;
        std::string value;

        Event() : type(EVENT_GREEN_FLAG), name(""), value("") {}
        Event(EventType t) : type(t), name(""), value("") {}
        Event(EventType t, const std::string& n) : type(t), name(n), value("") {}
    };


    struct Script {
        std::string id;
        Event trigger;
        std::vector<void*> blocks;  //type of block

        Script() : id("") {}
        Script(const Event& e) : trigger(e) {
            id = std::to_string(reinterpret_cast<uintptr_t>(this));
        }
    };



    struct EventManager {
        std::vector<Script> scripts;
        std::unordered_multimap<std::string, Script*> eventToScripts;
        std::vector<Event> activeEvents;
        std::function<void(const Script&)> executor;

        int totalEventsTriggered;
        int totalScriptsExecuted;
    };



    EventManager createEventManager();


    void setEventExecutor(EventManager& manager, std::function<void(const Script&)> exec);



    void addScript(EventManager& manager, const Script& script);
    bool removeScript(EventManager& manager, const std::string& scriptId);
    std::vector<Script*> findScriptsByEvent(const EventManager& manager, const Event& event);



    void triggerEvent(EventManager& manager, const Event& event);
    void triggerBroadcast(EventManager& manager, const std::string& message);
    void triggerKeyPress(EventManager& manager, const std::string& key);
    void triggerSpriteClick(EventManager& manager, const std::string& spriteName);
    void triggerGreenFlag(EventManager& manager);



    bool hasPendingEvents(const EventManager& manager);
    void processEvents(EventManager& manager);
    void clearEvents(EventManager& manager);


    int getScriptCount(const EventManager& manager);
    int getEventCount(const EventManager& manager);
    void printEventStats(const EventManager& manager);



    std::string serializeEventManager(const EventManager& manager);
    bool deserializeEventManager(EventManager& manager, const std::string& data);
}

#endif
// Events.h
#ifndef EVENTS_H
#define EVENTS_H

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
        std::vector<void*> blocks;

        Script() : id("") {}
        Script(const Event& e) : trigger(e) {
            id = std::to_string(reinterpret_cast<uintptr_t>(this));
        }
    };


    struct EventStore {
        std::vector<Script> scripts;
        std::unordered_multimap<std::string, Script*> eventToScripts;
        std::vector<Event> activeEvents;
        std::function<void(const Script&)> executor;
    };



    EventStore createEventStore();
    void setExecutor(EventStore& store, std::function<void(const Script&)> exec);
    void addScript(EventStore& store, const Script& script);
    bool removeScript(EventStore& store, const std::string& scriptId);
    std::vector<Script*> getScriptsForEvent(const EventStore& store, const Event& event);



    void triggerEvent(EventStore& store, const Event& event);
    void triggerBroadcast(EventStore& store, const std::string& message);
    void triggerKeyPress(EventStore& store, const std::string& key);
    void triggerSpriteClick(EventStore& store, const std::string& spriteName);



    bool hasActiveEvents(const EventStore& store);
    void processActiveEvents(EventStore& store);
    void clearActiveEvents(EventStore& store);

}

#endif
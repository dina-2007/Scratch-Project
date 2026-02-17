// Events.cpp
#include "Events.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace events;

EventStore events::createEventStore() {
    EventStore store;
    store.executor = nullptr;
    return store;
}

void events::setExecutor(EventStore& store, function<void(const Script&)> exec) {
    store.executor = exec;
}

void events::addScript(EventStore& store, const Script& script) {
    store.scripts.push_back(script);
    Script* scriptPtr = &store.scripts.back();

    string key = to_string(script.trigger.type) + ":" + script.trigger.name;
    store.eventToScripts.insert({key, scriptPtr});

    cout << "EventStore: Script added for event " << key << endl;
}

bool events::removeScript(EventStore& store, const string& scriptId) {
    auto it = find_if(store.scripts.begin(), store.scripts.end(),
        [&](const Script& s) { return s.id == scriptId; });

    if (it != store.scripts.end()) {
        store.scripts.erase(it);
        // note: eventToScripts رو ساده‌سازی کردیم
        cout << "EventStore: Script " << scriptId << " removed" << endl;
        return true;
    }
    return false;
}

vector<Script*> events::getScriptsForEvent(const EventStore& store, const Event& event) {
    vector<Script*> result;
    string key = to_string(event.type) + ":" + event.name;

    auto range = store.eventToScripts.equal_range(key);
    for (auto it = range.first; it != range.second; ++it) {
        result.push_back(it->second);
    }

    return result;
}

void events::triggerEvent(EventStore& store, const Event& event) {
    cout << "Event triggered: type=" << event.type
         << ", name=" << event.name << endl;
    store.activeEvents.push_back(event);
}

void events::triggerBroadcast(EventStore& store, const string& message) {
    triggerEvent(store, Event(EVENT_BROADCAST, message));
}

void events::triggerKeyPress(EventStore& store, const string& key) {
    Event e(EVENT_KEY_PRESSED, "key");
    e.value = key;
    store.activeEvents.push_back(e);
}

void events::triggerSpriteClick(EventStore& store, const string& spriteName) {
    triggerEvent(store, Event(EVENT_SPRITE_CLICKED, spriteName));
}

bool events::hasActiveEvents(const EventStore& store) {
    return !store.activeEvents.empty();
}

void events::processActiveEvents(EventStore& store) {
    if (!store.executor) {
        cerr << "EventStore Error: No executor set!" << endl;
        return;
    }

    for (const auto& event : store.activeEvents) {
        auto scripts = getScriptsForEvent(store, event);

        cout << "Processing event: found " << scripts.size()
             << " scripts" << endl;

        for (Script* script : scripts) {
            if (script) {
                store.executor(*script);
            }
        }
    }

    store.activeEvents.clear();
}

void events::clearActiveEvents(EventStore& store) {
    store.activeEvents.clear();
}
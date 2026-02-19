// EventManager.cpp
#include "EventManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace events;


EventManager events::createEventManager() {
    EventManager manager;
    manager.totalEventsTriggered = 0;
    manager.totalScriptsExecuted = 0;
    manager.executor = nullptr;
    return manager;
}


void events::setEventExecutor(EventManager& manager, function<void(const Script&)> exec) {
    manager.executor = exec;
}


void events::addScript(EventManager& manager, const Script& script) {
    manager.scripts.push_back(script);
    Script* scriptPtr = &manager.scripts.back();

    string key = to_string(script.trigger.type) + ":" + script.trigger.name;
    manager.eventToScripts.insert({key, scriptPtr});

    cout << "[EventManager] Script added for event: " << key << endl;
}

bool events::removeScript(EventManager& manager, const string& scriptId) {
    auto it = find_if(manager.scripts.begin(), manager.scripts.end(),
        [&](const Script& s) { return s.id == scriptId; });

    if (it != manager.scripts.end()) {
        manager.scripts.erase(it);

        cout << "[EventManager] Script removed: " << scriptId << endl;
        return true;
    }
    return false;
}

vector<Script*> events::findScriptsByEvent(const EventManager& manager, const Event& event) {
    vector<Script*> result;
    string key = to_string(event.type) + ":" + event.name;

    auto range = manager.eventToScripts.equal_range(key);
    for (auto it = range.first; it != range.second; ++it) {
        result.push_back(it->second);
    }

    return result;
}


void events::triggerEvent(EventManager& manager, const Event& event) {
    manager.activeEvents.push_back(event);
    manager.totalEventsTriggered++;

    cout << "[EventManager] Event triggered: type=" << event.type
         << ", name=" << event.name << endl;
}

void events::triggerBroadcast(EventManager& manager, const string& message) {
    triggerEvent(manager, Event(EVENT_BROADCAST, message));
}

void events::triggerKeyPress(EventManager& manager, const string& key) {
    Event e(EVENT_KEY_PRESSED, "key");
    e.value = key;
    triggerEvent(manager, e);
}

void events::triggerSpriteClick(EventManager& manager, const string& spriteName) {
    triggerEvent(manager, Event(EVENT_SPRITE_CLICKED, spriteName));
}

void events::triggerGreenFlag(EventManager& manager) {
    triggerEvent(manager, Event(EVENT_GREEN_FLAG));
}


bool events::hasPendingEvents(const EventManager& manager) {
    return !manager.activeEvents.empty();
}

void events::processEvents(EventManager& manager) {
    if (!manager.executor) {
        cerr << "[EventManager] Error: No executor set!" << endl;
        return;
    }

    vector<Event> eventsToProcess = manager.activeEvents;
    manager.activeEvents.clear();

    for (const auto& event : eventsToProcess) {
        auto scripts = findScriptsByEvent(manager, event);

        cout << "[EventManager] Processing event: found " << scripts.size()
             << " scripts" << endl;

        for (Script* script : scripts) {
            if (script) {
                manager.executor(*script);
                manager.totalScriptsExecuted++;
            }
        }
    }
}

void events::clearEvents(EventManager& manager) {
    manager.activeEvents.clear();
    cout << "[EventManager] All events cleared" << endl;
}


int events::getScriptCount(const EventManager& manager) {
    return manager.scripts.size();
}

int events::getEventCount(const EventManager& manager) {
    return manager.activeEvents.size();
}

void events::printEventStats(const EventManager& manager) {
    cout << "=== Event Manager Stats ===" << endl;
    cout << "Total scripts: " << manager.scripts.size() << endl;
    cout << "Pending events: " << manager.activeEvents.size() << endl;
    cout << "Events triggered: " << manager.totalEventsTriggered << endl;
    cout << "Scripts executed: " << manager.totalScriptsExecuted << endl;
}



string events::serializeEventManager(const EventManager& manager) {
    stringstream ss;
    ss << "EventManager:" << manager.scripts.size() << ":"
       << manager.totalEventsTriggered << ":"
       << manager.totalScriptsExecuted << "\n";
    return ss.str();
}

bool events::deserializeEventManager(EventManager& manager, const string& data) {
    cout << "[EventManager] Deserialize called" << endl;
    return true;
}
// SensingManager.cpp
#include "SensingManager.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace sensing;


static bool pointInRect(const Position& p, const Position& rectPos, double w, double h) {
    return (p.x >= rectPos.x - w/2 && p.x <= rectPos.x + w/2 &&
            p.y >= rectPos.y - h/2 && p.y <= rectPos.y + h/2);
}

static bool rectsOverlap(const Position& p1, double w1, double h1,
                         const Position& p2, double w2, double h2) {
    double left1 = p1.x - w1/2;
    double right1 = p1.x + w1/2;
    double top1 = p1.y - h1/2;
    double bottom1 = p1.y + h1/2;

    double left2 = p2.x - w2/2;
    double right2 = p2.x + w2/2;
    double top2 = p2.y - h2/2;
    double bottom2 = p2.y + h2/2;

    return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
}

static double distance(const Position& a, const Position& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}


SensingManager sensing::createSensingManager() {
    SensingManager sm;
    sm.mousePos = Position(0, 0);
    sm.mouseDown = false;
    sm.startTime = chrono::steady_clock::now();
    sm.waitingForAnswer = false;
    sm.getPixelColor = nullptr;
    sm.totalCollisions = 0;
    sm.totalDistanceCalculations = 0;
    return sm;
}


void sensing::updateMouse(SensingManager& sm, int x, int y, bool down) {
    sm.mousePos.x = x;
    sm.mousePos.y = y;
    sm.mouseDown = down;
}

void sensing::updateKey(SensingManager& sm, const string& key, bool pressed) {
    if (pressed) {
        if (find(sm.pressedKeys.begin(), sm.pressedKeys.end(), key)
            == sm.pressedKeys.end()) {
            sm.pressedKeys.push_back(key);
        }
    } else {
        auto it = find(sm.pressedKeys.begin(), sm.pressedKeys.end(), key);
        if (it != sm.pressedKeys.end()) {
            sm.pressedKeys.erase(it);
        }
    }
}

void sensing::updateSprite(SensingManager& sm, const SpriteInfo& info) {
    sm.sprites[info.name] = info;
}

void sensing::removeSprite(SensingManager& sm, const string& name) {
    sm.sprites.erase(name);
}

void sensing::clearAllSprites(SensingManager& sm) {
    sm.sprites.clear();
}


bool sensing::isTouchingMouse(const SensingManager& sm, const string& spriteName) {
    auto it = sm.sprites.find(spriteName);
    if (it == sm.sprites.end() || !it->second.visible) return false;

    const auto& sprite = it->second;
    bool result = pointInRect(sm.mousePos, sprite.pos, sprite.width, sprite.height);

    if (result) {
        const_cast<SensingManager&>(sm).totalCollisions++;
    }

    return result;
}

bool sensing::isTouchingEdge(const SensingManager& sm, const string& spriteName,
                            double stageWidth, double stageHeight) {
    auto it = sm.sprites.find(spriteName);
    if (it == sm.sprites.end() || !it->second.visible) return false;

    const auto& sprite = it->second;
    double left = sprite.pos.x - sprite.width/2;
    double right = sprite.pos.x + sprite.width/2;
    double top = sprite.pos.y - sprite.height/2;
    double bottom = sprite.pos.y + sprite.height/2;

    bool result = (left <= 0 || right >= stageWidth || top <= 0 || bottom >= stageHeight);

    if (result) {
        const_cast<SensingManager&>(sm).totalCollisions++;
    }

    return result;
}

bool sensing::isTouchingSprite(const SensingManager& sm,
                              const string& sprite1, const string& sprite2) {
    auto it1 = sm.sprites.find(sprite1);
    auto it2 = sm.sprites.find(sprite2);

    if (it1 == sm.sprites.end() || it2 == sm.sprites.end() ||
        !it1->second.visible || !it2->second.visible) return false;

    bool result = rectsOverlap(it1->second.pos, it1->second.width, it1->second.height,
                               it2->second.pos, it2->second.width, it2->second.height);

    if (result) {
        const_cast<SensingManager&>(sm).totalCollisions++;
    }

    return result;
}

bool sensing::isTouchingColor(SensingManager& sm, const string& spriteName, const Color& color) {
    if (!sm.getPixelColor) {
        cerr << "[SensingManager] Error: No pixel color function set!" << endl;
        return false;
    }

    auto it = sm.sprites.find(spriteName);
    if (it == sm.sprites.end() || !it->second.visible) return false;

    const auto& sprite = it->second;

    // بررسی ۹ نقطه (۳x۳)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double x = sprite.pos.x - sprite.width/2 + (i * sprite.width / 2);
            double y = sprite.pos.y - sprite.height/2 + (j * sprite.height / 2);

            Color pixelColor = sm.getPixelColor(static_cast<int>(x), static_cast<int>(y));
            if (pixelColor == color) {
                sm.totalCollisions++;
                return true;
            }
        }
    }
    return false;
}

bool sensing::isTouchingAnySprite(const SensingManager& sm, const string& spriteName) {
    auto it = sm.sprites.find(spriteName);
    if (it == sm.sprites.end() || !it->second.visible) return false;

    const auto& sprite1 = it->second;

    for (const auto& pair : sm.sprites) {
        if (pair.first == spriteName || !pair.second.visible) continue;

        if (rectsOverlap(sprite1.pos, sprite1.width, sprite1.height,
                         pair.second.pos, pair.second.width, pair.second.height)) {
            const_cast<SensingManager&>(sm).totalCollisions++;
            return true;
        }
    }
    return false;
}

double sensing::getDistanceToMouse(const SensingManager& sm, const string& spriteName) {
    auto it = sm.sprites.find(spriteName);
    if (it == sm.sprites.end()) return -1.0;

    const_cast<SensingManager&>(sm).totalDistanceCalculations++;
    return distance(it->second.pos, sm.mousePos);
}

double sensing::getDistanceBetweenSprites(const SensingManager& sm,
                                         const string& sprite1, const string& sprite2) {
    auto it1 = sm.sprites.find(sprite1);
    auto it2 = sm.sprites.find(sprite2);

    if (it1 == sm.sprites.end() || it2 == sm.sprites.end()) return -1.0;

    const_cast<SensingManager&>(sm).totalDistanceCalculations++;
    return distance(it1->second.pos, it2->second.pos);
}

double sensing::getDistanceToPoint(const SensingManager& sm, const string& spriteName,
                                  double x, double y) {
    auto it = sm.sprites.find(spriteName);
    if (it == sm.sprites.end()) return -1.0;

    const_cast<SensingManager&>(sm).totalDistanceCalculations++;
    return distance(it->second.pos, Position(x, y));
}


bool sensing::isKeyPressed(const SensingManager& sm, const string& key) {
    return find(sm.pressedKeys.begin(), sm.pressedKeys.end(), key)
           != sm.pressedKeys.end();
}

bool sensing::isAnyKeyPressed(const SensingManager& sm) {
    return !sm.pressedKeys.empty();
}

bool sensing::isMouseDown(const SensingManager& sm) {
    return sm.mouseDown;
}

Position sensing::getMousePos(const SensingManager& sm) {
    return sm.mousePos;
}

double sensing::getMouseX(const SensingManager& sm) {
    return sm.mousePos.x;
}

double sensing::getMouseY(const SensingManager& sm) {
    return sm.mousePos.y;
}


void sensing::resetTimer(SensingManager& sm) {
    sm.startTime = chrono::steady_clock::now();
}

double sensing::getTimerSeconds(const SensingManager& sm) {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - sm.startTime);
    return elapsed.count() / 1000.0;
}

double sensing::getTimerMilliseconds(const SensingManager& sm) {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - sm.startTime);
    return elapsed.count();
}


void sensing::askQuestion(SensingManager& sm, const string& question) {
    sm.lastQuestion = question;
    sm.waitingForAnswer = true;
    cout << "[SensingManager] Question: " << question << endl;
}

void sensing::submitAnswer(SensingManager& sm, const string& answer) {
    sm.lastAnswer = answer;
    sm.waitingForAnswer = false;
    cout << "[SensingManager] Answer received: " << answer << endl;
}

bool sensing::isAwaitingAnswer(const SensingManager& sm) {
    return sm.waitingForAnswer;
}

string sensing::getLastAnswer(const SensingManager& sm) {
    return sm.lastAnswer;
}

string sensing::getLastQuestion(const SensingManager& sm) {
    return sm.lastQuestion;
}


SpriteInfo* sensing::getSpriteInfo(SensingManager& sm, const string& name) {
    auto it = sm.sprites.find(name);
    if (it != sm.sprites.end()) {
        return &it->second;
    }
    return nullptr;
}

vector<string> sensing::getAllSpriteNames(const SensingManager& sm) {
    vector<string> names;
    for (const auto& pair : sm.sprites) {
        names.push_back(pair.first);
    }
    return names;
}

int sensing::getSpriteCount(const SensingManager& sm) {
    return sm.sprites.size();
}

void sensing::setPixelColorFunction(SensingManager& sm, function<Color(int,int)> func) {
    sm.getPixelColor = func;
}


void sensing::printSensingStats(const SensingManager& sm) {
    cout << "=== Sensing Manager Stats ===" << endl;
    cout << "Total collisions detected: " << sm.totalCollisions << endl;
    cout << "Total distance calculations: " << sm.totalDistanceCalculations << endl;
    cout << "Sprites tracked: " << sm.sprites.size() << endl;
    cout << "Keys pressed: " << sm.pressedKeys.size() << endl;
    cout << "Mouse position: (" << sm.mousePos.x << ", " << sm.mousePos.y << ")" << endl;
    cout << "Timer: " << getTimerSeconds(sm) << "s" << endl;
}

void sensing::resetStats(SensingManager& sm) {
    sm.totalCollisions = 0;
    sm.totalDistanceCalculations = 0;
}
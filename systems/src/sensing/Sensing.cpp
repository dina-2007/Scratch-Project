// Sensing.cpp
#include "Sensing.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace sensing;


SensingStore sensing::createSensingStore() {
    SensingStore store;
    store.mousePos = Position(0, 0);
    store.mouseDown = false;
    store.startTime = chrono::steady_clock::now();
    store.waitingForAnswer = false;
    store.getPixelColorFunc = nullptr;
    return store;
}


void sensing::updateMouse(SensingStore& store, int x, int y, bool down) {
    store.mousePos.x = x;
    store.mousePos.y = y;
    store.mouseDown = down;
}

void sensing::updateKey(SensingStore& store, const string& key, bool pressed) {
    if (pressed) {
        if (find(store.pressedKeys.begin(), store.pressedKeys.end(), key)
            == store.pressedKeys.end()) {
            store.pressedKeys.push_back(key);
        }
    } else {
        auto it = find(store.pressedKeys.begin(), store.pressedKeys.end(), key);
        if (it != store.pressedKeys.end()) {
            store.pressedKeys.erase(it);
        }
    }
}

void sensing::updateSprite(SensingStore& store, const SpriteInfo& info) {
    store.sprites[info.name] = info;
}

void sensing::removeSprite(SensingStore& store, const string& name) {
    store.sprites.erase(name);
}


static bool isPointInRect(const Position& point,
                          const Position& rectPos, double w, double h) {
    return (point.x >= rectPos.x - w/2 &&
            point.x <= rectPos.x + w/2 &&
            point.y >= rectPos.y - h/2 &&
            point.y <= rectPos.y + h/2);
}

static bool rectsCollide(const Position& pos1, double w1, double h1,
                         const Position& pos2, double w2, double h2) {
    double left1 = pos1.x - w1/2;
    double right1 = pos1.x + w1/2;
    double top1 = pos1.y - h1/2;
    double bottom1 = pos1.y + h1/2;

    double left2 = pos2.x - w2/2;
    double right2 = pos2.x + w2/2;
    double top2 = pos2.y - h2/2;
    double bottom2 = pos2.y + h2/2;

    return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
}


bool sensing::isTouchingMouse(const SensingStore& store, const string& spriteName) {
    auto it = store.sprites.find(spriteName);
    if (it == store.sprites.end() || !it->second.visible) return false;

    const auto& sprite = it->second;
    return isPointInRect(store.mousePos, sprite.pos, sprite.width, sprite.height);
}

bool sensing::isTouchingEdge(const SensingStore& store, const string& spriteName,
                            double stageWidth, double stageHeight) {
    auto it = store.sprites.find(spriteName);
    if (it == store.sprites.end() || !it->second.visible) return false;

    const auto& sprite = it->second;
    double left = sprite.pos.x - sprite.width/2;
    double right = sprite.pos.x + sprite.width/2;
    double top = sprite.pos.y - sprite.height/2;
    double bottom = sprite.pos.y + sprite.height/2;

    return (left <= 0 || right >= stageWidth || top <= 0 || bottom >= stageHeight);
}

bool sensing::isTouchingSprite(const SensingStore& store,
                              const string& sprite1, const string& sprite2) {
    auto it1 = store.sprites.find(sprite1);
    auto it2 = store.sprites.find(sprite2);

    if (it1 == store.sprites.end() || it2 == store.sprites.end() ||
        !it1->second.visible || !it2->second.visible) return false;

    return rectsCollide(it1->second.pos, it1->second.width, it1->second.height,
                       it2->second.pos, it2->second.width, it2->second.height);
}

bool sensing::isTouchingColor(SensingStore& store, const string& spriteName, const Color& color) {
    if (!store.getPixelColorFunc) {
        cerr << "Error: getPixelColor function not set!" << endl;
        return false;
    }

    auto it = store.sprites.find(spriteName);
    if (it == store.sprites.end() || !it->second.visible) return false;

    const auto& sprite = it->second;

    int steps = 5;
    for (int i = 0; i < steps; i++) {
        for (int j = 0; j < steps; j++) {
            double x = sprite.pos.x - sprite.width/2 + (i * sprite.width / (steps-1));
            double y = sprite.pos.y - sprite.height/2 + (j * sprite.height / (steps-1));

            Color pixelColor = store.getPixelColorFunc(static_cast<int>(x), static_cast<int>(y));
            if (pixelColor == color) {
                return true;
            }
        }
    }
    return false;
}


static double distance(const Position& a, const Position& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

double sensing::distanceToMouse(const SensingStore& store, const string& spriteName) {
    auto it = store.sprites.find(spriteName);
    if (it == store.sprites.end()) return -1.0;

    return distance(it->second.pos, store.mousePos);
}

double sensing::distanceToSprite(const SensingStore& store,
                                const string& sprite1, const string& sprite2) {
    auto it1 = store.sprites.find(sprite1);
    auto it2 = store.sprites.find(sprite2);

    if (it1 == store.sprites.end() || it2 == store.sprites.end()) return -1.0;

    return distance(it1->second.pos, it2->second.pos);
}


bool sensing::isKeyPressed(const SensingStore& store, const string& key) {
    return find(store.pressedKeys.begin(), store.pressedKeys.end(), key)
           != store.pressedKeys.end();
}

bool sensing::isMouseDown(const SensingStore& store) {
    return store.mouseDown;
}

Position sensing::getMousePosition(const SensingStore& store) {
    return store.mousePos;
}

double sensing::getMouseX(const SensingStore& store) {
    return store.mousePos.x;
}

double sensing::getMouseY(const SensingStore& store) {
    return store.mousePos.y;
}


void sensing::resetTimer(SensingStore& store) {
    store.startTime = chrono::steady_clock::now();
}

double sensing::getTimer(const SensingStore& store) {
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - store.startTime);
    return elapsed.count() / 1000.0;
}


void sensing::ask(SensingStore& store, const string& question) {
    store.lastQuestion = question;
    store.waitingForAnswer = true;
    cout << "Question: " << question << endl;
}

void sensing::submitAnswer(SensingStore& store, const string& answer) {
    store.lastAnswer = answer;
    store.waitingForAnswer = false;
    cout << "Answer received: " << answer << endl;
}

bool sensing::isWaitingForAnswer(const SensingStore& store) {
    return store.waitingForAnswer;
}

string sensing::getAnswer(const SensingStore& store) {
    return store.lastAnswer;
}

string sensing::getLastQuestion(const SensingStore& store) {
    return store.lastQuestion;
}


void sensing::setGetPixelColorFunc(SensingStore& store, function<Color(int,int)> func) {
    store.getPixelColorFunc = func;
}
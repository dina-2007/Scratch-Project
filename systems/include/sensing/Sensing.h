// Sensing.h
#ifndef SENSING_H
#define SENSING_H

#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <unordered_map>
#include "../variables/variable.h"

namespace sensing {


    struct Position {
        double x;
        double y;

        Position() : x(0), y(0) {}
        Position(double _x, double _y) : x(_x), y(_y) {}
    };

    struct Color {
        int r;
        int g;
        int b;

        Color() : r(0), g(0), b(0) {}
        Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}

        bool operator==(const Color& other) const {
            return r == other.r && g == other.g && b == other.b;
        }
    };

    struct SpriteInfo {
        std::string name;
        Position pos;
        double width;
        double height;
        bool visible;

        SpriteInfo() : name(""), pos(0,0), width(0), height(0), visible(true) {}
    };


    struct SensingStore {
        Position mousePos;
        bool mouseDown;
        std::vector<std::string> pressedKeys;
        std::unordered_map<std::string, SpriteInfo> sprites;
        std::chrono::steady_clock::time_point startTime;
        std::string lastQuestion;
        std::string lastAnswer;
        bool waitingForAnswer;
        std::function<Color(int,int)> getPixelColorFunc;
    };



    SensingStore createSensingStore();



    void updateMouse(SensingStore& store, int x, int y, bool down);
    void updateKey(SensingStore& store, const std::string& key, bool pressed);
    void updateSprite(SensingStore& store, const SpriteInfo& info);
    void removeSprite(SensingStore& store, const std::string& name);


    bool isTouchingMouse(const SensingStore& store, const std::string& spriteName);
    bool isTouchingEdge(const SensingStore& store, const std::string& spriteName,
                        double stageWidth, double stageHeight);

    bool isTouchingSprite(const SensingStore& store,
                          const std::string& sprite1, const std::string& sprite2);

    bool isTouchingColor(SensingStore& store, const std::string& spriteName, const Color& color);



    double distanceToMouse(const SensingStore& store, const std::string& spriteName);
    double distanceToSprite(const SensingStore& store,
                           const std::string& sprite1, const std::string& sprite2);



    bool isKeyPressed(const SensingStore& store, const std::string& key);
    bool isMouseDown(const SensingStore& store);
    Position getMousePosition(const SensingStore& store);
    double getMouseX(const SensingStore& store);
    double getMouseY(const SensingStore& store);



    void resetTimer(SensingStore& store);
    double getTimer(const SensingStore& store);



    void ask(SensingStore& store, const std::string& question);
    void submitAnswer(SensingStore& store, const std::string& answer);
    bool isWaitingForAnswer(const SensingStore& store);
    std::string getAnswer(const SensingStore& store);
    std::string getLastQuestion(const SensingStore& store);



    void setGetPixelColorFunc(SensingStore& store,
                              std::function<Color(int,int)> func);

}

#endif
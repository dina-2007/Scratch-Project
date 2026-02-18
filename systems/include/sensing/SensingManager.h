// SensingManager.h
#ifndef SENSING_MANAGER_H
#define SENSING_MANAGER_H

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
        int r, g, b;

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



    struct SensingManager {
        Position mousePos;
        bool mouseDown;

        std::vector<std::string> pressedKeys;

        std::unordered_map<std::string, SpriteInfo> sprites;

        std::chrono::steady_clock::time_point startTime;

        std::string lastQuestion;
        std::string lastAnswer;
        bool waitingForAnswer;

        std::function<Color(int,int)> getPixelColor;

        int totalCollisions;
        int totalDistanceCalculations;
    };


    SensingManager createSensingManager();


    void updateMouse(SensingManager& sm, int x, int y, bool down);
    void updateKey(SensingManager& sm, const std::string& key, bool pressed);
    void updateSprite(SensingManager& sm, const SpriteInfo& info);
    void removeSprite(SensingManager& sm, const std::string& name);
    void clearAllSprites(SensingManager& sm);



    bool isTouchingMouse(const SensingManager& sm, const std::string& spriteName);
    bool isTouchingEdge(const SensingManager& sm, const std::string& spriteName,
                        double stageWidth, double stageHeight);
    bool isTouchingSprite(const SensingManager& sm,
                          const std::string& sprite1, const std::string& sprite2);
    bool isTouchingColor(SensingManager& sm, const std::string& spriteName, const Color& color);
    bool isTouchingAnySprite(const SensingManager& sm, const std::string& spriteName);



    double getDistanceToMouse(const SensingManager& sm, const std::string& spriteName);
    double getDistanceBetweenSprites(const SensingManager& sm,
                                     const std::string& sprite1, const std::string& sprite2);
    double getDistanceToPoint(const SensingManager& sm, const std::string& spriteName,
                              double x, double y);



    bool isKeyPressed(const SensingManager& sm, const std::string& key);
    bool isAnyKeyPressed(const SensingManager& sm);
    bool isMouseDown(const SensingManager& sm);
    Position getMousePos(const SensingManager& sm);
    double getMouseX(const SensingManager& sm);
    double getMouseY(const SensingManager& sm);



    void resetTimer(SensingManager& sm);
    double getTimerSeconds(const SensingManager& sm);
    double getTimerMilliseconds(const SensingManager& sm);



    void askQuestion(SensingManager& sm, const std::string& question);
    void submitAnswer(SensingManager& sm, const std::string& answer);
    bool isAwaitingAnswer(const SensingManager& sm);
    std::string getLastAnswer(const SensingManager& sm);
    std::string getLastQuestion(const SensingManager& sm);



    SpriteInfo* getSpriteInfo(SensingManager& sm, const std::string& name);
    std::vector<std::string> getAllSpriteNames(const SensingManager& sm);
    int getSpriteCount(const SensingManager& sm);



    void setPixelColorFunction(SensingManager& sm, std::function<Color(int,int)> func);


    void printSensingStats(const SensingManager& sm);
    void resetStats(SensingManager& sm);

}

#endif
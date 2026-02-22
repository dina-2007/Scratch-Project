#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>

class Sprite {
public:
    Sprite();
    ~Sprite();

    bool loadTexture(SDL_Renderer* renderer, const std::string& path);

    void setPosition(int x, int y);
    void setRotation(double angle);

    void render(SDL_Renderer* renderer);

    int getX() const;
    int getY() const;

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    double rotation;
};

#endif

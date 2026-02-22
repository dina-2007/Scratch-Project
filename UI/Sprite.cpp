#include "Sprite.h"
#include <SDL_image.h>

Sprite::Sprite() : texture(nullptr), rotation(0) {
    rect = {100, 100, 100, 100};
}

Sprite::~Sprite() {
    if(texture)
        SDL_DestroyTexture(texture);
}

bool Sprite::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if(!surface) return false;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture != nullptr;
}

void Sprite::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Sprite::setRotation(double angle) {
    rotation = angle;
}

void Sprite::render(SDL_Renderer* renderer) {
    if(texture)
        SDL_RenderCopyEx(renderer, texture, nullptr, &rect, rotation, nullptr, SDL_FLIP_NONE);
}

int Sprite::getX() const { return rect.x; }
int Sprite::getY() const { return rect.y; }

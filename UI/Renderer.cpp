#include "Renderer.h"
#include <iostream>

RendererSystem::RendererSystem() : window(nullptr), renderer(nullptr) {}

RendererSystem::~RendererSystem() {
    clean();
}

bool RendererSystem::init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;

    window = SDL_CreateWindow("Scratch Engine",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 600,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return renderer != nullptr;
}

void RendererSystem::render(Sprite& sprite, EngineData& engine) {
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderClear(renderer);

    // Sync sprite with engine position
    sprite.setPosition(engine.state.x + 400,
                       300 - engine.state.y);
    sprite.setRotation(engine.state.direction);

    sprite.render(renderer);

    SDL_RenderPresent(renderer);
}

void RendererSystem::clean() {
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Renderer* RendererSystem::getRenderer() {
    return renderer;
}

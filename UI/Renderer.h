#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "Sprite.h"
#include "engine.h"

class RendererSystem {
public:
    RendererSystem();
    ~RendererSystem();

    bool init();
    void render(Sprite& sprite, EngineData& engine);
    void clean();

    SDL_Renderer* getRenderer();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif

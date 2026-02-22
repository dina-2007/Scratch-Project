#include "UIManager.h"

bool UIManager::init() {
    if(!rendererSystem.init()) return false;

    sprite.loadTexture(rendererSystem.getRenderer(), "assets/sprite.png");

    blocks.emplace_back(20,50,120,40,"MOVE");
    blocks.emplace_back(20,100,120,40,"TURN");

    return true;
}

void UIManager::mainLoop(EngineData& engine) {
    SDL_Event e;
    bool quit = false;

    while(!quit) {

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
                quit = true;

            for(auto& b : blocks)
                b.handleEvent(e);

            if(e.type == SDL_KEYDOWN) {
                dispatch_Event(engine, WHEN_KEY,
                               std::string(1,(char)e.key.keysym.sym));
            }
        }

        tick(engine);

        rendererSystem.render(sprite, engine);
    }

    rendererSystem.clean();
}

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "Renderer.h"
#include "Sprite.h"
#include "PenSystem.h"
#include "BlockView.h"
#include <vector>

class UIManager {
public:
    bool init();
    void mainLoop(EngineData& engine);

private:
    RendererSystem rendererSystem;
    Sprite sprite;
    PenSystem pen;
    std::vector<BlockView> blocks;
};

#endif

#ifndef BLOCK_VIEW_H
#define BLOCK_VIEW_H

#include <SDL.h>
#include <string>

class BlockView {
public:
    BlockView(int x,int y,int w,int h,const std::string& label);

    void render(SDL_Renderer* renderer);
    void handleEvent(SDL_Event& e);

private:
    SDL_Rect rect;
    std::string label;
    bool dragging;
};

#endif

#include "BlockView.h"

BlockView::BlockView(int x,int y,int w,int h,const std::string& l)
    : label(l), dragging(false) {
    rect = {x,y,w,h};
}

void BlockView::render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,100,150,255,255);
    SDL_RenderFillRect(renderer,&rect);
}

void BlockView::handleEvent(SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){
        if(SDL_PointInRect(&SDL_Point{e.button.x,e.button.y},&rect)){
            dragging = true;
        }
    }
    if(e.type == SDL_MOUSEBUTTONUP)
        dragging = false;

    if(e.type == SDL_MOUSEMOTION && dragging){
        rect.x = e.motion.x;
        rect.y = e.motion.y;
    }
}

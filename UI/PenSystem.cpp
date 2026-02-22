#include "PenSystem.h"

void PenSystem::addLine(int x1,int y1,int x2,int y2){
    lines.push_back({x1,y1,x2,y2});
}

void PenSystem::render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    for(auto& l: lines){
        SDL_RenderDrawLine(renderer,l.x1,l.y1,l.x2,l.y2);
    }
}

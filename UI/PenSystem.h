#ifndef PEN_SYSTEM_H
#define PEN_SYSTEM_H

#include <SDL.h>
#include <vector>

struct Line {
    int x1,y1,x2,y2;
};

class PenSystem {
public:
    void addLine(int x1,int y1,int x2,int y2);
    void render(SDL_Renderer* renderer);

private:
    std::vector<Line> lines;
};

#endif

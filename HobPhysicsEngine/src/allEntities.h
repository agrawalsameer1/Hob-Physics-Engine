#include "../include/SDL2/SDL.h"
#include <iostream>
#include <vector>
#include "rectEntity.h"
#pragma once

class allEntities {
    public:
        std::vector<rectEntity*> rects;
        int size;
        allEntities();
        void addRect(rectEntity* rect);
        rectEntity* removeRect(int index, rectEntity* trackedEntity);
        void render(SDL_Renderer* renderer, SDL_Window *window);
        rectEntity* update(SDL_Window *window, rectEntity* trackedRect, double dTime);
};
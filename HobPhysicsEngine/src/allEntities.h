#include "../include/SDL2/SDL.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "entity.h"
#include "QuadTree.h"

#pragma once


class allEntities {
    public:
        unsigned char *data;
        std::vector<entity*> rects;
        int size;
        double prevSum = 0;
        double currSum = 0;
        allEntities(unsigned char *gradData);
        void addRect(entity* rect);
        entity* removeRect(int index, entity* trackedEntity);
        void render(SDL_Renderer* renderer, SDL_Window *window);
        void calculateGravity(float G, allEntities massFull);
        void barnesHut(SDL_Window *window, float G, QuadTree* tree, node* b, entity* a);
        entity* update(SDL_Window *window, entity* trackedRect, double dTime);
        QuadTree* update2(SDL_Window *window, entity* trackedRect, double dTime, float G, QuadTree* tree);
        QuadTree* update3(SDL_Window *window, entity* trackedRect, double dTime, int windowSize, float G, QuadTree* tree);//, int width, int height);//, allEntities massFull);
        QuadTree* rungeKutta(SDL_Window *window, double dTime, float G, QuadTree* tree);
};
#include "../include/SDL2/SDL.h"
#include <iostream>
#include<cstdlib>
#include <math.h>
#include "textBox.h"
#define PI 3.1415926535897932384626433832795028841971

#pragma once

class rectEntity {
    public:
        double X,Y,W,H;
        int R,G,B;
        double Xvelocity, Yvelocity;
        double mass;
        textBox massText;
        textBox velocityText;
        SDL_Rect* rect;
        rectEntity();
        rectEntity(double w, double h, double x, double y, int r, int g, int b);
        void updatePos(double dTime);
        bool checkBounds(SDL_Window *window);
        void bounceWall(SDL_Window *window);
        bool isColliding (rectEntity rect);
        void calculateCollision(rectEntity &rect);
        void render(SDL_Renderer* renderer);
        double calculateVelocity();
        double calculateAngle();
};
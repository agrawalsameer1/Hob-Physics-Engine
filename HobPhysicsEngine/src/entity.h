#include "../include/SDL2/SDL.h"
#include "../include/SDL2_image/SDL2/SDL_image.h"
#include <iostream>
#include<cstdlib>
#include <math.h>
#include <vector>
#define PI 3.1415926535897932384626433832795028841971

#pragma once

float Dist(float x1, float x2, float y1, float y2);

class entity {
    public:
        int id;
        double X,Y,Rad;
        int R,G,B;
        double Xvelocity, Yvelocity;
        double Xacceleration, Yacceleration;
        double mass;
        entity();
        entity(double rad, double x, double y, int r, int g, int b, int id);
        entity(double rad, double x, double y, int r, int g, int b, int vx, int vy, int id);
        void updatePos(double dTime);
        void reset();
        bool checkBounds(SDL_Window *window);
        void bounceWall(SDL_Window *window);
        bool isColliding (entity rect);
        void calculateCollision(entity &rect);
        void calculateGravity(float G, entity& rect);
        void render(SDL_Renderer* renderer);
        double calculateVelocity();
        double calculateAngle();
        void incrementAccelerations(float xacceleration, float yacceleration);
};
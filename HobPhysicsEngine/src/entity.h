#include "../include/SDL2/SDL.h"
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
        double prevX, prevY;
        float R,G,B;
        double Xvelocity, Yvelocity;
        double Xacceleration, Yacceleration;
        double Xdisplacement, Ydisplacement;
        double mass;
        double colorPoint = 0;
        bool attractor = false;
        bool hasCollided = false;
        double kValsDisplacement[8];
        double kValsVelocity[8]; 
        entity();
        entity(double rad, double x, double y, int r, int g, int b, int id);
        entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int id);
        entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int id, double m);
        entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int id, double m, bool attract);
        void updatePos(double dTime);
        void reset();
        bool checkBounds(SDL_Window *window);
        void bounceWall(SDL_Window *window);
        bool isColliding (entity rect);
        void calculateCollision(entity &rect);
        void calculateGravity(float G, entity& rect);
        void render(SDL_Renderer* renderer);
        void incrementAccelerations(float xacceleration, float yacceleration);
};
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
        // Unique number assigned to every entity
        int id;

        // X & Y coords, and radius of entity
        double X,Y,Rad;

        // Used in runge-Kutta
        double prevX, prevY;

        // Color of entity, only used for rendering
        float R,G,B;

        // Velocity of entity
        double Xvelocity, Yvelocity;

        // Accelerations of entities, only changed by gravitational forces
        double Xacceleration, Yacceleration;

        // Used for QuadTree optimization
        double Xdisplacement, Ydisplacement;

        // Entity mass
        double mass;

        // Used for real-time color updating
        double colorPoint = 0;

        // If a particle is an attractor, it does not move, and only attracts other entities
        bool attractor = false;

        // k Values for runge-Kutta
        double kValsDisplacement[8];
        double kValsVelocity[8]; 

        // Constructors
        entity(double rad, double x, double y, int r, int g, int b, int id);
        entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int id);
        entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int id, double m);
        entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int id, double m, bool attract);

        // Use velocities and accelerations to move entity. (unnecessary now that I have runge-Kutta)
        void updatePos(double dTime);

        // Set acceleration of 0
        void reset();

        // Check if entity is outside window boundary, and if so, bounce it off the wall (not used for cosmological simulations)
        bool checkBounds(SDL_Window *window);
        void bounceWall(SDL_Window *window);

        // Check and calculate collision (assume collision are ellastic)
        bool isColliding(entity rect);
        void calculateCollision(entity &rect);

        // Calculate gravity between two entities using Newton's Law of gravitation
        void calculateGravity(float G, entity& rect);

        // Render entity
        void render(SDL_Renderer* renderer);
};
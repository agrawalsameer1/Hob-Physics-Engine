#include "../include/SDL2/SDL.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "entity.h"
#include "QuadTree.h"

#pragma once

// Class containing all physics entities and methods to manipulate them as a system
class allEntities {
    public:
        std::vector<entity*> entities; // List of all physical bodies in play
        int size; // Number of physics bodies

        // Total sum of displacements for particles, used for QuadTree optimization
        double prevSum = 0; 
        double currSum = 0;

        unsigned char *data; // Color gradient data, used for dynamic particle color updating

        // Assign color gradient data
        allEntities(unsigned char *gradData = nullptr);

        // Add/remove new entity to physics/rendering queue
        void addEntity(entity* rect);
        void removeEntity(int index = -1, entity* entityToRemove = nullptr);

        // Render all entities
        void render(SDL_Renderer* renderer, SDL_Window *window);

        // Calculate gravitational force/acceleration between two entities. Allows for entities with zero mass
        void calculateGravity(float G, allEntities massFull);

        // Implementation of Barnes-Hut optimization for n-body simulation
        void barnesHut(SDL_Window *window, float G, QuadTree* tree, node* b, entity* a);
        
        // Performs collision-detection and actually runs gravitational force calculation. Uses Runge-Kutta method
        QuadTree* update(SDL_Window *window, double dTime, float windowSize, float G, QuadTree* tree);//, int width, int height);//, allEntities massFull);

        // Implementation of 4th order Runge-Kutta Method
        QuadTree* rungeKutta(SDL_Window *window, double dTime, float G, QuadTree* tree);
};
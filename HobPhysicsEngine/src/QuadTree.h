#include "entity.h"
#include <iomanip>
#define PI 3.14159265358979
#pragma once

enum Directions {
  NW, NE, SW, SE
};

class node {
  public:
    entity* myEntity;
    bool hasEntity;
    float* pt1;
    float* pt2;
    node* parent;
    float mass;
    float* COM;
    std::vector <node*> pointers {nullptr, nullptr, nullptr, nullptr};
    node();
    void removeEntity();
};

class QuadTree {
    public:
      node* root;
      int numOfElements;
      QuadTree(SDL_Window* window);
      Directions calcDirection(node* n, entity* b);
      bool insert(entity* b, node* n);
      void textDisplay(node* n, int numOfTabs);
      void visualDisplayTool(node* n, SDL_Renderer* renderer);
      void visualDisplay(node* n, SDL_Renderer* renderer);
      void deallocate(node* n);
      void countElements(node* n);
};
#include "entity.h"
#include <iomanip>
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
    int mass;
    float* COM;
    std::vector <node*> pointers {nullptr, nullptr, nullptr, nullptr};
    node();
    void removeEntity();
};

class QuadTree {
    public:
      node* root;
      std::vector <node*> allPointersInTree;
      QuadTree(SDL_Window* window);
      ~QuadTree();
      Directions calcDirection(node* n, entity* b);
      void insert(entity* b, node* n);
      void textDisplay(node* n, int numOfTabs);
      void visualDisplayTool(node* n, SDL_Renderer* renderer);
      void visualDisplay(node* n, SDL_Renderer* renderer);
};
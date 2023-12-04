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
    float pt1[2];
    float pt2[2];
    node* parent;
    float mass;
    float COM[2];
    std::vector <node*> pointers {nullptr, nullptr, nullptr, nullptr};
    node();
    void removeEntity();
};

class QuadTree {
    public:
      node* root;
      int numOfElements;
      QuadTree(SDL_Window* window);
      QuadTree(int width, int height);
      Directions calcDirection(node* n, entity* b);
      bool insert(entity* b, node* n);
      void textDisplay(node* n, int numOfTabs);
      void visualDisplayTool(node* n, SDL_Renderer* renderer);
      void visualDisplay(node* n, SDL_Renderer* renderer);
      void deallocate(node* n);
      void countElements(node* n);
};
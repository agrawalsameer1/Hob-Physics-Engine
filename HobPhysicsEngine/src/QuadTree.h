#include "entity.h"
#include <iomanip>
#pragma once

// Defines four quadrants of a node
enum Directions {
  NW, NE, SW, SE
};

// QuadTree is made of many nodes
class node {
  public:
    // Entity contained within a node
    entity* myEntity;
    bool hasEntity;

    // Points describing boundary rect of node
    float pt1[2];
    float pt2[2];

    // Parent pointer
    node* parent;

    // Total mass of all entities within node
    float mass;

    // Center of mass of node
    float COM[2];

    // Pointers to four quadrants of node
    std::vector <node*> pointers {nullptr, nullptr, nullptr, nullptr};

    node();

    // Remove entity from node
    void removeEntity();
};

class QuadTree {
    public:
      node* root;
      int numOfElements;

      // Constructors with either custom width/height, or by passing full SDL Window
      QuadTree(SDL_Window* window);
      QuadTree(int width, int height);

      // Calculating in which quadrant an entity should be placed
      Directions calcDirection(node* n, entity* b);

      // Insert node into tree
      bool insert(entity* b, node* n);

      // Graphically display tree
      void visualDisplayTool(node* n, SDL_Renderer* renderer);
      void visualDisplay(node* n, SDL_Renderer* renderer);

      void deallocate(node* n);
      void countElements(node* n);
};
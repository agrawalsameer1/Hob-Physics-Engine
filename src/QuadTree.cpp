#include "QuadTree.h"

node::node() {
    pointers[NW] = nullptr;
    pointers[NE] = nullptr;
    pointers[SW] = nullptr;
    pointers[SE] = nullptr;
    hasEntity = false;
    myEntity = nullptr;
    mass = 0;
}

void node::removeEntity() {
    hasEntity = false;

    float prevX = mass*COM[0];
    float prevY = mass*COM[1];
    mass -= myEntity->mass;

    COM[0] = (prevX-myEntity->X*myEntity->mass)/mass;
    COM[1] = (prevY-myEntity->Y*myEntity->mass)/mass;

    myEntity = nullptr;
}

QuadTree::QuadTree(SDL_Window* window) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    root = new node();

    root->pt1[0] = 0;
    root->pt1[1] = 0;

    root->pt2[0] = static_cast<float>(w);
    root->pt2[1] = static_cast<float>(h);

    numOfElements = 0;
}

QuadTree::QuadTree(int width, int height) {
    root = new node();

    root->pt1[0] = 0;
    root->pt1[1] = 0;

    root->pt2[0] = static_cast<float>(width);
    root->pt2[1] = static_cast<float>(height);

    numOfElements = 0;
}

Directions QuadTree::calcDirection(node* n, entity* b) {
    float eastWest = (n->pt2[0]-n->pt1[0])/2 + n->pt1[0];
    float northSouth = (n->pt2[1]-n->pt1[1])/2 + n->pt1[1];

    if (b->X < eastWest) {
        if (b->Y < northSouth) { // Left-up
            return NW;
        }
        if (b->Y > northSouth || b->Y == northSouth) { // Left down
            return SW;
        }
    }
    else if (b->X == eastWest || b->X > eastWest) {
        if (b->Y < northSouth) { // Right-up
            return NE;
        }
        if (b->Y > northSouth || b->Y == northSouth) { // Right down
            return SE;
        }
    }
}

bool QuadTree::insert(entity* b, node* n) {
    if (b->X < 0 || b->X > root->pt2[0] || b->Y < 0 || b->Y > root->pt2[1]) {
        return false;
    }

    if (n->myEntity != nullptr) {
        if (fabs(n->myEntity->X - b->X) < 0.001 && fabs(n->myEntity->Y - b->Y) < 0.001 && b->id != n->myEntity->id) {
            n->mass = n->mass + b->mass;
            return true;
        }
    }

    Directions direction = calcDirection(n, b);

    float prevX = n->mass*n->COM[0];
    float prevY = n->mass*n->COM[1];
    n->mass += b->mass;
    n->COM[0] = (prevX+b->X*b->mass)/n->mass;
    n->COM[1] = (prevY+b->Y*b->mass)/n->mass;

    if (n->pointers[direction] == nullptr) {
        node* toAdd = new node();

        toAdd->myEntity = b;
        toAdd->parent = n;
        toAdd->hasEntity = true;

        float distX = n->pt2[0]-n->pt1[0];
        float distY = n->pt2[1]-n->pt1[1];
        
        float p1X = n->pt1[0] + distX*((direction%2)/2.0);
        float p1Y = n->pt1[1] + distY*((direction/2)/2.0);
        float p2X = n->pt1[0] + distX*((direction%2+1)/2.0);
        float p2Y = n->pt1[1] + distY*((direction/2+1)/2.0);

        toAdd->pt1[0] = p1X;
        toAdd->pt1[1] = p1Y;
        toAdd->pt2[0] = p2X;
        toAdd->pt2[1] = p2Y;

        toAdd->mass = b->mass;
        toAdd->COM[0] = b->X;
        toAdd->COM[1] = b->Y;

        n->pointers[direction] = toAdd;

        if (n->hasEntity == true) {
            n->hasEntity = false;
            insert(n->myEntity, n);
            n->removeEntity();
        }
        return true;
    }

    else {
        return insert(b, n->pointers[direction]);
    }
}

void QuadTree::textDisplay(node* n, int numOfTabs) {
    for (int i = 0; i < numOfTabs; i++) {
        std::cout << "\t";
    }

    if (n == nullptr) {
        std::cout << "no node at all!\n";
    }
    else {
        std::cout << n->COM[0] << "," << n->COM[1] << " " << n->mass << " ";
        if (n->hasEntity) {
            std::cout << n->myEntity->id << " (" << n->pt1[0] << "," << n->pt1[1] << "); (" << n->pt2[0] << "," << n->pt2[1] << ")\n";
        }
        else {
            std::cout << "no entity here!" << " (" << n->pt1[0] << "," << n->pt1[1] << "); (" << n->pt2[0] << "," << n->pt2[1] << ")\n";
        }
        textDisplay(n->pointers[NW], numOfTabs+1);
        textDisplay(n->pointers[NE], numOfTabs+1);
        textDisplay(n->pointers[SW], numOfTabs+1);
        textDisplay(n->pointers[SE], numOfTabs+1);
    }
}

void QuadTree::visualDisplayTool(node* n, SDL_Renderer* renderer) {
    if (n == nullptr) {
        return;
    }
    else {
        if (n->pointers[0] != nullptr || n->pointers[1] != nullptr || n->pointers[2] != nullptr || n->pointers[3] != nullptr) {
            float eastWest = (n->pt2[0]-n->pt1[0])/2 + n->pt1[0];
            float northSouth = (n->pt2[1]-n->pt1[1])/2 + n->pt1[1];

            SDL_RenderDrawLineF(renderer, eastWest, n->pt1[1], eastWest, n->pt2[1]);
            SDL_RenderDrawLineF(renderer, n->pt1[0], northSouth, n->pt2[0], northSouth);
            visualDisplayTool(n->pointers[NW], renderer);
            visualDisplayTool(n->pointers[NE], renderer);
            visualDisplayTool(n->pointers[SW], renderer);
            visualDisplayTool(n->pointers[SE], renderer);
        }
        else {
            return;
        }
    }
}

void QuadTree::visualDisplay(node* n, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    visualDisplayTool(n, renderer);
    //SDL_RenderPresent(renderer);
}

void QuadTree::countElements(node* n) {
    if (n == nullptr) {
        return;
    }
    else {
        if (n->myEntity) {
            numOfElements++;
        }
        countElements(n->pointers[NW]);
        countElements(n->pointers[NE]);
        countElements(n->pointers[SW]);
        countElements(n->pointers[SE]);
    }
}

void QuadTree::deallocate(node* n) {
    if (n == nullptr) {
        return;
    }
    else {
        deallocate(n->pointers[NW]);
        deallocate(n->pointers[NE]);
        deallocate(n->pointers[SW]);
        deallocate(n->pointers[SE]);
        delete(n);
    }
}


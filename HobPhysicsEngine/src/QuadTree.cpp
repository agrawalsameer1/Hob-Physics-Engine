#include "QuadTree.h"

node::node() {
    pointers[NW] = nullptr;
    pointers[NE] = nullptr;
    pointers[SW] = nullptr;
    pointers[SE] = nullptr;
    pt1 = new float[2];
    pt2 = new float[2];
    COM = new float[2];
    hasEntity = false;
    myEntity = nullptr;
    mass = 0;
}

void node::removeEntity() {
    hasEntity = false;

    float prevX = mass*COM[0];
    float prevY = mass*COM[1];
    mass -= myEntity->mass;
    COM[0] = (prevX-myEntity->X)/mass;
    COM[1] = (prevY-myEntity->Y)/mass;

    myEntity = nullptr;
}

QuadTree::QuadTree(SDL_Window* window) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    float* p1 = new float[2];
    p1[0] = 0;
    p1[1] = 0;

    float* p2 = new float[2];
    p2[0] = static_cast<float>(w);
    p2[1] = static_cast<float>(h);
    
    root = new node();

    root->pt1 = p1;
    root->pt2 = p2;

    allPointersInTree.push_back(root);
}

QuadTree::~QuadTree() {
    std::vector <node*> temp;

    for (int i = 0; i < allPointersInTree.size(); i++) {
        if (std::find(temp.begin(), temp.end(), allPointersInTree[i]) == temp.end()) {
            delete allPointersInTree[i];
        }
        //std::cout << allPointersInTree[i] << "\n";
        temp.push_back(allPointersInTree[i]);
    }
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

void QuadTree::insert(entity* b, node* n) {
    if (b->X < 0 || b->X > root->pt2[0] || b->Y < 0 || b->Y > root->pt2[1]) {
        return;
    }
    if (n->myEntity != nullptr) {
        if (fabs(n->myEntity->X - b->X) < 0.001 && fabs(n->myEntity->Y - b->Y) < 0.001 && b->id != n->myEntity->id) {
            //std::cout << "here!\n";
            return;
        }
    }
    Directions direction = calcDirection(n, b);
    float prevX = n->mass*n->COM[0];
    float prevY = n->mass*n->COM[1];
    n->mass += b->mass;
    n->COM[0] = (prevX+b->X)/n->mass;
    n->COM[1] = (prevY+b->Y)/n->mass;

    if (n->pointers[direction] == nullptr) {
        //

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
        //std::cout << "got here!\n";

        if (n->hasEntity == true) {
            //std::cout << (n->myEntity->Y == b->Y) << "\n";
            if (fabs(n->myEntity->X - b->X) < 0.001 && fabs(n->myEntity->Y - b->Y) < 0.001 && b->id != n->myEntity->id) {
                //std::cout << "here!\n";
                return;
            }
            //std::cout << "called for entity " << std::setprecision(15) << n->myEntity->X << " " << n->myEntity->Y << " " << b->X << " " << b->Y << "!\n";
            n->hasEntity = false;
            insert(n->myEntity, n);
            n->removeEntity();
        }
        //std::cout << "added to direction " << direction << "!\n";
        allPointersInTree.push_back(toAdd);
    }

    else {
        //std::cout << "traversing to direction " << direction << "!";
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


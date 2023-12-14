#include "QuadTree.h"

// Constructor for node
node::node() {
    pointers[NW] = nullptr;
    pointers[NE] = nullptr;
    pointers[SW] = nullptr;
    pointers[SE] = nullptr;
    hasEntity = false;
    myEntity = nullptr;
    mass = 0;
}

// Remove entity from node, updating mass and center of mass accordingly
void node::removeEntity() {
    hasEntity = false;

    float prevX = mass*COM[0];
    float prevY = mass*COM[1];
    mass -= myEntity->mass;

    COM[0] = (prevX-myEntity->X*myEntity->mass)/mass;
    COM[1] = (prevY-myEntity->Y*myEntity->mass)/mass;

    myEntity = nullptr;
}

// QuadTree constructor
QuadTree::QuadTree(SDL_Window* window) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    root = new node();

    // Assign entire boundary of window as bounding rect for root node
    root->pt1[0] = 0;
    root->pt1[1] = 0;
    root->pt2[0] = static_cast<float>(w);
    root->pt2[1] = static_cast<float>(h);

    numOfElements = 0;
}

// Construct QuadTree based on custom width and height
QuadTree::QuadTree(int width, int height) {
    root = new node();

    // Assign entire boundary of window as bounding rect for root node
    root->pt1[0] = 0;
    root->pt1[1] = 0;
    root->pt2[0] = static_cast<float>(width);
    root->pt2[1] = static_cast<float>(height);

    numOfElements = 0;
}

// Calculate quadrant of node to insert entity in
Directions QuadTree::calcDirection(node* n, entity* b) {

    // Divide quadrant into four smaller quadrants
    float eastWest = (n->pt2[0]-n->pt1[0])/2 + n->pt1[0];
    float northSouth = (n->pt2[1]-n->pt1[1])/2 + n->pt1[1];

    // If entity on left two quadrants
    if (b->X < eastWest) {
        if (b->Y < northSouth) { // Left-up
            return NW;
        }
        if (b->Y > northSouth || b->Y == northSouth) { // Left down
            return SW;
        }
    }

    // If entity on right two quadrants
    else if (b->X == eastWest || b->X > eastWest) {
        if (b->Y < northSouth) { // Right-up
            return NE;
        }
        if (b->Y > northSouth || b->Y == northSouth) { // Right down
            return SE;
        }
    }
}

// Insert entity into tree, returns true if successful
bool QuadTree::insert(entity* b, node* n) {

    // If entity is outside window boundary, return false
    if (b->X < 0 || b->X > root->pt2[0] || b->Y < 0 || b->Y > root->pt2[1]) {
        return false;
    }

    // If entity is already extremely close to a different entity in the tree, return false. (to avoid excessive tree depth)
    if (n->myEntity != nullptr) {
        if (fabs(n->myEntity->X - b->X) < 0.001 && fabs(n->myEntity->Y - b->Y) < 0.001 && b->id != n->myEntity->id) {
            n->mass = n->mass + b->mass;
            return true;
        }
    }

    // Calculate which direction entity should be inserted in
    Directions direction = calcDirection(n, b);

    // Update mass and COM
    float prevX = n->mass*n->COM[0];
    float prevY = n->mass*n->COM[1];
    n->mass += b->mass;
    n->COM[0] = (prevX+b->X*b->mass)/n->mass;
    n->COM[1] = (prevY+b->Y*b->mass)/n->mass;

    // If quadrant is empty, insert
    if (n->pointers[direction] == nullptr) {
        // Create new node with entity
        node* toAdd = new node();
        toAdd->myEntity = b;
        toAdd->parent = n;
        toAdd->hasEntity = true;

        // Define bounding rect of newly created node
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

        // Add mass and COM of node, COM is just coordinates of entity
        toAdd->mass = b->mass;
        toAdd->COM[0] = b->X;
        toAdd->COM[1] = b->Y;

        n->pointers[direction] = toAdd;

        // If parent quadrant already had an entity, insert this as well
        if (n->hasEntity == true) {
            n->hasEntity = false;
            insert(n->myEntity, n);
            n->removeEntity();
        }
        return true;
    }

    // If quadrant is not empty, recursively call insert on subtree
    else {
        return insert(b, n->pointers[direction]);
    }
}

// Visually display tree
void QuadTree::visualDisplayTool(node* n, SDL_Renderer* renderer) {
    if (n == nullptr) {
        return;
    }

    // If node has subquadrants 
    else {
        if (n->pointers[0] != nullptr || n->pointers[1] != nullptr || n->pointers[2] != nullptr || n->pointers[3] != nullptr) {
            float eastWest = (n->pt2[0]-n->pt1[0])/2 + n->pt1[0];
            float northSouth = (n->pt2[1]-n->pt1[1])/2 + n->pt1[1];

            // Draw lines to divide node into its quadrants
            SDL_RenderDrawLineF(renderer, eastWest, n->pt1[1], eastWest, n->pt2[1]);
            SDL_RenderDrawLineF(renderer, n->pt1[0], northSouth, n->pt2[0], northSouth);

            // Recursively draw function on subquadrants
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

// Set render draw color (cannot be done in recursive function above)
void QuadTree::visualDisplay(node* n, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    visualDisplayTool(n, renderer);
    //SDL_RenderPresent(renderer);
}

// Count elements in Quadtree
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


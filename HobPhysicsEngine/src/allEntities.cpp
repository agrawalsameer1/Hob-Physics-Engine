#include "allEntities.h"

double coloringFunction(double ratio) {
    double firstPart = 4.5/(1 + exp(-5.5*ratio));
    double num = firstPart - (0.5*4.5);
    return num/2.25;
}

// Assign size variable and add gradient data if specified by user
allEntities::allEntities(unsigned char *gradData) {
    size = 0;
    data = gradData;
}

// Add an entity to rendering queue
void allEntities::addEntity(entity* rect) {
    entities.push_back(rect);
    size += 1;
}

// Remove entity, either by index, or by referring to the entity itself
void allEntities::removeEntity(int index, entity* entityToRemove) {
    if (index > -1) {
        entities.erase(entities.begin()+index);
    }

    else if (entityToRemove) {
        for (int i = 0; i < size; i++) {
            if (entities[i] == entityToRemove) {
                entities.erase(entities.begin() + i);
            }
        }
    }

    else {
        return;
    }
}

// Render all entities to screen
void allEntities::render(SDL_Renderer* renderer, SDL_Window *window) {
    for (int i = 0; i < size; i++) {
        SDL_SetRenderDrawColor(renderer, entities[i]->R, entities[i]->G, entities[i]->B, 255); // Set rendering color to color of current rentity
        entities[i]->render(renderer); // Circle-drawing method
    }
}

// Update function for system of entities
QuadTree* allEntities::update(SDL_Window *window, double dTime, float windowsize, float G, QuadTree* tree) {
    // Represents size of partition for space-partitioning optimization for collision detection
    float windowSize = windowsize; 

    // For particle re-coloring
    double totalMass = 0;

    // Get SDL Window Dimensions
    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    // Calculates total number of partitions within window, with some extra buffer, and creates a vector to represent this
    int numOfWindows = ((w/windowsize))*((h/windowsize)+2);

    std::vector <int> intervals[numOfWindows];

    double totalMasses[numOfWindows];

    // Assign each element to a window
    for (int j = 0; j < entities.size(); j++) {
        // Calculate 4 corners of bounding box surrounding the entity
        int smallX = entities[j]->X - entities[j]->Rad;
        int smallY = entities[j]->Y - entities[j]->Rad;
        int largeX = entities[j]->X + entities[j]->Rad;
        int largeY = entities[j]->Y + entities[j]->Rad;

        // Continue if entity is outside of SDL window
        if (smallX < 0) {
            continue;
        }

        if (smallY < 0) {
            continue;
        }

        if (largeX > w) {
            continue;
        }

        if (largeY > h) {
            continue;
        }

        // Calculate which partition the object is in
        int xInterval = smallX/windowsize;
        int xInterval2 = largeX/windowsize;
        int yInterval = smallY/windowsize;
        int yInterval2 = largeY/windowsize;

        int interval1 = yInterval*(w/windowsize) + xInterval;
        int interval2 = interval1 + 1;
        int interval3 = interval1+(w/windowsize);
        int interval4 = interval3 + 1;

        // If the physics entity is outside the window, continue
        if (interval4 >= numOfWindows) {
            continue;
        }

        // If the physics entity spans two partitions in the x direction
        if (xInterval != xInterval2) {
            interval2 = interval1 + 1;

            // If the physics entity also spans two partitions in the y direction
            if (yInterval != yInterval2) {
                interval3 = interval1+(w/windowsize);
                interval4 = interval3 + 1;
                
                // Add the entity to all 4 partitions
                intervals[interval1].push_back(j);
                intervals[interval2].push_back(j);
                intervals[interval3].push_back(j);
                intervals[interval4].push_back(j);
            }

            // Add the entity to the two x partitions
            else {
                intervals[interval1].push_back(j);
                intervals[interval2].push_back(j);
            }
        }

        // If the entity is in only one x interval
        else {
            interval2 = interval1;

            // If the entity spans two y intervals
            if (yInterval != yInterval2) {
                interval3 = interval1 + (w/windowsize);

                intervals[interval1].push_back(j);
                intervals[interval3].push_back(j);

            }

            // If the entity is in only one interval
            else {
                intervals[interval1].push_back(j);
            }
        }
    }


    // Actual collision checking
    for (int i = 0; i < numOfWindows; i++) {
        if (intervals[i].size() > 0) {

            // Only perform collision checks on entities in the same interval
            for (int j = 0; j < intervals[i].size()-1; j++) {
                for (int k = j+1; k < intervals[i].size(); k++) {
                    if (entities[intervals[i][j]]->isColliding(*entities[intervals[i][k]])) {
                        // For changing the color of the entity based on its collisions, ignore for now
                        /*
                        float increment = 1.0/3000.0;

                        if (entities[intervals[i][j]]->colorPoint < 1.0-increment) {
                            entities[intervals[i][j]]->colorPoint += increment;
                            //entities[intervals[i][j]]->G += increment;
                            //entities[intervals[i][j]]->B += increment;
                        }

                        if (entities[intervals[i][k]]->colorPoint < 1.0-increment) {
                            entities[intervals[i][k]]->colorPoint += increment;
                            //entities[intervals[i][k]]->R += increment;
                            //entities[intervals[i][k]]->G += increment;
                            //entities[intervals[i][k]]->B += increment;
                        }*/

                        // Collisions are very rare in interstellar space, so collisions are not actually calculated right now
                        //entities[intervals[i][j]]->calculateCollision(*entities[intervals[i][k]]);
                    }
                }
            }

            // For real-time color updating of entities based on mass density
            totalMass = 0;

            for (int j = 0; j < intervals[i].size(); j++) {
                totalMass += entities[intervals[i][j]]->mass;
            }

            totalMasses[i] = totalMass;
        }
    }

    // Find maximum mass density, use this to normalize all densities between 0 and 1
    double maxMass = -1;

    for (int i = 0; i < numOfWindows; i++) {
        if (maxMass < totalMasses[i]) {
            maxMass = totalMasses[i];
        }
    }

    for (int i = 0; i < numOfWindows; i++) {
        for (int j = 0; j < intervals[i].size(); j++) {
            double multiplier = totalMasses[i]/maxMass;
            multiplier = coloringFunction(multiplier);
            if (multiplier < 0.015) {
                entities[intervals[i][j]]->R = 0*multiplier;
                entities[intervals[i][j]]->G = 0*multiplier;
                entities[intervals[i][j]]->B = 0*multiplier;
            }
            else {
                entities[intervals[i][j]]->R = 255*multiplier;
                entities[intervals[i][j]]->G = 255*multiplier;
                entities[intervals[i][j]]->B = 255*multiplier;
            }
        }
    }

    std::cout << "Collision-calc done!\n";

    // Using runge-kutta algorithm for gravity collision
    tree = rungeKutta(window, dTime, G, tree);

    // Return updated QuadTree
    return tree;
}

// Use QuadTree for barnes-Hut calculation
void allEntities::barnesHut(SDL_Window *window, float G, QuadTree* tree, node* b, entity* a) {
    // Widely accepted threshold value is 1, lower threshold value corresponds to higher accuracy but slower runtime
    float threshold = 0.25;//0.25;

    if (!b) {
        return;
    }
    
    else if (b->myEntity == a) {
        return;
    }

    // If the threshold value holds, then use the center of mass to calculate the force
    else if ((b->pt2[0]-b->pt1[0])/(Dist(b->COM[0], a->X, b->COM[1], a->Y)) < threshold) {
        entity entToCalc = entity(1, b->COM[0], b->COM[1], 0, 0, 0, 0, 0, 1, b->mass);
        a->calculateGravity(G, entToCalc);
    }

    // Recursively descend through the tree
    else {
        barnesHut(window, G, tree, b->pointers[0], a);
        barnesHut(window, G, tree, b->pointers[1], a);
        barnesHut(window, G, tree, b->pointers[2], a);
        barnesHut(window, G, tree, b->pointers[3], a);
    }
}

void allEntities::calculateGravity(float G, allEntities massLess) {
    for (int i = 0; i < massLess.size; i++) {
        for (int j = 0; j < size; j++) {
            massLess.entities[i]->calculateGravity(G, *entities[j]);
        }
    }
}

QuadTree* allEntities::rungeKutta(SDL_Window *window, double dTime, float G, QuadTree* tree) {
    // Used this resource to implement Runge-Kutta: http://spiff.rit.edu/richmond/nbody/OrbitRungeKutta4.pdf

    // k values for velocity and displacement are calculated separately, and will be represented as kiv or kir

    // Set baseline values, and find acceleration at starting position
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->prevX = entities[i]->X;
        entities[i]->prevY = entities[i]->Y;
        barnesHut(window, G, tree, tree->root, entities[i]);
    }

    // Setting k1 x and y values for displacement and velocity
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->kValsVelocity[0] = entities[i]->Xacceleration;
        entities[i]->kValsVelocity[1] = entities[i]->Yacceleration;

        entities[i]->kValsDisplacement[0] = entities[i]->Xvelocity;
        entities[i]->kValsDisplacement[1] = entities[i]->Yvelocity;
        
        // Reset acceleration before next iteration of Barnes-Hut
        entities[i]->Xacceleration = 0;
        entities[i]->Yacceleration = 0;
    }

    // Move all entities based on displacement k value, and recreate QuadTree
    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < entities.size(); i++) {
        entities[i]->X = entities[i]->prevX + entities[i]->kValsDisplacement[0]*dTime/2;
        entities[i]->Y = entities[i]->prevY + entities[i]->kValsDisplacement[1]*dTime/2;
    }

    for (int i = 0; i < entities.size(); i++) {
        tree->insert(entities[i], tree->root);
    }

    // Use new QuadTree to find forces after particles are moved
    for (int i = 0; i < entities.size(); i++) {
        barnesHut(window, G, tree, tree->root, entities[i]);
    }

    // Assign new k vals for velocity and displacement
    for (int i = 0; i < entities.size(); i++) {

        // kv2 = a(r + k1r*dt/2)
        entities[i]->kValsVelocity[2] = entities[i]->Xacceleration;
        entities[i]->kValsVelocity[3] = entities[i]->Yacceleration;

        // kr2 = vi + k1v*dt/2
        entities[i]->kValsDisplacement[2] = entities[i]->kValsDisplacement[0]+entities[i]->kValsVelocity[0]*dTime/2;
        entities[i]->kValsDisplacement[3] = entities[i]->kValsDisplacement[1]+entities[i]->kValsVelocity[1]*dTime/2;

        // Reset acceleration before next iteration of Barnes-Hut
        entities[i]->Xacceleration = 0;
        entities[i]->Yacceleration = 0;
    }

    // Move all entities based on displacement k value, and recreate QuadTree
    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < entities.size(); i++) {
        entities[i]->X = entities[i]->prevX + entities[i]->kValsDisplacement[2]*dTime/2;
        entities[i]->Y = entities[i]->prevY + entities[i]->kValsDisplacement[3]*dTime/2;
    }

    for (int i = 0; i < entities.size(); i++) {
        tree->insert(entities[i], tree->root);
    }

    // Use new QuadTree to find forces after particles are moved
    for (int i = 0; i < entities.size(); i++) {
        barnesHut(window, G, tree, tree->root, entities[i]);
    }

    // Assign new k vals for velocity and displacement
    for (int i = 0; i < entities.size(); i++) {
        // k3v = a(r + k2r*dt/2)
        entities[i]->kValsVelocity[4] = entities[i]->Xacceleration;
        entities[i]->kValsVelocity[5] = entities[i]->Yacceleration;

        // k3r = vi + k2v*dt/2
        entities[i]->kValsDisplacement[4] = entities[i]->kValsDisplacement[0]+entities[i]->kValsVelocity[2]*dTime/2;
        entities[i]->kValsDisplacement[5] = entities[i]->kValsDisplacement[1]+entities[i]->kValsVelocity[3]*dTime/2;

        // Reset acceleration before next iteration of Barnes-Hut
        entities[i]->Xacceleration = 0;
        entities[i]->Yacceleration = 0;
    }

    // Move all entities based on displacement k value, and recreate QuadTree
    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < entities.size(); i++) {
        entities[i]->X = entities[i]->prevX + entities[i]->kValsDisplacement[4]*dTime;
        entities[i]->Y = entities[i]->prevY + entities[i]->kValsDisplacement[5]*dTime;
    }

    for (int i = 0; i < entities.size(); i++) {
        tree->insert(entities[i], tree->root);
    }

    // Use new QuadTree to find forces after particles are moved
    for (int i = 0; i < entities.size(); i++) {
        barnesHut(window, G, tree, tree->root, entities[i]);
    }

    for (int i = 0; i < entities.size(); i++) {
        // k4v = a(r + k3r*dt)
        entities[i]->kValsVelocity[6] = entities[i]->Xacceleration;
        entities[i]->kValsVelocity[7] = entities[i]->Yacceleration;

        // k4r = ri + k3v*dt
        entities[i]->kValsDisplacement[6] = entities[i]->kValsDisplacement[0]+entities[i]->kValsVelocity[4]*dTime;
        entities[i]->kValsDisplacement[7] = entities[i]->kValsDisplacement[1]+entities[i]->kValsVelocity[5]*dTime;

        // Reset acceleration before next iteration of Barnes-Hut
        entities[i]->Xacceleration = 0;
        entities[i]->Yacceleration = 0;
    }

    // Calculation of final velocity and displacement updates
    for (int i = 0; i < entities.size(); i++) {
        double vIX = entities[i]->kValsDisplacement[0];
        double vIY = entities[i]->kValsDisplacement[1];

        double k1vX = entities[i]->kValsVelocity[0];
        double k1vY = entities[i]->kValsVelocity[1];

        double k2vX = entities[i]->kValsVelocity[2];
        double k2vY = entities[i]->kValsVelocity[3];

        double k3vX = entities[i]->kValsVelocity[4];
        double k3vY = entities[i]->kValsVelocity[5];

        double k4vX = entities[i]->kValsVelocity[6];
        double k4vY = entities[i]->kValsVelocity[7];


        double rIX = entities[i]->prevX;
        double rIY = entities[i]->prevY;

        double k1rX = entities[i]->kValsDisplacement[0];
        double k1rY = entities[i]->kValsDisplacement[1];

        double k2rX = entities[i]->kValsDisplacement[2];
        double k2rY = entities[i]->kValsDisplacement[3];

        double k3rX = entities[i]->kValsDisplacement[4];
        double k3rY = entities[i]->kValsDisplacement[5];

        double k4rX = entities[i]->kValsDisplacement[6];
        double k4rY = entities[i]->kValsDisplacement[7];

        // velocity = vi + (k1v+2*k2v+2*k3v+k4v)*dTime/6
        entities[i]->Xvelocity = vIX + ((dTime/6)*(k1vX + 2*k2vX + 2*k3vX + k4vX));
        entities[i]->Yvelocity = vIY + ((dTime/6)*(k1vY + 2*k2vY + 2*k3vY + k4vY));
        
        // displacement = ri + (k1r+2*k2r+2*k3r+k4r)*dTime/6
        entities[i]->X = rIX + ((dTime/6)*(k1rX + 2*k2rX + 2*k3rX + k4rX));
        entities[i]->Y = rIY + ((dTime/6)*(k1rY + 2*k2rY + 2*k3rY + k4rY));
    }

    // Recreate final QuadTree and return it
    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < entities.size(); i++) {
        tree->insert(entities[i], tree->root);
    }

    return tree;
}
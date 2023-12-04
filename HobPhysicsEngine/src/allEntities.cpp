#include "allEntities.h"

bool compare(entity* a, entity* b) {
    return a->X<b->X;
}

bool compare2(entity* a, entity* b) {
    return a->Y*1360+a->X < b->Y*1360+b->X;
}

allEntities::allEntities(unsigned char *gradData) {
    size = 0;
    data = gradData;
}
void allEntities::addRect(entity* rect) {
    rects.push_back(rect);
    size += 1;
}

entity* allEntities::removeRect(int index, entity* trackedEntity) {
    entity* tmp;
    tmp = rects[index];
    if (trackedEntity == tmp) {
        trackedEntity = nullptr;
    }
    rects.erase(rects.begin()+index);
    delete tmp;
    size -= 1;
    return trackedEntity;
}

void allEntities::render(SDL_Renderer* renderer, SDL_Window *window) {
    for (int i = 0; i < size; i++) {
        SDL_SetRenderDrawColor(renderer, rects[i]->R, rects[i]->G, rects[i]->B, 255);
        rects[i]->render(renderer);
    }
}

QuadTree* allEntities::update2(SDL_Window *window, entity* trackedRect, double dTime, float G, QuadTree* tree) {
    for (int i = 0; i < size; i++) {
        //rects[i]->bounceWall(window);
        rects[i]->reset();
    }
    
    for (int i = 0; i < size-1; i++) {
        for (int j = i+1; j < size; j++) {
            if (rects[i]->isColliding(*rects[j])) {
                rects[i]->calculateCollision(*rects[j]);
            }
            rects[i]->calculateGravity(G, *rects[j]);
        }
    }

    /*for (int i = 0; i < size; i++) {
        barnesHut(window, dTime, G, tree, tree->root, rects[i]);
    }*/

    for (int i = 0; i < size; i++) {
       rects[i]->updatePos(dTime);
    }

    //delete tree;
}

QuadTree* allEntities::update3(SDL_Window *window, entity* trackedRect, double dTime, int windowsize, float G, QuadTree* tree) {//, int width, int height) {//, allEntities massFull) {
    std::cout << "Entered update!\n";
    
    int* num = new int;
    *num = 0;
    
    float windowSize = windowsize;

    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    int startIndex = 0;
    int endIndex = 0;
    float totalMass = 0;

    int numOfWindows = ((w/windowsize))*((h/windowsize)+2);
    std::vector <int> intervals[numOfWindows];

    for (int j = 0; j < rects.size(); j++) {
        int smallX = rects[j]->X - rects[j]->Rad;
        int smallY = rects[j]->Y - rects[j]->Rad;
        int largeX = rects[j]->X + rects[j]->Rad;
        int largeY = rects[j]->Y + rects[j]->Rad;

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

        int xInterval = smallX/windowsize;
        int xInterval2 = largeX/windowsize;
        int yInterval = smallY/windowsize;
        int yInterval2 = largeY/windowsize;

        int interval1 = yInterval*(w/windowsize) + xInterval;
        int interval2 = interval1 + 1;
        int interval3 = interval1+(w/windowsize);
        int interval4 = interval3 + 1;

        if (interval4 >= numOfWindows) {
            continue;
        }

        if (xInterval != xInterval2) {
            interval2 = interval1 + 1;
            if (yInterval != yInterval2) {
                interval3 = interval1+(w/windowsize);
                interval4 = interval3 + 1;
                
                intervals[interval1].push_back(j);
                intervals[interval2].push_back(j);
                intervals[interval3].push_back(j);
                intervals[interval4].push_back(j);
            }
            else {
                intervals[interval1].push_back(j);
                intervals[interval2].push_back(j);
            }
        }
        else {
            interval2 = interval1;
            if (yInterval != yInterval2) {
                interval3 = interval1 + (w/windowsize);

                intervals[interval1].push_back(j);
                intervals[interval3].push_back(j);

            }
            else {
                intervals[interval1].push_back(j);
            }
        }
    }

    double totalMasses[numOfWindows];

    for (int i = 0; i < numOfWindows; i++) {
        if (intervals[i].size() > 0) {
            for (int j = 0; j < intervals[i].size()-1; j++) {
                for (int k = j+1; k < intervals[i].size(); k++) {
                    if (rects[intervals[i][j]]->isColliding(*rects[intervals[i][k]])) {
                        rects[intervals[i][j]]->hasCollided = true;
                        rects[intervals[i][k]]->hasCollided = true;

                        float increment = 1.0/3000.0;

                        if (rects[intervals[i][j]]->colorPoint < 1.0-increment) {
                            rects[intervals[i][j]]->colorPoint += increment;
                            //rects[intervals[i][j]]->G += increment;
                            //rects[intervals[i][j]]->B += increment;
                        }

                        if (rects[intervals[i][k]]->colorPoint < 1.0-increment) {
                            rects[intervals[i][k]]->colorPoint += increment;
                            //rects[intervals[i][k]]->R += increment;
                            //rects[intervals[i][k]]->G += increment;
                            //rects[intervals[i][k]]->B += increment;
                        }

                        //rects[intervals[i][j]]->calculateCollision(*rects[intervals[i][k]]);
                    }
                }
                /*for (int horizontalOffset = -1; horizontalOffset < 2; horizontalOffset++) {
                    if (i % (w/windowsize) == 0) {
                        if (horizontalOffset == -1) {
                            continue;
                        }
                    }
                    if ((i+1) % (w/windowsize) == 0) {
                        if (horizontalOffset == 1) {
                            continue;
                        }
                    }

                    for (int verticalOffset = -1; verticalOffset < 2; verticalOffset++) {
                        if (i < (w/windowsize)) {
                            if (verticalOffset == -1) {
                                continue;
                            }
                        }
                        if (i > numOfWindows-(w/windowsize)) {
                            if (verticalOffset == 1) {
                                continue;
                            }
                        }
                        if (horizontalOffset == 0 && verticalOffset == 0) {
                            continue;
                        }
                        for (int mm = 0; mm < intervals[i+(verticalOffset*(w/windowsize)) + horizontalOffset].size(); mm++) {
                    //std::cout << k << "\n";      
                            if (rects[intervals[i][j]]->isColliding(*rects[intervals[i+(verticalOffset*(w/windowsize)) + horizontalOffset][mm]])) {
                                //std::cout << "Collision!\n";
                                //rects[intervals[i][j]]->calculateCollision(*rects[intervals[i+(verticalOffset*(w/windowsize)) + horizontalOffset][mm]]);
                            }
                        }
                    }
                }*/
            }

            totalMass = 0;

            for (int j = 0; j < intervals[i].size(); j++) {
                totalMass += rects[intervals[i][j]]->mass;
            }

            totalMasses[i] = totalMass;
        }
    }

    double maxMass = -1;

    for (int i = 0; i < numOfWindows; i++) {
        if (maxMass < totalMasses[i]) {
            maxMass = totalMasses[i];
        }
    }

    for (int i = 0; i < numOfWindows; i++) {
        for (int j = 0; j < intervals[i].size(); j++) {
            double multiplier = totalMasses[i]/maxMass;
            std::cout << multiplier << "\n";
            rects[intervals[i][j]]->R = 255*multiplier;
            rects[intervals[i][j]]->G = 255*multiplier;
            rects[intervals[i][j]]->B = 255*multiplier;
        }
    }

    std::cout << "Collision-calc done!\n";

    
    /*for (int i = 0; i < size-1; i++) {
        for (int j = i+1; j < size; j++) {
            rects[i]->calculateGravity(G, *rects[j]);
        }
    }*/



    tree = rungeKutta(window, dTime, G, tree);

    return tree;
}



entity* allEntities::update(SDL_Window *window, entity* trackedRect, double dTime) {
    for (int i = 0; i < size; i++) {
        rects[i]->bounceWall(window);
    }

    std::sort(rects.begin(), rects.end(), compare);

    int startIndex = 0;
    int endIndex = 0;
    std::vector<int> intervals;

    for (int i = 0; i < size-1; i++) {
        if ((rects[i+1]->X - rects[i+1]->Rad) <= (rects[i]->X+rects[i]->Rad)) {
            if (endIndex > startIndex) {
                endIndex++;
            }
            else {
                startIndex = i;
                endIndex = i+1;
            }
            if (i+1 == size-1) {
                intervals.push_back(startIndex);
                intervals.push_back(endIndex);
            }
        }
        else {
            if (endIndex > startIndex) {
                intervals.push_back(startIndex);
                intervals.push_back(endIndex);
                startIndex = endIndex;
            }
        }
    }

    int numofCollisions = 0;

    for (int k = 0; k < intervals.size()/2; k++) {
        for (int j = intervals[2*k]; j <= intervals[2*k+1]-1; j++) {
            for (int i = j+1; i <= intervals[2*k+1]; i++) {
                numofCollisions++;
                if (i == j) {
                    continue;
                }
                if (rects[i]->isColliding(*rects[j])) {
                    rects[i]->calculateCollision(*rects[j]);
                }
            }
        }
    }
    
    int expected = 0;
    if (rects.size() % 2 == 1) {
        expected = ((rects.size()-1)/2)*(rects.size()+1) + ((rects.size()+1)/2);
    }
    else {
        expected = ((rects.size())/2)*(rects.size()+1);
    }

    std::cout << "Expected: " << expected << "  Actual: " << numofCollisions << "\n";

    for (int i = 0; i < size; i++) {
        rects[i]->bounceWall(window);
    }

    for (int i = 0; i < size; i++) {
       rects[i]->updatePos(dTime);
    }
}

void allEntities::barnesHut(SDL_Window *window, float G, QuadTree* tree, node* b, entity* a) {
    float threshold = 0.25;//0.25;

    if (!b) {
        return;
    }
    
    else if (b->myEntity == a) {
        return;
    }

    else if ((b->pt2[0]-b->pt1[0])/(Dist(b->COM[0], a->X, b->COM[1], a->Y)) < threshold) {
        entity entToCalc = entity(1, b->COM[0], b->COM[1], 0, 0, 0, 0, 0, 1, b->mass);
        a->calculateGravity(G, entToCalc);
    }

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
            massLess.rects[i]->calculateGravity(G, *rects[j]);
        }
    }
}

QuadTree* allEntities::rungeKutta(SDL_Window *window, double dTime, float G, QuadTree* tree) {
    for (int i = 0; i < rects.size(); i++) {
        rects[i]->prevX = rects[i]->X;
        rects[i]->prevY = rects[i]->Y;
        barnesHut(window, G, tree, tree->root, rects[i]);
    }

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->kValsVelocity[0] = rects[i]->Xacceleration;
        rects[i]->kValsVelocity[1] = rects[i]->Yacceleration;

        rects[i]->kValsDisplacement[0] = rects[i]->Xvelocity;
        rects[i]->kValsDisplacement[1] = rects[i]->Yvelocity;

        rects[i]->Xacceleration = 0;
        rects[i]->Yacceleration = 0;
    }

    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->X = rects[i]->prevX + rects[i]->kValsDisplacement[0]*dTime/2;
        rects[i]->Y = rects[i]->prevY + rects[i]->kValsDisplacement[1]*dTime/2;
    }

    for (int i = 0; i < rects.size(); i++) {
        tree->insert(rects[i], tree->root);
    }

    for (int i = 0; i < rects.size(); i++) {
        barnesHut(window, G, tree, tree->root, rects[i]);
    }

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->kValsVelocity[2] = rects[i]->Xacceleration;
        rects[i]->kValsVelocity[3] = rects[i]->Yacceleration;

        rects[i]->kValsDisplacement[2] = rects[i]->kValsDisplacement[0]+rects[i]->kValsVelocity[0]*dTime/2;
        rects[i]->kValsDisplacement[3] = rects[i]->kValsDisplacement[1]+rects[i]->kValsVelocity[1]*dTime/2;

        rects[i]->Xacceleration = 0;
        rects[i]->Yacceleration = 0;
    }

    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->X = rects[i]->prevX + rects[i]->kValsDisplacement[2]*dTime/2;
        rects[i]->Y = rects[i]->prevY + rects[i]->kValsDisplacement[3]*dTime/2;
    }

    for (int i = 0; i < rects.size(); i++) {
        tree->insert(rects[i], tree->root);
    }

    for (int i = 0; i < rects.size(); i++) {
        barnesHut(window, G, tree, tree->root, rects[i]);
    }

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->kValsVelocity[4] = rects[i]->Xacceleration;
        rects[i]->kValsVelocity[5] = rects[i]->Yacceleration;

        rects[i]->kValsDisplacement[4] = rects[i]->kValsDisplacement[0]+rects[i]->kValsVelocity[2]*dTime/2;
        rects[i]->kValsDisplacement[5] = rects[i]->kValsDisplacement[1]+rects[i]->kValsVelocity[3]*dTime/2;

        rects[i]->Xacceleration = 0;
        rects[i]->Yacceleration = 0;
    }

    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->X = rects[i]->prevX + rects[i]->kValsDisplacement[4]*dTime;
        rects[i]->Y = rects[i]->prevY + rects[i]->kValsDisplacement[5]*dTime;
    }

    for (int i = 0; i < rects.size(); i++) {
        tree->insert(rects[i], tree->root);
    }

    for (int i = 0; i < rects.size(); i++) {
        barnesHut(window, G, tree, tree->root, rects[i]);
    }

    for (int i = 0; i < rects.size(); i++) {
        rects[i]->kValsVelocity[6] = rects[i]->Xacceleration;
        rects[i]->kValsVelocity[7] = rects[i]->Yacceleration;

        rects[i]->kValsDisplacement[6] = rects[i]->kValsDisplacement[0]+rects[i]->kValsVelocity[4]*dTime;
        rects[i]->kValsDisplacement[7] = rects[i]->kValsDisplacement[1]+rects[i]->kValsVelocity[5]*dTime;

        rects[i]->Xacceleration = 0;
        rects[i]->Yacceleration = 0;
    }

    for (int i = 0; i < rects.size(); i++) {
        double vIX = rects[i]->kValsDisplacement[0];
        double vIY = rects[i]->kValsDisplacement[1];

        double k1vX = rects[i]->kValsVelocity[0];
        double k1vY = rects[i]->kValsVelocity[1];

        double k2vX = rects[i]->kValsVelocity[2];
        double k2vY = rects[i]->kValsVelocity[3];

        double k3vX = rects[i]->kValsVelocity[4];
        double k3vY = rects[i]->kValsVelocity[5];

        double k4vX = rects[i]->kValsVelocity[6];
        double k4vY = rects[i]->kValsVelocity[7];


        double rIX = rects[i]->prevX;
        double rIY = rects[i]->prevY;

        double k1rX = rects[i]->kValsDisplacement[0];
        double k1rY = rects[i]->kValsDisplacement[1];

        double k2rX = rects[i]->kValsDisplacement[2];
        double k2rY = rects[i]->kValsDisplacement[3];

        double k3rX = rects[i]->kValsDisplacement[4];
        double k3rY = rects[i]->kValsDisplacement[5];

        double k4rX = rects[i]->kValsDisplacement[6];
        double k4rY = rects[i]->kValsDisplacement[7];

        rects[i]->Xvelocity = vIX + ((dTime/6)*(k1vX + 2*k2vX + 2*k3vX + k4vX));
        rects[i]->Yvelocity = vIY + ((dTime/6)*(k1vY + 2*k2vY + 2*k3vY + k4vY));

        rects[i]->X = rIX + ((dTime/6)*(k1rX + 2*k2rX + 2*k3rX + k4rX));
        rects[i]->Y = rIY + ((dTime/6)*(k1rY + 2*k2rY + 2*k3rY + k4rY));
    }

    tree->deallocate(tree->root);
    tree = new QuadTree(window);

    for (int i = 0; i < rects.size(); i++) {
        tree->insert(rects[i], tree->root);
    }

    return tree;
}
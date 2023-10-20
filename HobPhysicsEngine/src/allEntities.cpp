#include "allEntities.h"

bool compare(entity* a, entity* b) {
    return a->X<b->X;
}

bool compare2(entity* a, entity* b) {
    return a->Y*1360+a->X < b->Y*1360+b->X;
}

allEntities::allEntities() {
    size = 0;
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
        rects[i]->bounceWall(window);
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

QuadTree* allEntities::update3(SDL_Window *window, entity* trackedRect, double dTime, int windowsize, float G, QuadTree* tree) {
    float windowSize = windowsize;
    for (int i = 0; i < size; i++) {
        rects[i]->bounceWall(window);
    }

    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    int startIndex = 0;
    int endIndex = 0;

    int numOfWindows = ((w/windowsize))*((h/windowsize)+2);
    std::vector <int> intervals[numOfWindows];

    //std::cout << numOfWindows << "\n";

    //std::cout << "Size: " << rects.size() << "\n";

    for (int j = 0; j < rects.size(); j++) {
        //std::cout << j << "\n";
        int smallX = rects[j]->X - rects[j]->Rad;
        int smallY = rects[j]->Y - rects[j]->Rad;
        int largeX = rects[j]->X + rects[j]->Rad;
        int largeY = rects[j]->Y + rects[j]->Rad;

        if (smallX < 0) {
            smallX = 0;
        }

        if (smallY < 0) {
            smallY = 0;
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
                //std::cout << "Yes1 " << interval1 << " " << numOfWindows << " " << rects[j]->X << " " << rects[j]->Y << " " << rects[j]->id << " " << rects[j]->Yvelocity << "\n";
                interval3 = interval1+(w/windowsize);
                interval4 = interval3 + 1;
                
                //std::cout << interval1 << "\n";
                intervals[interval1].push_back(j);
                //std::cout << interval2 << "\n";
                intervals[interval2].push_back(j);
                //std::cout << interval3 << "\n";
                intervals[interval3].push_back(j);
                //std::cout << interval4 << "\n";
                intervals[interval4].push_back(j);
                //std::cout << "Yes1Part2:) :) :) :) :)\n";
            }
            else {
                //std::cout << "Yes2\n";
                interval3 = interval1;
                interval4 = interval1;
                
                intervals[interval1].push_back(j);
                intervals[interval2].push_back(j);
            }
        }
        else {
            interval2 = interval1;
            if (yInterval != yInterval2) {
                //std::cout << "Yes3\n";
                interval3 = interval1 + (w/windowsize);
                interval4 = interval3;

                intervals[interval1].push_back(j);
                intervals[interval3].push_back(j);

            }
            else {
                //std::cout << "Yes4\n";
                interval3 = interval1;
                interval4 = interval1;

                intervals[interval1].push_back(j);
            }
        }
    }

    for (int i = 0; i < numOfWindows; i++) {
        // top row
        if (intervals[i].size() > 0) {
            for (int j = 0; j < intervals[i].size()-1; j++) {
                for (int k = j+1; k < intervals[i].size(); k++) {
                    //std::cout << k << "\n";      
                    if (rects[intervals[i][j]]->isColliding(*rects[intervals[i][k]])) {
                        //std::cout << "Collision!\n";
                        rects[intervals[i][j]]->calculateCollision(*rects[intervals[i][k]]);
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
                                std::cout << "Collision!\n";
                                rects[intervals[i][j]]->calculateCollision(*rects[intervals[i+(verticalOffset*(w/windowsize)) + horizontalOffset][mm]]);
                            }
                        }
                    }
                }*/
            }
        }
    }

    //std::cout << "Reached here!\n";

    /*for (int i = 0; i < size-1; i++) {
        for (int j = i+1; j < size; j++) {
            rects[i]->calculateGravity(G, *rects[j]);
        }
    }*/

    for (int i = 0; i < size; i++) {
        barnesHut(window, dTime, G, tree, tree->root, rects[i]);
    }
    
    for (int i = 0; i < size; i++) {
       rects[i]->updatePos(dTime);
       //std::cout << i << "\n";
    }

    //std::cout << "Reachedhere!\n";

    tree = new QuadTree(window);

    for (int i = 0; i < size; i++) {
        tree->insert(rects[i], tree->root);
        //std::cout << "Reachedhere!" << i << "\n";
    }

    //std::cout << "Reachedhere22!\n";

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

void allEntities::barnesHut(SDL_Window *window, double dTime, float G, QuadTree* tree, node* b, entity* a) {
    float threshold = 0.25;

    if (b == nullptr) {
        return;
    }
    
    else if (b->myEntity == a) {
        return;
    }

    else if ((b->pt2[0]-b->pt1[0])/(Dist(b->COM[0], a->X, b->COM[1], a->Y)) < threshold) {
        float force = (G*b->mass*a->mass)/(Dist(b->COM[0], a->X, b->COM[1], a->Y)*Dist(b->COM[0], a->X, b->COM[1], a->Y));
        
        float vecX = b->COM[0]-a->X;
        float vecY = b->COM[1]-a->Y;

        if (vecX == 0) {
            vecX = 0.001;
        }

        float cosFactor = 1/(sqrt((vecY/vecX)*(vecY/vecX)+1));
        float sinFactor = fabs(vecY/vecX)/(sqrt((vecY/vecX)*(vecY/vecX)+1));

        float forceX = cosFactor*force;
        float forceY = sinFactor*force;

        float Xacceleration = (fabs(forceX/a->mass) * (int)((vecX/fabs(vecX))));
        float Yacceleration = (fabs(forceY/a->mass) * (int)((vecY/fabs(vecY))));

        a->incrementAccelerations(Xacceleration, Yacceleration);
    }

    else {
        barnesHut(window, dTime, G, tree, b->pointers[0], a);
        barnesHut(window, dTime, G, tree, b->pointers[1], a);
        barnesHut(window, dTime, G, tree, b->pointers[2], a);
        barnesHut(window, dTime, G, tree, b->pointers[3], a);
    }



    //std::cout << "mass: " << rect.mass << " " << mass << "\n";
}
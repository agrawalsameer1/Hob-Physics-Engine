#include "allEntities.h"

allEntities::allEntities() {
    size = 0;
}
void allEntities::addRect(rectEntity* rect) {
    rects.push_back(rect);
    size += 1;
}

rectEntity* allEntities::removeRect(int index, rectEntity* trackedEntity) {
    rectEntity* tmp;
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
        rects[i]->render(renderer);
    }
}

rectEntity* allEntities::update(SDL_Window *window, rectEntity* trackedRect, double dTime) {
    for (int i = 0; i < size; i++) {
        /*if (rects[i]->checkBounds(window)) {
            trackedRect = removeRect(i, trackedRect);
        }*/
        rects[i]->bounceWall(window);
    }
    
    for (int i = 0; i < size-1; i++) {
        for (int j = i+1; j < size; j++) {
            if (rects[i]->isColliding(*rects[j])) {
                rects[i]->calculateCollision(*rects[j]);
            }
        }
    }

    for (int i = 0; i < size; i++) {
       rects[i]->updatePos(dTime);
    }
    return trackedRect;
}
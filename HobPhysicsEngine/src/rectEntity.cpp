#include "rectEntity.h"

rectEntity::rectEntity() {
    rect = new SDL_Rect();
}

rectEntity::rectEntity(double w, double h, double x, double y, int r, int g, int b) {
    X = x;
    Y = y;
    W = w;
    H = h;
    R = r;
    G = g;
    B = b;
    rect = new SDL_Rect();
    rect->h = h;
    rect->w = w;
    rect->x = x;
    rect->y = y;
    mass = w*h;
    char* content = strdup(("Mass of selected rect: " + std::to_string(mass)).c_str());
    char* path = strdup("/Users/sameeragrawal/HobPhysicsEngine/src/font.ttf");
    SDL_Color White = {255, 255, 255};

    massText = textBox(content, path, 12, White, 20, 20, 200, 25);
    
    /*srand((unsigned) time(NULL));
    rand();
    rand();
    rand();*/
    Xvelocity = (rand()%2)%2 == 0 ? (rand()%30)+10 : -((rand()%30)+10);
    /*rand();
    rand();
    rand();*/
    Yvelocity = (rand()%2)%2 == 0 ? (rand()%30)+10 : -((rand()%30)+10);

    char* content2 = strdup(("Velocity of selected rect: " + std::to_string(calculateVelocity()) + " at " + std::to_string(calculateAngle()) + " degrees").c_str());
    velocityText = textBox(content2, path, 12, White, 20, 40, 200, 25);
}

void rectEntity::updatePos(double dTime) {
    X += Xvelocity*(dTime);
    Y += Yvelocity*(dTime);
    rect->x = X;
    rect->y = Y;
}

bool rectEntity::checkBounds(SDL_Window *window) {
    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    if ((X+W) < 0 || X > w || ((Y+H) < 0 || Y > h)) {
        return true;
    }

    else {
        return false;
    }
}

void rectEntity::bounceWall(SDL_Window *window) {
    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    if ((X) < 0 || X+W > w) {
        Xvelocity = -1*Xvelocity;
    }

    else if (((Y) < 0 || Y+H > h)) {
        Yvelocity = -1*Yvelocity;
    }

    velocityText.content = strdup(("Velocity of selected rect: " + std::to_string(calculateVelocity()) + " at " + std::to_string(calculateAngle()) + " degrees").c_str());
}

bool rectEntity::isColliding(rectEntity rectangle) {
    if (rectangle.X < X) {
        if (rectangle.X+rectangle.W >= X) {
            if (rectangle.Y < Y) {
                if (rectangle.Y+H >= Y) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                if (Y+H >= rectangle.Y) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    if (rectangle.X > X) {
        if (X+W >= rectangle.X) {
            if (rectangle.Y < Y) {
                if (rectangle.Y+H >= Y) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                if (Y+H >= rectangle.Y) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    return true;
}

void rectEntity::calculateCollision(rectEntity &rect) {
    double mass2 = rect.mass;

    double Xvelocity2 = rect.Xvelocity;
    double v1 = (mass-mass2)/(mass+mass2) * Xvelocity + ((2*mass2)/(mass+mass2)) * Xvelocity2;
    double v2 = (mass2-mass)/(mass+mass2) * Xvelocity2 + ((2*mass)/(mass+mass2)) * Xvelocity;
    Xvelocity = v1;
    velocityText.content = strdup(("Velocity of selected rect: " + std::to_string(calculateVelocity()) + " at " + std::to_string(calculateAngle()) + " degrees").c_str());
    rect.Xvelocity = v2;

    double Yvelocity2 = rect.Yvelocity;
    double v3 = (mass-mass2)/(mass+mass2) * Yvelocity + ((2*mass2)/(mass+mass2)) * Yvelocity2;
    double v4 = (mass2-mass)/(mass+mass2) * Yvelocity2 + ((2*mass)/(mass+mass2)) * Yvelocity;
    Yvelocity = v3;
    rect.Yvelocity = v4;

    rect.velocityText.content = strdup(("Velocity of selected rect: " + std::to_string(rect.calculateVelocity()) + " at " + std::to_string(rect.calculateAngle()) + " degrees").c_str());
}

void rectEntity::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderFillRect(renderer, rect);
}

double rectEntity::calculateVelocity() {
    return sqrt(Xvelocity*Xvelocity + Yvelocity*Yvelocity);
}

double rectEntity::calculateAngle() {
    return (atan(Yvelocity/Xvelocity)*180/PI);
}
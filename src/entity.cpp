#include "entity.h"

float Dist(float x1, float x2, float y1, float y2) {
    return (sqrt(((x2 - x1)*(x2-x1) +(y2 - y1)*(y2-y1)))); 
}

void draw_circle(SDL_Renderer *renderer, float x, float y, int radius)
{
    for (float w = 0; w < radius * 2 * 1; w++)
    {
        for (float h = 0; h < radius * 2 * 1; h++)
        {
            float dx = radius - w/3; // horizontal offset
            float dy = radius - h/3; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPointF(renderer, x + dx, y + dy);
            }
        }
    }
}

entity::entity() {
    ;
}

entity::entity(double rad, double x, double y, int r, int g, int b, int ID) {
    X = x;
    Y = y;
    Rad = rad;
    R = r;
    G = g;
    B = b;

    mass = Rad*Rad;
    
    Xvelocity = (rand()%2)%2 == 0 ? (rand()%10)+20 : -((rand()%40)+10);
    Yvelocity = (rand()%2)%2 == 0 ? (rand()%10)+20 : -((rand()%40)+10);
    Xacceleration = 0;
    Yacceleration = 0;
    id = ID;
}

entity::entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int ID) {
    X = x;
    Y = y;
    Rad = rad;
    R = r;
    G = g;
    B = b;
    Xvelocity = vx;
    Yvelocity = vy;
    mass = Rad*rad;
    Xacceleration = 0;
    Yacceleration = 0;
    id = ID;
}

entity::entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int ID, double m) {
    X = x;
    Y = y;
    Rad = rad;
    R = r;
    G = g;
    B = b;
    Xvelocity = vx;
    Yvelocity = vy;
    mass = m;
    Xacceleration = 0;
    Yacceleration = 0;
    id = ID;
}

entity::entity(double rad, double x, double y, int r, int g, int b, double vx, double vy, int ID, double m, bool attract) {
    X = x;
    Y = y;
    Rad = rad;
    R = r;
    G = g;
    B = b;
    Xvelocity = vx;
    Yvelocity = vy;
    mass = m;
    Xacceleration = 0;
    Yacceleration = 0;
    id = ID;
    attractor = attract;
}

void entity::updatePos(double dTime) {
    if (!attractor) {
        X += (Xvelocity*(dTime) + 1/2*Xacceleration*dTime*dTime);
        Y += (Yvelocity*(dTime) + 1/2*Yacceleration*dTime*dTime);

        Xdisplacement = fabs(Xvelocity*(dTime) + 1/2*Xacceleration*dTime*dTime);
        Ydisplacement = fabs(Yvelocity*(dTime) + 1/2*Yacceleration*dTime*dTime);
        //std::cout << id << " - velocity: " << Xvelocity << " " << Yvelocity << "\n";

        Xvelocity += Xacceleration*dTime;
        Yvelocity += Yacceleration*dTime;
    }
}

bool entity::checkBounds(SDL_Window *window) {
    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    if ((X+Rad) < 0 || X > w || ((Y+Rad) < 0 || Y > h)) {
        return true;
    }

    else {
        return false;
    }
}

void entity::bounceWall(SDL_Window *window) {
    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    if ((X) < Rad || X+Rad > w) {
        Xvelocity = -1*Xvelocity;
    }

    if (((Y) < Rad || Y+Rad > h)) {
        Yvelocity = -1*Yvelocity;
    }
}

bool entity::isColliding(entity circle) {
    return (Dist(circle.X, X, circle.Y, Y) < (circle.Rad + Rad));
}

void entity::calculateCollision(entity &rect) {
   float damping = 1.0;

    double mass2 = rect.mass;

    double Xvelocity2 = rect.Xvelocity;
    double v1 = (mass-mass2)/(mass+mass2) * Xvelocity + ((2*mass2)/(mass+mass2)) * Xvelocity2;
    double v2 = (mass2-mass)/(mass+mass2) * Xvelocity2 + ((2*mass)/(mass+mass2)) * Xvelocity;
    Xvelocity = damping*v1;
    rect.Xvelocity = damping*v2;

    double Yvelocity2 = rect.Yvelocity;
    double v3 = (mass-mass2)/(mass+mass2) * Yvelocity + ((2*mass2)/(mass+mass2)) * Yvelocity2;
    double v4 = (mass2-mass)/(mass+mass2) * Yvelocity2 + ((2*mass)/(mass+mass2)) * Yvelocity;
    Yvelocity = damping*v3;
    rect.Yvelocity = damping*v4;
}

void entity::calculateGravity(float G, entity& rect) {
    if (rect.id == id) {
        return;
    }
    
    float softening = 0.3;
    float force = 0;

    if (mass == 0) {
        force = (G*1*rect.mass)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y) + softening*softening);
    }

    if (rect.mass == 0) {
        force = (G*mass*1)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y) + softening*softening);
    }

    if (mass > 0 && rect.mass > 0) {
        force = (G*mass*rect.mass)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y) + softening*softening);
    }
    
    
    float vecX = rect.X-X;
    float vecY = rect.Y-Y;

    if (vecX == 0) {
        vecX = 0.001;
    }

    float cosFactor = 1/(sqrt((vecY/vecX)*(vecY/vecX)+1));
    float sinFactor = fabs(vecY/vecX)/(sqrt((vecY/vecX)*(vecY/vecX)+1));

    float forceX = cosFactor*force;
    float forceY = sinFactor*force;

    if (mass == 0) {
        Xacceleration += (fabs(forceX/1) * (int)((vecX/fabs(vecX))));
        Yacceleration += (fabs(forceY/1) * (int)((vecY/fabs(vecY))));

        rect.Xacceleration += 0;
        rect.Yacceleration += 0;
    }
    
    if (rect.mass == 0) {
        rect.Xacceleration += (fabs(forceX/1) * (int)(((-vecX)/fabs(vecX))));
        rect.Yacceleration += (fabs(forceY/1) * (int)(((-vecY)/fabs(vecY))));

        Xacceleration += 0;
        Yacceleration += 0;
    }

    if (rect.mass > 0 && mass > 0) {
        rect.Xacceleration += (fabs(forceX/rect.mass) * (int)(((-vecX)/fabs(vecX))));
        rect.Yacceleration += (fabs(forceY/rect.mass) * (int)(((-vecY)/fabs(vecY))));
        Xacceleration += (fabs(forceX/mass) * (int)((vecX/fabs(vecX))));
        Yacceleration += (fabs(forceY/mass) * (int)((vecY/fabs(vecY))));
    }
}

void entity::incrementAccelerations(float xacceleration, float yacceleration) {
    Xacceleration += xacceleration;
    Yacceleration += yacceleration;
}

void entity::reset() {
    Xacceleration = 0;
    Yacceleration = 0;   
}

void entity::render(SDL_Renderer* renderer) {
    draw_circle(renderer, X, Y, Rad);
}
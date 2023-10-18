#include "entity.h"

float Dist(float x1, float x2, float y1, float y2) {
    return (sqrt(((x2 - x1)*(x2-x1) +(y2 - y1)*(y2-y1)))); 
}

void draw_circle(SDL_Renderer *renderer, float x, float y, int radius)
{
    for (float w = 0; w < radius * 2; w++)
    {
        for (float h = 0; h < radius * 2; h++)
        {
            float dx = radius - w; // horizontal offset
            float dy = radius - h; // vertical offset
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

entity::entity(double rad, double x, double y, int r, int g, int b, int vx, int vy, int ID) {
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

void entity::updatePos(double dTime) {
    X += (Xvelocity*(dTime) + 1/2*Xacceleration*dTime*dTime);
    Y += (Yvelocity*(dTime) + 1/2*Yacceleration*dTime*dTime);

    //std::cout << id << " - velocity: " << Xvelocity << " " << Yvelocity << "\n";

    Xvelocity += Xacceleration*dTime;
    Yvelocity += Yacceleration*dTime;
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

    else if (((Y) < Rad || Y+Rad > h)) {
        Yvelocity = -1*Yvelocity;
    }
}

bool entity::isColliding(entity circle) {
    if (Dist(circle.X, X, circle.Y, Y) < (circle.Rad + Rad)) {
        return true;
    }
    else {
        return false;
    }
}

void entity::calculateCollision(entity &rect) {

    /*
    float vecX = rect.X-X;
    float vecY = rect.Y-Y;

    float cosFactor = 1/(sqrt((vecY/vecX)*(vecY/vecX)+1));
    float sinFactor = fabs(vecY/vecX)/(sqrt((vecY/vecX)*(vecY/vecX)+1));

    float diffX = cosFactor*((rect.Rad+Rad)-sqrt(vecX*vecX+vecY*vecY));
    float diffY = sinFactor*((rect.Rad+Rad)-sqrt(vecX*vecX+vecY*vecY));

    rect.X += (rect.mass/(rect.mass + mass))*(diffX)*0.75;
    X -= (mass/(rect.mass + mass))*(diffX)*0.75;
    rect.Y += (rect.mass/(rect.mass + mass))*(diffY)*0.75;
    Y -= (mass/(rect.mass + mass))*(diffY)*0.75;
    */

    double mass2 = rect.mass;

    double Xvelocity2 = rect.Xvelocity;
    double v1 = (mass-mass2)/(mass+mass2) * Xvelocity + ((2*mass2)/(mass+mass2)) * Xvelocity2;
    double v2 = (mass2-mass)/(mass+mass2) * Xvelocity2 + ((2*mass)/(mass+mass2)) * Xvelocity;
    Xvelocity = v1;
    rect.Xvelocity = v2;

    double Yvelocity2 = rect.Yvelocity;
    double v3 = (mass-mass2)/(mass+mass2) * Yvelocity + ((2*mass2)/(mass+mass2)) * Yvelocity2;
    double v4 = (mass2-mass)/(mass+mass2) * Yvelocity2 + ((2*mass)/(mass+mass2)) * Yvelocity;
    Yvelocity = v3;
    rect.Yvelocity = v4;
}

void entity::calculateGravity(float G, entity& rect) {

    //std::cout << rect.id << ": " << rect.Yvelocity << "\n";
    //std::cout << id << ": " << Yvelocity << "\n";
    if (!(std::isfinite(Yvelocity))) {
        std::cout << id << "\n";
        exit(0);
    }

    if (!(std::isfinite(rect.Yvelocity))) {
        std::cout << "rect: " << rect.id << " " << rect.Yvelocity << "\n";
        exit(0);
    }

    float force = (G*mass*rect.mass)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y));
    
    float vecX = rect.X-X;
    float vecY = rect.Y-Y;

    if (vecX == 0) {
        vecX = 0.001;
    }

    //std::cout << "mass: " << rect.mass << " " << mass << "\n";
    float cosFactor = 1/(sqrt((vecY/vecX)*(vecY/vecX)+1));
    float sinFactor = fabs(vecY/vecX)/(sqrt((vecY/vecX)*(vecY/vecX)+1));

    float forceX = cosFactor*force;
    float forceY = sinFactor*force;

    if (!(std::isfinite(forceX)) || !(std::isfinite(forceY))) {
        std::cout << Dist(X, rect.X, Y, rect.Y) << "\n";
        std::cout << X << " " << Y << " " << rect.X << " " << rect.Y << " " << "\n";
        std::cout << vecX << " " << vecY << "\n";
        std::cout << "forces are nan! " << forceX << " " << forceY << " " << force << "\n";
        exit(0);
    }

    //std::cout << "forces: " << forceX << " "  << forceY << "\n";

    Xacceleration += (fabs(forceX/mass) * (int)((vecX/fabs(vecX))));
    Yacceleration += (fabs(forceY/mass) * (int)((vecY/fabs(vecY))));

    rect.Xacceleration += (fabs(forceX/rect.mass) * (int)(((-vecX)/fabs(vecX))));
    rect.Yacceleration += (fabs(forceY/rect.mass) * (int)(((-vecY)/fabs(vecY))));

    if (!(std::isfinite(Xacceleration)) || !(std::isfinite(rect.Yacceleration))) {
        std::cout << "accelerations are nan!\n";
        exit(0);
    }

    //std::cout << "accelerations: " << Xacceleration << " "  << rect.Yacceleration << "\n";
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

double entity::calculateVelocity() {
    return sqrt(Xvelocity*Xvelocity + Yvelocity*Yvelocity);
}

double entity::calculateAngle() {
    return (atan(Yvelocity/Xvelocity)*180/PI);
}
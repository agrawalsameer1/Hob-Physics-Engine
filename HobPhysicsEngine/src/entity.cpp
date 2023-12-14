#include "entity.h"

// Calculate distance between two entities
float Dist(float x1, float x2, float y1, float y2) {
    return (sqrt(((x2 - x1)*(x2-x1) +(y2 - y1)*(y2-y1)))); 
}

// Draw Circle
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

// If mass is not explicitly defined, set mass to radius^2, and randomize velocities
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

// Same as above, but if velocities are defined
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

// Same as above, but if mass is also provided
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

// For setting attractors
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

// If entity is not an attractor, move
void entity::updatePos(double dTime) {
    if (!attractor) {
        X += (Xvelocity*(dTime) + 1/2*Xacceleration*dTime*dTime);
        Y += (Yvelocity*(dTime) + 1/2*Yacceleration*dTime*dTime);

        Xdisplacement = fabs(Xvelocity*(dTime) + 1/2*Xacceleration*dTime*dTime);
        Ydisplacement = fabs(Yvelocity*(dTime) + 1/2*Yacceleration*dTime*dTime);

        Xvelocity += Xacceleration*dTime;
        Yvelocity += Yacceleration*dTime;
    }
}

// Check entity is outside window
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

// Bounce entity off wall if necessary
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

// If distance between centers of two circles is less than the sum of their radii, they are colliding
bool entity::isColliding(entity circle) {
    return (Dist(circle.X, X, circle.Y, Y) < (circle.Rad + Rad));
}

// Use conservation of momentum & kinetic energy to calculate collision
void entity::calculateCollision(entity &rect) {
   float damping = 1.0; // Represents how much energy is lost during collision, 1 represents perfectly elastic

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

// Calculate gravity between two entities
void entity::calculateGravity(float G, entity& rect) {
    if (Dist(rect.X, X, rect.Y, Y) > 10) {
        return;
    }

    if (rect.id == id) {
        return;
    }
    
    float softening = 0.3; // Prevents acceleration from reaching infinity if particles are very close together
    float force = 0;

    // If mass of entity is 0, it accelerates as if it had a mass of 1, but does not attract the other body at all
    if (mass == 0) {
        force = (G*1*rect.mass)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y) + softening*softening);
    }

    if (rect.mass == 0) {
        force = (G*mass*1)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y) + softening*softening);
    }

    // If mass of both entities is non-zero, calculate force normally
    if (mass > 0 && rect.mass > 0) {
        force = (G*mass*rect.mass)/(Dist(X, rect.X, Y, rect.Y)*Dist(X, rect.X, Y, rect.Y) + softening*softening);
    }
    
    // Calculate x and y components of gravity
    float vecX = rect.X-X;
    float vecY = rect.Y-Y;

    // Prevent divided by 0 error
    if (vecX == 0) {
        vecX = 0.001;
    }

    // Represents cos(tan(y/x))
    float cosFactor = 1/(sqrt((vecY/vecX)*(vecY/vecX)+1));

    // Represents sin(tan(y/x))
    float sinFactor = fabs(vecY/vecX)/(sqrt((vecY/vecX)*(vecY/vecX)+1));

    float forceX = cosFactor*force;
    float forceY = sinFactor*force;

    // Accelerate particle with 0 mass as if it had mass of 1, and do not accelerate other particle at all
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

    // If both masses are non-zero, calculate forces normally
    if (rect.mass > 0 && mass > 0) {
        rect.Xacceleration += (fabs(forceX/rect.mass) * (int)(((-vecX)/fabs(vecX))));
        rect.Yacceleration += (fabs(forceY/rect.mass) * (int)(((-vecY)/fabs(vecY))));
        Xacceleration += (fabs(forceX/mass) * (int)((vecX/fabs(vecX))));
        Yacceleration += (fabs(forceY/mass) * (int)((vecY/fabs(vecY))));
    }
}

// Reset acceleration of entity. Must be called before any call to barnes-Hut
void entity::reset() {
    Xacceleration = 0;
    Yacceleration = 0;   
}

void entity::render(SDL_Renderer* renderer) {
    draw_circle(renderer, X, Y, Rad);
}
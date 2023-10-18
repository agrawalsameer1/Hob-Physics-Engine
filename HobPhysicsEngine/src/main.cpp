#include "allEntities.h"
#include "entity.h"
#include "QuadTree.h"
#include "textBox.h"

using namespace std;

float Distance(float x1, float x2, float y1, float y2) {
    return (sqrt(((x2 - x1)*(x2-x1) +(y2 - y1)*(y2-y1)))); 
}

int main() {
    int selected = -1;

    float frameRate = 100;
    // Create Window and Renderer
    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 750, SDL_WINDOW_RESIZABLE); // 1360x770
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    QuadTree* tree = new QuadTree(window);
    
    // Initialize list with all our entities
    allEntities list = allEntities();
    int x = 0;
    int y = 0;
    int w,h = 0;
    SDL_GetWindowSize(window, &w, &h);

    std::cout << w << " " << h << "\n";

    float n = 60.0;

    
    for (int i = 0; i < 1500; i++) {
        int x = (rand()%1100);
        int y = (rand()%750);

        entity* ent = new entity(2, x, y, 255, 255, 255, -y/10, x/10, i);   
        list.addRect(ent);     
        tree->insert(ent, tree->root);
        std::cout << i << "\n";
    }

    entity* trackedEntity = nullptr;

    SDL_Event e;
    bool quit = false;
    bool inRect = false;
    int id = 200;
    // Until user presses x button
    while (!quit){
        Uint64 start = SDL_GetPerformanceCounter(); // Count milliseconds since frame
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT){ // If user presses x button, quit
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) { // If user clicks
                inRect = false;
                int x = 0;
                int y = 0;
                SDL_GetMouseState(&x, &y); // Get mouse coordinates
                
                int size = 10;
                
                for (int i = 0; i < list.size; i++) {
                    if (Distance(list.rects[i]->X, x, list.rects[i]->Y, y) < list.rects[i]->Rad+size) {
                        inRect = true;
                        break;
                    }
                }

                if (!inRect) {
                    entity* ent = new entity(size, x-size/2, y-size/2, 255, 0, 0, id); // Create square of size with mouse click coords
                    id++;
                    
                    list.addRect(ent); // Add rect to allEntities object
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        float G = 150;
        for (int i = 0; i < 1; i++) {
            tree = list.update3(window, trackedEntity, 1/frameRate, 10, G, tree);//, 10); // Updates entity positions, checks for out of bounds entities, and checks for & calculates collisions
        }
        list.render(renderer, window);
        //tree->visualDisplay(tree->root, renderer);

        //std:cout << list.rects[0]->X << " " << list.rects[0]->Y << "\n";
    
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);

        Uint64 end = SDL_GetPerformanceCounter();
        
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        //std::cout << elapsedMS << "\n";
        
        //std::cout << elapsedMS << "\n";
        SDL_Delay(floor(1000/frameRate - elapsedMS));
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // After rendering, set back to black
        SDL_RenderClear(renderer); // Clear screen
    }

    for (int i = 0; i < list.size; i++) {
        delete list.rects[i];
    }

    delete tree;

    SDL_DestroyWindow(window);
    SDL_Quit();
}
#include "allEntities.h"
#include "rectEntity.h"
#include "textBox.h"

using namespace std;

int main() {
    // Create Window and Renderer
    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    char* content = strdup(("No rect is being tracked."));
    char* path = strdup("/Users/sameeragrawal/HobPhysicsEngine/src/font.ttf");
    SDL_Color White = {255, 255, 255};
    textBox defaultText = textBox(content, path, 12, White, 20, 20, 200, 25);
    
    // Initialize list with all our entities
    allEntities list = allEntities();

    rectEntity* trackedEntity = nullptr;

    SDL_Event e;
    bool quit = false;
    bool inRect = false;
    int frameCount = 0;
    double msTime = 0;

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

                for (int i = 0; i < list.size; i++) {
                    if (list.rects[i]->X <= x && x <= list.rects[i]->X + list.rects[i]->W && list.rects[i]->Y <= y && y <= list.rects[i]->Y + list.rects[i]->H) {
                        if (trackedEntity != nullptr) {
                            trackedEntity->R = 255;
                            trackedEntity->G = 0;
                            trackedEntity->B = 0;
                        }
                        trackedEntity = list.rects[i];
                        trackedEntity->R = 255;
                        trackedEntity->G = 255;
                        trackedEntity->B = 0;
                        inRect = true;
                        break;
                    }
                }

                if (!inRect) {
                    srand((unsigned) time(NULL));
                    // Running rand multiple times to get better random numbers for size
                    rand();
                    rand();
                    rand();
                    
                    int size = 5;

                    rectEntity* rectangle = new rectEntity(size, size, x-size/2, y-size/2, 255, 0, 0); // Create square of size with mouse click coords
                    
                    list.addRect(rectangle); // Add rect to allEntities object
                }
            }
        }
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedS = (end - start) / (float)SDL_GetPerformanceFrequency();
        trackedEntity = list.update(window, trackedEntity, elapsedS); // Updates entity positions, checks for out of bounds entities, and checks for & calculates collisions
        list.render(renderer, window);
        if (trackedEntity != NULL) {
            trackedEntity->massText.render(renderer, window);
            trackedEntity->velocityText.render(renderer, window);
        }

        else {
            defaultText.render(renderer, window);
        }

        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // After rendering, set back to black
        SDL_RenderClear(renderer); // Clear screen
        frameCount++;
        msTime += elapsedS;
        if (frameCount%100 == 0 && frameCount > 0) {
            std::cout << "Avg Frame Time: " << msTime*10 << " ms\n";
            msTime = 0;
        }
    }

    for (int i = 0; i < list.size; i++) {
        delete list.rects[i];
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
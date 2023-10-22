#include "allEntities.h"
#include "entity.h"
#include "QuadTree.h"
#include "textBox.h"
#include "../include/stb_image/stb_image_write.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

using namespace std;

float Distance(float x1, float x2, float y1, float y2) {
    return (sqrt(((x2 - x1)*(x2-x1) +(y2 - y1)*(y2-y1)))); 
}

int main() {
    system("mkdir images");
    system("rm movie.mp4");

    std::vector<Uint8*> allPixels;
    int frameCounter = 0;
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
    int width,height = 0;
    SDL_GetWindowSize(window, &width, &height);

    std::cout << width << " " << height << "\n";

    float n = 60.0;

    float galaxyRadius = 240.0;

    for (int i = 0; i < 20000; i++) {
        int x = (rand()%2 == 1) ? -((rand()%(static_cast<int>(galaxyRadius)))) : ((rand()%(static_cast<int>(galaxyRadius))));
        

        int y = (rand()%2 == 1) ? -(rand()%(int)(sqrt(galaxyRadius*galaxyRadius - (x)*(x)))) : (rand()%(int)(sqrt(galaxyRadius*galaxyRadius - (x)*(x))));

        double prob = exp(-(((x*x)/(galaxyRadius*galaxyRadius))+((y*y)/(galaxyRadius*galaxyRadius))))*0.2;

        double scale = (rand()%10)+5;

        if (Dist(x+550, 550, y+375, 375) < 20.0) {
            continue;
        }

        if (prob*scale > 1) {
            x = 550 + x;
            y = y + 375;
            entity* ent = new entity(1, x, y, 255, 255, 255, -(y-375)/(2.5), (x-550)/(2.5), i); // /2 too high, /3 too low   
            list.addRect(ent);    
            tree->insert(ent, tree->root);
        }
    }

    std::cout << list.size << "\n";

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
        float G = 125;
        for (int i = 0; i < 5; i++) {
            tree = list.update3(window, trackedEntity, (1.0/5)/frameRate, 10, G, tree);//, 10); // Updates entity positions, checks for out of bounds entities, and checks for & calculates collisions
        }
        list.render(renderer, window);
        //tree->textDisplay(tree->root, 0);
        //tree->visualDisplay(tree->root, renderer);

        //std:cout << list.rects[0]->X << " " << list.rects[0]->Y << "\n";

        Uint8* pixels = new (std::nothrow) unsigned char[width * height*4];

        //std::cout << static_cast<int>(pixels[3]) << " " << static_cast<int>(pixels[1]) << " " << static_cast<int>(pixels[2]) << "\n";

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // After rendering, set back to black

        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, pixels, width*4);

        if (frameCounter > 0) {
            const char* hey = ("images/hello" + std::to_string(frameCounter) + ".png").c_str();
            stbi_write_png(hey, width, height, 4, pixels, width*4);
        }
    
        SDL_RenderPresent(renderer);

        SDL_UpdateWindowSurface(window);

        Uint64 end = SDL_GetPerformanceCounter();
        
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        //std::cout << elapsedMS << "\n";
        
        //std::cout << elapsedMS << "\n";
        SDL_Delay(floor(1000/frameRate - elapsedMS));
    
        SDL_RenderClear(renderer); // Clear screen

        frameCounter++;
        //std::cout << "reached here!\n";

    }
    
    system("cd images");
    system("ffmpeg -r 30 -i images/hello%01d.png -vcodec mpeg4 -y -vb 80M movie.mp4");
    system("rm -r images");

    for (int i = 0; i < list.size; i++) {
        delete list.rects[i];
    }

    delete tree;

    SDL_DestroyWindow(window);
    SDL_Quit();
}
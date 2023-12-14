#include "allEntities.h"
#include "entity.h"
#include "QuadTree.h"
#include "textBox.h"
#include "../include/stb_image/stb_image_write.h"
#include "../include/stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

using namespace std;

float Distance(float x1, float x2, float y1, float y2) {
    return (sqrt(((x2 - x1)*(x2-x1) +(y2 - y1)*(y2-y1)))); 
}

// Check if a point is in an ellipse centered at the center of the window
bool inEllipse(float x, float y, float majorAxis, float minorAxis) {
    return (((x*x)/(majorAxis*majorAxis)) + ((y*y)/(minorAxis*minorAxis))) < 1;

}
// Random number between 0 and 1
double r2() {
    return (double)rand() / (double)RAND_MAX ;
}

int main() {
    // Delete folder of screenshots of simulation
    system("rm -r images");

    // Gradient data
    int imageWidth = 1600;//1024;
    int imageHeight = 3;//512;
    int numberChannels = 3;
    unsigned char *gradData = stbi_load("gradientreally.png", &imageWidth, &imageHeight, &numberChannels, 3);

    SDL_Color color = {255, 255, 255};

    // Load CMBR data
    int imgWidth = 750;//1024;
    int imgHeight = 750;//512;
    int numChannels = 4;

    std::cout << "hreaced here!\n";

    unsigned char *data = stbi_load("cmbractual.jpg", &imgWidth, &imgHeight, &numChannels, 3);
    
    system("mkdir images");
    system("rm movie.mp4");

    // For counting frames which have passed
    int frameCounter = 0;
    int selected = -1;

    // 1/100 seconds will pass every frame
    float frameRate = 100;

    float windowWidth = 1100;
    float windowHeight = 750;

    // Create Window and Renderer
    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE); // 1360x770
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    QuadTree* tree = new QuadTree(window);
    
    // Initialize list with all our entities
    allEntities list = allEntities(gradData);

    int x = 1100;
    int y = 750;

    int width,height = 0;
    SDL_GetWindowSize(window, &width, &height);

    bool succeeded;

    std::cout << width << " " << height << "\n";

    // Defines dimensions of starting ellipse of particles
    float galaxyRadiusMajor = 250.0;
    float galaxyRadiusMinor = 150.0;

    float maxRadius = galaxyRadiusMajor*galaxyRadiusMajor + galaxyRadiusMinor*galaxyRadiusMinor;

    // Defines foci of ellipse
    float focus1X = 550-sqrt(galaxyRadiusMajor*galaxyRadiusMajor-galaxyRadiusMinor*galaxyRadiusMinor);
    float focus1Y = 375;
    float focus2X = 550+sqrt(galaxyRadiusMajor*galaxyRadiusMajor-galaxyRadiusMinor*galaxyRadiusMinor);
    float focus2Y = 375;

    int counter = 0;

    // Defines strength of gravitational force
    float G = 80;
    
    for (int i = 0; i < 120000; i++) {
        // Spawn particles randomly 
        float x1 = r2()*galaxyRadiusMajor*2;
        float y1 = r2()*galaxyRadiusMinor*2;

        float x = x1;
        float y = y1;


        x -= galaxyRadiusMajor;
        y -= galaxyRadiusMinor;

        // Don't spawn particle in the exact center
        if (x == 0 && y == 0) {
            continue;
        }

        float randomPicker = r2();

        // If particle outside of ellipse, continue
        if ((((x*x)/(galaxyRadiusMajor*galaxyRadiusMajor)) + ((y*y)/(galaxyRadiusMinor*galaxyRadiusMinor))) > 1) {
            continue;
        }

        // Cause density of particles to decrease as you go further out in ellipse
        float majorAxis = sqrt(pow(x,2) + (pow(y,2)/pow(galaxyRadiusMinor/galaxyRadiusMajor,2)));
        float minorAxis = majorAxis*galaxyRadiusMinor/galaxyRadiusMajor;

        if (randomPicker < majorAxis/galaxyRadiusMajor * 0.7) {
            continue;
        }

        float radius = sqrt(x*x + y*y);

        // Convert ellipse coordinates of particle into screen coordinates
        int xCoord = static_cast<int>(x);
        int yCoord = static_cast<int>(y);
        xCoord = (windowWidth/2) + xCoord;
        yCoord = yCoord + (windowHeight/2);

        // If particle is very near the center give it a large mass, else give it a default mass of 1
        float mass = 1;

        if (majorAxis/galaxyRadiusMajor < 0.01) {
            mass = 15;
        }

        // Give particle a velocity proportional to its distance from the center
        float velocity = 2.57*Dist(x, 0, y, 0);

        // Spawn entity and add it to QuadTree and rendering queue
        entity* ent = new entity(1, xCoord, yCoord, 255, 255, 255, -((y*galaxyRadiusMajor*galaxyRadiusMajor)/(radius*maxRadius)*velocity), ((x*galaxyRadiusMinor*galaxyRadiusMinor)/(radius*maxRadius)*velocity), i, mass); // /2 too high, /3 too low 

        succeeded = tree->insert(ent, tree->root);

        if (succeeded) {
            list.addEntity(ent);
        }

        counter++;
    }
    
    // Number of elements
    std::cout << list.size << "\n";

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
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        // Step forward time
        for (int i = 0; i < 1; i++) {
            tree = list.update(window, (1.0)/frameRate, 0.5, G, tree);//, 2*galaxyRadiusMajor, 2*galaxyRadiusMinor);//, massFull);//, 10); // Updates entity positions, checks for out of bounds entities, and checks for & calculates collisions
        }

        list.render(renderer, window);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // After rendering, set back to black

        // Read pixels from screen to create video
        Uint8* pixels = new (std::nothrow) unsigned char[width * height*4];
        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ABGR8888, pixels, width*4);

        // Create png out of pixel data
        if (frameCounter > 0) {
            const char* hey = ("images/hello" + std::to_string(frameCounter) + ".png").c_str();
            stbi_write_png(hey, width, height, 4, pixels, width*4);
        }

        delete[] pixels;

        // Draw entities to screen
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
        
        // Get time taken per frame
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        std::cout << elapsedMS << "\n";

        SDL_Delay(floor(1000/frameRate - elapsedMS));
    
        SDL_RenderClear(renderer); // Clear screen

        frameCounter++;
    }
    
    // Convert screen images to video
    system("cd images");
    system("ffmpeg -r 30 -i images/hello%01d.png -vcodec mpeg4 -y -vb 80M movie.mp4");
    system("rm -r images");


    // Clean-up
    for (int i = 0; i < list.size; i++) {
        delete list.entities[i];
    }

    tree->deallocate(tree->root);

    if (gradData) {
        delete gradData;
    }
    
    if (data) {
        delete data;
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}
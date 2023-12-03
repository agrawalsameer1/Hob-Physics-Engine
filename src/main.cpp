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

bool inEllipse(float x, float y, float majorAxis, float minorAxis) {
    return (((x*x)/(majorAxis*majorAxis)) + ((y*y)/(minorAxis*minorAxis))) < 1;

}

float velocityFunction(float x) {
    float x1 = (x-120)*(x-120) - 120*120;
    float x2 = -x1/120;
    return x2/60;
}

double r2() {
    return (double)rand() / (double)RAND_MAX ;
}

int main() {
    system("rm -r images");

    int imageWidth = 1600;//1024;
    int imageHeight = 3;//512;
    int numberChannels = 3;
    unsigned char *gradData = stbi_load("gradientreally.png", &imageWidth, &imageHeight, &numberChannels, 3);

    std::cout << static_cast<int>(*(gradData + (1599) * 3)) << " " << static_cast<int>(*(gradData + (1599) * 3 + 1)) << " " << static_cast<int>(*(gradData + (1599) * 3 + 2)) << "\n";

    SDL_Color color = {255, 255, 255};

    int imgWidth = 750;//1024;
    int imgHeight = 750;//512;
    int numChannels = 4;

    std::cout << "hreaced here!\n";

    unsigned char *data = stbi_load("cmbractual.jpg", &imgWidth, &imgHeight, &numChannels, 3);
    
    system("mkdir images");
    system("rm movie.mp4");

    std::vector<Uint8*> allPixels;
    int frameCounter = 0;
    int selected = -1;

    float frameRate = 50;

    float windowWidth = 1100;
    float windowHeight = 750;
    // Create Window and Renderer
    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE); // 1360x770
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    QuadTree* tree = new QuadTree(window);
    
    // Initialize list with all our entities
    allEntities list = allEntities(gradData);

    int x = 0;
    int y = 0;
    int width,height = 0;
    SDL_GetWindowSize(window, &width, &height);
    bool succeeded;

    std::cout << width << " " << height << "\n";

    float n = 60.0;

    float galaxyRadiusMajor = 250.0;
    float galaxyRadiusMinor = 150.0;//256;//275;

    float maxRadius = galaxyRadiusMajor*galaxyRadiusMajor + galaxyRadiusMinor*galaxyRadiusMinor;

    float focus1X = 550-sqrt(galaxyRadiusMajor*galaxyRadiusMajor-galaxyRadiusMinor*galaxyRadiusMinor);
    float focus1Y = 375;
    float focus2X = 550+sqrt(galaxyRadiusMajor*galaxyRadiusMajor-galaxyRadiusMinor*galaxyRadiusMinor);
    float focus2Y = 375;

    int counter = 0;

    float smallMass = 5;

    float largeMass = 3360*smallMass;

    float G = 80;

    /*entity* ent = new entity(3, 550, 375, 255, 255, 255, 0, 0, 11000, largeMass, true); // /2 too high, /3 too low // between 16500 and 16875
    succeeded = tree->insert(ent, tree->root);
    if (!succeeded) {
        std::cout << "FAILED!\n";
    }
    list.addRect(ent);*/
    
    for (int i = 0; i < 120000; i++) {
        /*float multiplier = r2();
        float radius = multiplier*galaxyRadiusMajor;
        float radius2 = multiplier*galaxyRadiusMinor;
        double alpha = r2() * 2 * PI;
        float x = (float) (cos(alpha) * radius);
		float y = (float) (sin(alpha) * radius2);*/

        float x1 = r2()*galaxyRadiusMajor*2;
        float y1 = r2()*galaxyRadiusMinor*2;

        float x = x1;
        float y = y1;


        x -= galaxyRadiusMajor;
        y -= galaxyRadiusMinor;

        if (x == 0 && y == 0) {
            continue;
        }

        float randomPicker = r2();

        if ((((x*x)/(galaxyRadiusMajor*galaxyRadiusMajor)) + ((y*y)/(galaxyRadiusMinor*galaxyRadiusMinor))) > 1) {
            continue;
        }

        float majorAxis = sqrt(pow(x,2) + (pow(y,2)/pow(galaxyRadiusMinor/galaxyRadiusMajor,2)));
        
        float minorAxis = majorAxis*galaxyRadiusMinor/galaxyRadiusMajor;

        if (randomPicker < majorAxis/galaxyRadiusMajor * 0.7) {
            continue;
        }

        float radius = sqrt(x*x + y*y);

        /*
        if ((inEllipse(x, y, 0.2*galaxyRadiusMajor, 0.2*galaxyRadiusMinor))) {
            if (randomPicker < 0.1) {
                continue;
            }
        }
        else if ((inEllipse(x, y, 0.4*galaxyRadiusMajor, 0.4*galaxyRadiusMinor))) {
            if (randomPicker < 0.4) {
                continue;
            }
        }
        else if ((inEllipse(x, y, 0.6*galaxyRadiusMajor, 0.6*galaxyRadiusMinor))) {
            if (randomPicker < 0.5) {
                continue;
            }
        }
        else if ((inEllipse(x, y, 0.8*galaxyRadiusMajor, 0.8*galaxyRadiusMinor))) {
            if (randomPicker < 0.6) {
                continue;
            }
        }
        else if ((inEllipse(x, y, 1*galaxyRadiusMajor, 1*galaxyRadiusMinor))) {
            if (randomPicker < 0.7) {
                continue;
            }
        }
        */

        int xCoord = static_cast<int>(x);
        int yCoord = static_cast<int>(y);
        
        /*if (Dist(x+550, 550, y+375, 375) < 5) {
            continue;
        }*/

        xCoord = (windowWidth/2) + xCoord;
        yCoord = yCoord + (windowHeight/2);

        float mass = 1;

        if (majorAxis/galaxyRadiusMajor < 0.01) {
            float mass = 55;
            //mass = 1.5;
        }

        //float mass = 2-majorAxis/galaxyRadiusMajor;//4-radius/400.0;
        //double massDecider = r2();

        //std::cout << massDecider << " " << radius/galaxyRadius << "\n";

        /*if (massDecider < 0.01) {
            mass = smallMass;
        } */

        //float velocity = exp(-1*radius/54) + 0.8411;
        //velocity *= 1.5;

        float velocity = 2.57*Dist(x, 0, y, 0);

        int i1 = static_cast<int>(x1);
        int j1 = static_cast<int>(y1);

        //float color = static_cast<int>(*(data + (i1 + imgWidth * j1) * 3));
        //float mass = 1 + (color-108)/8;

        //std::cout << mass << '\n';
        //std::cout << velocity << "\n";

        entity* ent = new entity(1, xCoord, yCoord, 255, 255, 255, -((y*galaxyRadiusMajor*galaxyRadiusMajor)/(radius*maxRadius)*velocity), ((x*galaxyRadiusMinor*galaxyRadiusMinor)/(radius*maxRadius)*velocity), i, mass); // /2 too high, /3 too low 

        succeeded = tree->insert(ent, tree->root);

        if (succeeded) {
            list.addRect(ent);
            /*if (ent->mass > 0) {
                massFull.addRect(ent);
            } */
        }

        counter++;

        //if (prob*scale > 1) {
           
            /*if (dist < 10) {
                mass = 5;
            }*/
            
            //entity* ent = new entity(1, x, y, 255, 255, 255, (-(100.0/dist)*(y-375)), ((100.0/dist)*(x-550)), i, mass); // /2 too high, /3 too low 
            
        //}
    }
    
    /*
    entity* ent = new entity(1, 550, 375, 255, 255, 255, 0, 0, -1, 100, true); // /2 too high, /3 too low 

    succeeded = tree->insert(ent, tree->root);

    if (succeeded) {
        list.addRect(ent);
        if (ent->mass > 0) {
            massFull.addRect(ent);
        }
    }

    std::cout << succeeded << "\n";
    */

    //entity* ent = new entity(3, 550, 375, 255, 255, 255, 0, 0, 110000, largeMass, true); // /2 too high, /3 too low // between 16500 and 16875
    //succeeded = tree->insert(ent, tree->root);
    //succeeded = true;
    /*if (!succeeded) {
        std::cout << "FAILED!\n";
    }
    list.addRect(ent);
    massFull.addRect(ent);*/

    std::cout << list.size << "\n";
    //tree->countElements(tree->root);
    //std::cout << tree->numOfElements << "\n";

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
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < 1; i++) {
            tree = list.update3(window, trackedEntity, (1.0)/frameRate, 10, G, tree);//, 2*galaxyRadiusMajor, 2*galaxyRadiusMinor);//, massFull);//, 10); // Updates entity positions, checks for out of bounds entities, and checks for & calculates collisions
        }

        list.render(renderer, window);
        //tree->visualDisplay(tree->root, renderer);
        //tree->textDisplay(tree->root, 0);
        

        //std:cout << list.rects[0]->X << " " << list.rects[0]->Y << "\n";

        Uint8* pixels = new (std::nothrow) unsigned char[width * height*4];

        //std::cout << static_cast<int>(pixels[3]) << " " << static_cast<int>(pixels[1]) << " " << static_cast<int>(pixels[2]) << "\n";

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // After rendering, set back to black

        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ABGR8888, pixels, width*4);

        if (frameCounter > 0) {
            const char* hey = ("images/hello" + std::to_string(frameCounter) + ".png").c_str();
            stbi_write_png(hey, width, height, 4, pixels, width*4);
        }

        delete[] pixels;
    
        SDL_RenderPresent(renderer);

        SDL_UpdateWindowSurface(window);

        Uint64 end = SDL_GetPerformanceCounter();
        
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        //std::cout << elapsedMS << "\n";
        
        std::cout << elapsedMS << "\n";
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

    tree->deallocate(tree->root);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
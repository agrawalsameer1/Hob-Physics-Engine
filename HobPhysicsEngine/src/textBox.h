#include "../include/SDL2/SDL.h"
#include "../include/SDL2_ttf/SDL2/SDL_ttf.h"
#include <iostream>
#include<cstdlib>

#pragma once

class textBox {
    public:
        SDL_Rect box;
        char* content;
        TTF_Font *font;
        int fontSize;
        SDL_Color textColor;
        textBox();
        textBox(char* words, char* fontPath, int size, SDL_Color textColor, int x, int y, int w, int h);
        void render(SDL_Renderer* renderer, SDL_Window *window);
};
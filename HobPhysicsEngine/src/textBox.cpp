#include "textBox.h"

textBox::textBox() {
    TTF_Init();
    font = TTF_OpenFont("/Users/sameeragrawal/HobPhysicsEngine/src/font.ttf", 24);
    fontSize = 24;
    box.x = 0;
    box.y = 0;
    box.w = 0;
    box.h = 0;
}

textBox::textBox(char* words, char* fontPath, int size, SDL_Color color, int x, int y, int w, int h) {
    TTF_Init();
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    content = words;
    font = TTF_OpenFont(fontPath, size);
    fontSize = size;
    textColor = color;
}

void textBox::render(SDL_Renderer* renderer, SDL_Window *window) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, content, textColor);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    box.w = surfaceMessage->w;
    box.h = surfaceMessage->h;
    SDL_RenderCopy(renderer, Message, NULL, &box);
}
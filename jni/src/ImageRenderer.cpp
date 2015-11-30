//
//  ImageRenderer.cpp
//  CanyonBomber
//
//  Created by Mac-Awais on 29/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#include "ImageRenderer.hpp"

using namespace std;

SDL_Window* ImageRenderer::window = nullptr;
SDL_Renderer* ImageRenderer::renderer = nullptr;

Sprite ImageRenderer::LoadSprite(const char* file){
    Sprite result;
    result.texture = NULL;
    result.w = 0;
    result.h = 0;
    
    SDL_Surface* temp;
    
    /* Load the sprite image */
    temp = SDL_LoadBMP(file);
    if (temp == NULL)
    {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return result;
    }
    result.w = temp->w;
    result.h = temp->h;
    
    /* Create texture from the image */
    result.texture = SDL_CreateTextureFromSurface(renderer, temp);
    if (!result.texture) {
        fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
        return result;
    }
    SDL_FreeSurface(temp);
    
    return result;
}

void ImageRenderer::draw(const Sprite sprite, int x, int y, int w, int h){
    // On définit un rectangle avec le coin gauche positionné en (x, y)
    // avec w*h de dimensions
    SDL_Rect destRect = {x, y, w, h};
    
    /* Blit the sprite onto the screen */
    SDL_RenderCopy(renderer, sprite.texture, NULL, &destRect);
}

void ImageRenderer::initialiseWindowAndRenderer(SDL_Window* w, SDL_Renderer* r){
    ImageRenderer::window = w;
    ImageRenderer::renderer = r;
}

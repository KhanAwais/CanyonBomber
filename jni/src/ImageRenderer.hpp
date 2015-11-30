//
//  ImageRenderer.hpp
//  CanyonBomber
//
//  Created by Mac-Awais on 29/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#ifndef ImageRenderer_hpp
#define ImageRenderer_hpp

#include "SDL.h"

using namespace std;

typedef struct Sprite
{
    SDL_Texture* texture;
    Uint16 w;
    Uint16 h;
} Sprite;

class ImageRenderer {
public:
    /**
     * variables de classe
     */
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    /**
     * méthodes de classe
     */
    static Sprite LoadSprite(const char* file);
    static void draw(const Sprite sprite, int x, int y, int w, int h);
    static void initialiseWindowAndRenderer(SDL_Window* w, SDL_Renderer* r);
};

#endif /* ImageRenderer_hpp */

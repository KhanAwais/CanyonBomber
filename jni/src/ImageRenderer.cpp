//
//  ImageRenderer.cpp
//  CanyonBomber
//
//  Created by Mac-Awais on 29/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#include "ImageRenderer.hpp"
#include <android/log.h> // Pour le log en natif vers logcat

using namespace std;

SDL_Window* ImageRenderer::window = nullptr;
SDL_Renderer* ImageRenderer::renderer = nullptr;
map<string, Sprite> ImageRenderer::sprites_charges;

Sprite ImageRenderer::LoadSprite(const char* file){
    Sprite result;
    result.texture = NULL;
    result.w = 0;
    result.h = 0;
    
    SDL_Surface* temp;
    
    /* Load the sprite image */
    temp = SDL_LoadBMP(file);
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,60,180,213));
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
        // On log le non chargement du sprite
        string tag = "ImageRenderer.cpp";
        string msg = "On quitte car le sprite n'a pas réussi à être chargé : ";
        __android_log_print(50, tag.c_str(), "%s %s", msg.c_str(), file);

        fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(temp);
        return result;
    }
    SDL_FreeSurface(temp);
    
    return result;
}

void ImageRenderer::draw(string fileSprite, int x, int y, int w, int h){
    // On essaie de récupérer le Sprite à partir des sprites déjà chargés
    Sprite sprite;
    map<string, Sprite>::iterator it;
    it = sprites_charges.find(fileSprite);
    if (it != sprites_charges.end()) {
        sprite = it->second;
    } else {
        // Sinon on le crée et on le garde pour les prochains affichages
string tag = "ImageRenderer.cpp";
string msg = "On sauvegarde le sprite : ";
__android_log_print(50, tag.c_str(), "%s %s", msg.c_str(), fileSprite.c_str());
        sprite = ImageRenderer::LoadSprite(fileSprite.c_str());
        if (sprite.texture == NULL) {
            exit(2);
        }
        // on l'ajoute à la map
        sprites_charges[fileSprite] = sprite;
    }
    
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

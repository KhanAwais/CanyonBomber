#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "ImageRenderer.cpp"
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    if(SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0)
        exit(2);
    
    ImageRenderer::initialiseWindowAndRenderer(window, renderer);
    
    
    string file = "background2.bmp";
    
    Sprite sprite = ImageRenderer::LoadSprite(file.c_str());
    if(sprite.texture == NULL)
        exit(2);
    
    /* Main render loop */
    Uint8 done = 0;
    SDL_Event event;
    while(!done)
    {
        /* Check for events */
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN)
            {
                done = 1;
            }
        }
        
        
        /* Draw a gray background */
        SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderClear(renderer);
        
        ImageRenderer::draw(sprite, 0, 0, 200, 200);//background
        
        /* Update the screen! */
        SDL_RenderPresent(renderer);
        
        SDL_Delay(10);
    }
    
    exit(0);
}

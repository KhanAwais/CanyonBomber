#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <android/log.h> 

#include "SDL.h"
#include "ImageRenderer.hpp"
#include <string>
#include "Application.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    if(SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0)
    {
        string tag = "main.cpp";
        string msg = "On quitte, la window pas créer ";
        __android_log_print(50, tag.c_str(), "%s", msg.c_str());
        exit(2);
    }
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    ImageRenderer::initialiseWindowAndRenderer(window, renderer);
    
    // On instancie le modèle de notre application
    Application app(w, h);
    
    /* Main render loop */
    Uint8 done = 0;
    SDL_Event event;
    while(!done)
    {
        // On joue un tour du jeu
        app.jouer();

        /* Check for events */
        while(SDL_PollEvent(&event))
        {
            // Si on reçoit l'événement SDL_QUIT ou si l'utilisateur presse un bouton (back ou autre)
            if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN)
            {
                done = 1;
            }
            // Sinon, si doigt posé sur l'écran
            else if(event.type == SDL_FINGERDOWN){
                int pos_x_touch = event.tfinger.x * w;
                int pos_y_touch = event.tfinger.y * h;

                if (app.partieCommencee()) {
                    app.click(w, h);
                }
                if(!app.partieCommencee()){
                    if(pos_x_touch > (w/2) - w*0.33 &&
                       pos_y_touch > h - 200 &&
                       pos_x_touch < ((w/2)-w*0.33 + w*0.66) &&
                       pos_y_touch < (h - 200 + 120))
                    {
                        app.click(w, h);
                    }
                }
            }
        }
		SDL_RenderClear(renderer);
        
        // On dit à l'application de se dessiner!
        app.dessiner();
        
        /* Update the screen! */
        SDL_RenderPresent(renderer);
        
        SDL_Delay(Application::intervalle_rafraichir);
    }
    
    exit(0);
}

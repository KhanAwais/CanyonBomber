//
//  Bombe.cpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#include "Bombe.hpp"
#include <math.h>
#include <iostream>
#include <string>
#include "ImageRenderer.hpp"
#include <android/log.h> // Pour le log en natif vers logcat

const int Bombe::nb_explosions_max = 6;

Bombe::Bombe(int pos_x, int pos_y, int direction){
    _pos_x = pos_x;
    _pos_y = pos_y;
    _direction = direction;
    
    _nb_explosions = 0;
}

int Bombe::getPosX(){
    return _pos_x;
}

int Bombe::getPosY(){
    return _pos_y;
}

int Bombe::getNbExplosions(){
    return _nb_explosions;
}

/**
 * Fait tomber la bombe
 */
void Bombe::avancer(int pas){
    _pos_x = _pos_x + _direction * pas / sqrt(37);
    _pos_y = _pos_y + 6 * pas / sqrt(37);
}

/**
 * La bombe vient de heurter un bloc
 */
int Bombe::explose(){
    _nb_explosions++;
    return getNbExplosions();
}

void Bombe::affiche(){
    std::cout << "Bombe { _pos_x=" << _pos_x << ", _pos_y=" << _pos_y << ", _nb_explosions=" << _nb_explosions
        << " }" << std::endl;
}

void Bombe::dessiner(int longueur){
    std::string spriteBombe = "bombe.bmp";
    ImageRenderer::draw(spriteBombe, _pos_x, _pos_y, longueur, longueur);
}

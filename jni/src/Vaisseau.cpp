//
//  Vaisseau.cpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#include "Vaisseau.hpp"
#include "Application.hpp"
#include <stdlib.h>// pour rand()
#include "Terrain.hpp"
#include <iostream>

using namespace std;

Vaisseau::Vaisseau(int x_min_terrain, int x_max_terrain){
    _score = 0;
    
    bombe = nullptr;
    _bombeChargee = true;
    
    /**
     * TODO : modifier ces valeurs en fonction de l'image
     */
    _longueur = 50;
    _hauteur = 25;
    
    _x_min = x_min_terrain;
    _x_max = x_max_terrain - _longueur;
    
    // Le pas dépend de la longueur horizontale que le vaisseau peut parcourir et du temps de rafraichissement de l'écran
    // je veux que le vaisseau parcoure la longueur horizontale en 10 sec
    _pas = Application::intervalle_rafraichir * (_x_max - _x_min) / 10000;
    
    // on place le vaisseau pour le debut de son mouvement
    debut_mouvement();
}

void Vaisseau::gagner(int nb){
    _score = _score + nb;
}

/**
 * Place le vaisseau pour le début de son mouvement
 */
void Vaisseau::debut_mouvement(){
    if(rand()%2 == 0){
        // on le met à gauche en direction de la droite
        _pos_x = _x_min;
        _direction = 1;
    } else {
        // sinon on fait l'inverse
        _pos_x = _x_max;
        _direction = -1;
    }
    if(rand()%2 == 0 && _pos_y >= Terrain::hauteur_min_vaisseau && (_pos_y+_hauteur) < Terrain::hauteur_max_vaisseau){
        // on le laisse à la même hauteur
    } else {
        // on le met à une position aléatoire (mais cohérente) en ordonnée
        _pos_y = rand()%((Terrain::hauteur_max_vaisseau - _hauteur) - Terrain::hauteur_min_vaisseau)
                    + Terrain::hauteur_min_vaisseau;
    }
}

/**
 * Fait avancer le vaisseau
 */
void Vaisseau::avancer(){
    _pos_x = _pos_x + _direction * _pas;
    if ( _pos_x < _x_min || _pos_x > _x_max ){
        debut_mouvement();
    }
}

/**
 * instancie une Bombe et met à jour _bombeChargee pour ne pas
 * pouvoir en lancer d'autre tant que celle ci n'a pas explosé
 */
void Vaisseau::lacherBombe(){
    if(_bombeChargee){
        // C'est la classe Terrain qui se chargera de la faire avancer
        bombe = new Bombe(_pos_x+_longueur/2, _pos_y+_hauteur/2, _direction);
        // on empêche de tirer juste après
        _bombeChargee = false;
    }
}

/**
 * détruit l'ancienne Bombe et met à jour _bombeChargee
 * pour pouvoir en tirer une nouvelle
 */
void Vaisseau::rechargerBombe(){
    bombe = nullptr;
    _bombeChargee = true;
}

void Vaisseau::afficher(){
    cout << "Vaisseau { _x_min=" << _x_min << ", _x_max=" << _x_max << ", _pas=" << _pas
        << ", _direction=" << _direction << ", _pos_x" << _pos_x << ", _pos_y" << _pos_y
        << endl;
    if (bombe != nullptr) {
        bombe->affiche();
    }
}

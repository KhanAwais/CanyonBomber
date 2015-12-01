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
#include "ImageRenderer.hpp"
#include <string>
#include <android/log.h> // Pour le log en natif vers logcat

#include <sstream>

using namespace std;

Vaisseau::Vaisseau(int x_min_terrain, int x_max_terrain){
    _score = 0;
    
    bombe = nullptr;
    _bombeChargee = true;
    
    _longueur = 69; // valeur fixé en fonction des sprites
    _hauteur = 60; // valeur fixé en fonction des sprites
    
    _nb_vies_restantes = 3;

    _x_min = x_min_terrain;
    _x_max = x_max_terrain - _longueur;
    
    // Le pas dépend de la longueur horizontale que le vaisseau peut parcourir et du temps de rafraichissement de l'écran
    // je veux que le vaisseau parcoure la longueur horizontale en 10 sec
    _pas = Application::intervalle_rafraichir * (_x_max - _x_min) / 10000;
    // si le pas calculé est à 0 (écran de résolution petite probablement), alors on le met à 1
    if (_pas == 0) {
        _pas = 1;
    }
    
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

/**
 * L'utilisateur vient de rater sa bombe : elle n'a touché aucun bloc
 * On décrémente donc le nombre de vies
 * Si il n'en reste plus (=0) alors sa partie est finie
 */
bool Vaisseau::bombeRatee(){
    _nb_vies_restantes--;
    if (_nb_vies_restantes == 0) {
        return true;
    } else {
        return false;
    }
}

void Vaisseau::afficher(){
    cout << "Vaisseau { _x_min=" << _x_min << ", _x_max=" << _x_max << ", _pas=" << _pas
        << ", _direction=" << _direction << ", _pos_x" << _pos_x << ", _pos_y" << _pos_y
        << endl;
    if (bombe != nullptr) {
        bombe->affiche();
    }
}

void Vaisseau::dessiner(){
    // On affiche le vaisseau
    string spriteVaisseau;
    if (_direction == 1) {
        spriteVaisseau = "Gohan1.bmp"; // Il se déplace vers la droite
    } else {
        spriteVaisseau = "Gohan-1.bmp"; // Il se déplace vers la gauche
    }
    ImageRenderer::draw(spriteVaisseau, _pos_x, _pos_y, _longueur, _hauteur); // Les images du vaisseau sont toujours à 69*60
}

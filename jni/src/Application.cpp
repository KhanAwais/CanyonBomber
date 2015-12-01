//
//  Application.cpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#include "Application.hpp"
#include "Terrain.hpp"
#include <iostream>
#include "ImageRenderer.hpp"

using namespace std;

const int Application::intervalle_rafraichir = 200; // temps d'intervalle de rafraichissement de l'interface (en ms)

Application::Application(int w, int h){
    _isGameStarted = false;
    _terrain = nullptr;
    _longueur = w;
    _hauteur = h;
}

/**
 * Gestion du clic de l'utilisateur
 * Permet de commencer une partie ou si partie en cours, délègue à _terrain
 */
void Application::click(int x, int y){
    if(!_isGameStarted){
        _isGameStarted = true;
        /**
         * TODO : changer ces valeurs en fonction du device
         */
        _terrain = new Terrain(_longueur, _hauteur, this);
    } else {
        _terrain->clicUtilisateur();
    }
}

/**
 * joue un "tour" de la partie en cours
 */
void Application::jouer(){
    if (_isGameStarted) {
        _terrain->jouer();
    }
}

/**
 * appelé lorsque la partie est terminée
 */
void Application::endGame(){
    _isGameStarted = false;
    delete _terrain; // appelle le destructeur(par défaut) de Terrain qui désalloue le tableau blocs
    _terrain = nullptr;
}

void Application::afficher(){
    //cout << "Application { _isGameStarted = " << _isGameStarted << " } " << endl;
    if(_isGameStarted && _terrain != nullptr){
        _terrain->afficher();
    }
}

/**
 * Accesseur _isGameStarted
 */
bool Application::partieCommencee(){
    return _isGameStarted;
}

/**
 * Rendu graphique de l'application
 */
void Application::dessiner(){
    if (_isGameStarted) {
        // On affiche le jeu
        _terrain->dessiner();
    } else {
        // On affiche le menu
        ImageRenderer::draw("background2.bmp", 0, 0, _longueur, _hauteur);
    }
}

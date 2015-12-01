//
//  Bombe.hpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#ifndef Bombe_hpp
#define Bombe_hpp

#include <stdio.h>

/*
 Classe Bombe:
 - utilisée par Vaisseau
 Défini par:
 - pos_x
 - pos_y
 */

class Bombe {
private:
    /**
     * variables d'instance
     */
    int _pos_x;// position dans l'axe des abscisses en pixels
    int _pos_y;// position dans l'axe des ordonnées en pixels
    int _direction;// direction dans lequel allait le vaisseau en lâchant la bombe (1/-1 <=> droite/gauche)
    int _nb_explosions;// nombre d'explosions, nombre de blocs détruits
public:
    Bombe(int pos_x, int pos_y, int direction);
    /**
     * méthodes d'instance
     */
    int getPosX();// accesseur _pos_x
    int getPosY();// accesseur _pos_y
    int getNbExplosions();// accesseur _nb_explosions
    void avancer(int pas);// avance la bombe de {pas} pixels
    int explose();// signale qu'on a explosé un bloc et retourne getNbExplosions()
    void affiche();
    void dessiner(int longueur);
    /**
     * constantes de classe
     */
    const static int nb_explosions_max;
};

#endif /* Bombe_hpp */

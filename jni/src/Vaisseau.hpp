//
//  Vaisseau.hpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#ifndef Vaisseau_hpp
#define Vaisseau_hpp

#include "Bombe.hpp"

class Vaisseau {
private:
    /**
     * variables d'instance
     */
    int _pos_x;// position dans l'axe des abscisses en pixels
    int _pos_y;// position dans l'axe des ordonnées en pixels
    int _direction;// 1 ou -1 : se déplace vers la droite ou vers la gauche
    int _pas;// distance en pixels qu'il parcoure en une fois
    int _x_min;// abscisse en pixels minimale où il peut se dessiner
    int _x_max;// abscisse en pixels maximale où il peut se dessiner
    int _longueur;// longueur du vaisseau (ie de l'image)
    int _hauteur;// hauteur du vaisseau (ie de l'image)
    int _score;// cumul des valeurs des blocs explosés
    bool _bombeChargee;// booléen qui permet ou non de lancer une bombe
    /**
     * méthodes d'instances
     */
    void debut_mouvement();
public:
    Vaisseau(int x_min_terrain, int x_max_terrain);
    /**
     * variables d'instance
     */
    Bombe* bombe;
    /**
     * méthodes d'instance
     */
    void gagner(int nb);// récompense l'explosion d'un bloc
    void avancer();// fait avancer le vaisseau (il gère sa vitesse comme un grand)
    void lacherBombe();// lâche une bombe si possible
    void rechargerBombe();// recharge une bombe pour permettre au vaisseau de tirer de nouveau
    void afficher();
};

#endif /* Vaisseau_hpp */

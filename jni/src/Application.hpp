//
//  Application.hpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

class Terrain; // juste la déclaration de la classe et rien d'autre

class Application {
private:
    bool _isGameStarted;
    Terrain* _terrain;
    int _longueur;
    int _hauteur;
public:
    Application(int w, int h);
    /**
     * méthodes d'instance
     */
    void click(int x, int y);
    void jouer();
    void endGame();
    void afficher();
    void dessiner();
    bool partieCommencee();
    /**
     * constantes de classe
     */
    const static int intervalle_rafraichir;
};

#endif /* Application_hpp */

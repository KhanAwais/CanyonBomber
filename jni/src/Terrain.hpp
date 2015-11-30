//
//  Terrain.hpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#ifndef Terrain_hpp
#define Terrain_hpp

#include "Vaisseau.hpp"
#include <vector>
class Application; // juste la déclaration de la classe et rien d'autre

using namespace std;

class Terrain {
private:
    /**
     * variables d'instances
     */
    int _longueur;// longueur de tout le terrain (ie de l'image)
    int _hauteur;// hauteur de tout le terrain (ie de l'image)
    int _x_pix_deb_blocs;// abscisse de depart où peut être positionné un bloc
    int _x_pix_fin_blocs;// abscisse de fin où ne peut plus être positionné de bloc
    int _y_pix_deb_blocs;// ordonnée de départ où peut être positionné un bloc (ligne d'en haut)
    int _y_pix_fin_blocs;// ordonnée de fin où ne peut plus être positionné de bloc (ligne d'en bas)
    int** blocs; // liste des valeurs posibles : >0 si bloc présent, 0 si vide, <0 si explosion en cours
    int _longueur_bloc;
    Application* _app;
    Vaisseau* _vaisseau; // TODO gérer le multijoueur
    vector<int> _explosions_en_cours;// position des blocs en cours d'explosion (pos_x*nb_blocs_abscisses + pos_y)
    /**
     * constante de classe
     */
    const static int nb_blocs_abscisses;
    const static int nb_blocs_ordonnees;
    /**
     * méthodes d'instances
     */
    void calcul_hauteur_vaisseau();
    void init_blocs();
    int yPixelsToYBloc(int yPix);
    int xPixelsToXBloc(int xPix);
    void gerer_explosions();
    void reorganise(int position);
public:
    Terrain(int w, int h, Application* app);
    /**
     * variables de classe
     */
    static int hauteur_min_vaisseau;// hauteur de départ des vaisseaux (le plus haut possible)
    static int hauteur_max_vaisseau;// hauteur jusqu'à laquelle les vaisseaux peuvent descendre, varie en explosant les blocs
    /**
     * méthodes d'instance
     */
    void jouer();
    void clicUtilisateur();
    // TODO ajouter une méthode repositionne(Vaisseau v*) qui repositionne un vaisseau en fin de ligne
    // elle sera utile lorsque le multi joueur sera géré
    void afficher();
};

#endif /* Terrain_hpp */

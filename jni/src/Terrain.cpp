//
//  Terrain.cpp
//  CanyonBomber
//
//  Created by Mac-Awais on 26/11/2015.
//  Copyright © 2015 Mac-Awais. All rights reserved.
//

#include "Terrain.hpp"
#include "Application.hpp"
#include <stdlib.h> // pour la fonction rand()
#include <iostream>
#include <algorithm> // Pour std::sort
#include "ImageRenderer.hpp"
#include <android/log.h> // Pour le log en natif vers logcat

using namespace std;

const int Terrain::nb_blocs_abscisses = 12;
const int Terrain::nb_blocs_ordonnees = 16;

// Ces deux valeurs seront mises à jour lorsqu'un Terrain sera instancié
int Terrain::hauteur_min_vaisseau = 0;
int Terrain::hauteur_max_vaisseau = 0;

Terrain::Terrain(int w, int h, Application* app){
    _longueur = w;
    _hauteur = h;
    _app = app;
    
    // ces deux variables serviront à l'instanciation des vaisseaux pour les limiter dans leur déplacement horizontal
    /**
     * TODO : Ici, il faudra modifier les coefficients multiplicateurs par ceux que j'aurai trouvé empiriquement
     * pour l'image du terrain que je choisirai
     */
    _x_pix_deb_blocs = w * 0.2;
    _x_pix_fin_blocs = w * 0.8;
    _y_pix_fin_blocs = h * 0.9;
    hauteur_min_vaisseau = h * 0.3;
    
    // on initialise la longueur des blocs (pour le rendu)
    _longueur_bloc = (_x_pix_fin_blocs - _x_pix_deb_blocs) / nb_blocs_abscisses;
    // ensuite on met à jour _x_pix_fin_blocs car le rapport calculé ci-dessus ne tombe pas forcément juste
    _x_pix_fin_blocs = _x_pix_deb_blocs + nb_blocs_abscisses * _longueur_bloc;

    // la position de la ligne la plus haute est toujours calculé dynamiquement
    _y_pix_deb_blocs = _y_pix_fin_blocs - (_longueur_bloc * nb_blocs_ordonnees);
    
    // on initialise la hauteur jusqu'à laquelle un vaisseau peut descendre
    hauteur_max_vaisseau = _y_pix_deb_blocs;
    
    // on intialise les blocs de points
    init_blocs();
    
    // on initialise le vaisseau
    _vaisseau = new Vaisseau(_x_pix_deb_blocs, _x_pix_fin_blocs);
    
    // on initialise le vector contenant la position des blocs en cours d'explosion
    _explosions_en_cours.clear();
}

/**
 * méthode qui met à jour hauteur_max_vaisseau au fur et à mesure que les blocs sont éliminés
 */
void Terrain::calcul_hauteur_vaisseau(){
    int y = yPixelsToYBloc(hauteur_max_vaisseau);

    // On parcourt le tableau pour voir si cette ligne contient toujours un bloc
    // Sinon on pourra augmenter hauteur_max_vaisseau
    int j;
    for (j=y; j<nb_blocs_ordonnees; j++) {
        int i = 0;
        while (i<nb_blocs_abscisses && blocs[i][j] == 0) {
            i++;
        }
        if (i != nb_blocs_abscisses) { // alors il reste qqch à cette ligne
            // donc on sort car le vaisseau ne doit pas descendre jusqu'à la ligne j
            break;
        }
    }
    hauteur_max_vaisseau = hauteur_max_vaisseau + (j-y)*_longueur_bloc;
    
    // Si la hauteur max correspond à la limite inférieure des blocs
    // alors c'est que tous les blocs ont été supprimés
    if(hauteur_max_vaisseau == _y_pix_fin_blocs){
        _app->endGame();
    }
}

/**
 * Méthode qui initialise les blocs (fait du random au niveau des points)
 */
void Terrain::init_blocs(){
    blocs = new int*[nb_blocs_abscisses];
    for (int i=0; i<nb_blocs_abscisses; i++) {
        blocs[i] = new int [nb_blocs_ordonnees];
        for (int j=0; j<nb_blocs_ordonnees; j++){
            blocs[i][j] = rand()%3 + 1;
        }
    }
}

/**
 * Fait la correspondance entre une abscisse en pixel et l'abscisse dans le tableau de bloc
 * Renvoie -1 si l'abscisse est hors de l'intervalle
 */
int Terrain::xPixelsToXBloc(int xPix){
    if(xPix < _x_pix_deb_blocs || xPix >= _x_pix_fin_blocs){
        return -1;
    }
    return (xPix - _x_pix_deb_blocs)/_longueur_bloc;
}



/**
 * Fait la correspondance entre une ordonnée en pixel et l'ordonnée dans le tableau de bloc
 * Renvoie -1 si l'ordonnée est hors de l'intervalle
 */
int Terrain::yPixelsToYBloc(int yPix){
    if(yPix >= _y_pix_fin_blocs || yPix < _y_pix_deb_blocs){
        return -1;
    }
    return (yPix - _y_pix_deb_blocs)/_longueur_bloc;
}

void Terrain::jouer(){
    _vaisseau->avancer();
    if(_vaisseau->bombe != nullptr){
        _vaisseau->bombe->avancer(_longueur_bloc);
        
        int bloc_x = xPixelsToXBloc(_vaisseau->bombe->getPosX());
        int bloc_y = yPixelsToYBloc(_vaisseau->bombe->getPosY());
        if (bloc_x > 0 && bloc_y > 0 && blocs[bloc_x][bloc_y] > 0) {
            _vaisseau->gagner(blocs[bloc_x][bloc_y]);
            blocs[bloc_x][bloc_y] = -4;
            _vaisseau->bombe->explose();
            
            // On ajoute à liste des explosions en cours
            _explosions_en_cours.push_back(bloc_x*nb_blocs_abscisses + bloc_y);
        }
        
        // On fait disparaître la bombe si elle est en dehors des limites ou si elle a explosé le maximum de blocs permis
        if (_vaisseau->bombe->getPosX() < _x_pix_deb_blocs || _vaisseau->bombe->getPosX() > _x_pix_fin_blocs ||
            _vaisseau->bombe->getPosY() > _y_pix_fin_blocs ||
            _vaisseau->bombe->getNbExplosions() >= Bombe::nb_explosions_max
            ) {
            if (_vaisseau->bombe->getNbExplosions() >= Bombe::nb_explosions_max) {
                std::cout << Bombe::nb_explosions_max << " bombes explosés" << std::endl;
            }else {
                std::cout << "La bombe sort du terrain" << std::endl;
            }
            _vaisseau->rechargerBombe();
        }
    }
    gerer_explosions();
}

void Terrain::clicUtilisateur(){
    _vaisseau->lacherBombe();
}

/**
 * Gère les explosions en cours et celles qu'on a fini d'afficher
 */
void Terrain::gerer_explosions(){
    vector<int> positions_blocs_a_reorganiser;
    
    for (int i=0; i<_explosions_en_cours.size(); i++) {
        int pos_x = _explosions_en_cours[i] / nb_blocs_abscisses;// indice en abscisse de l'explosion dans blocs
        int pos_y = _explosions_en_cours[i] % nb_blocs_abscisses;// indice en ordonnée de l'explosion dans blocs
        
        // Dans le tableau de blocs, pour ces coordonnées, le nombre est négatif pour qu'au moment où on dessine
        // on mette l'image de l'explosion
        // on veut faire apparaître cette image 3x c'est pour ça que je met pas direct à 0 lors de l'explosion
        blocs[pos_x][pos_y]++;
        
        // Dès qu'on arrive à 0, il faut réorganiser les blocs pour cette position
        if(blocs[pos_x][pos_y] == 0){
            positions_blocs_a_reorganiser.push_back(_explosions_en_cours[i]);
        }
    }
    
    // On trie par ordre croissant ces positions (pour éviter les problèmes si
    // on a deux positoins l'une au dessus de l'autre à réorganiser
    std::sort(positions_blocs_a_reorganiser.begin(), positions_blocs_a_reorganiser.end());
    
    // On parcoure les positions de bloc à réorganiser
    for (int i=0; i<positions_blocs_a_reorganiser.size(); i++) {
        // On supprime ces positions de la liste des explosions en cours
        _explosions_en_cours.erase(
            std::remove(_explosions_en_cours.begin(), _explosions_en_cours.end(), positions_blocs_a_reorganiser[i])
            , _explosions_en_cours.end()
        );
        // On réordonne les blocs pour ces positions
        reorganise(positions_blocs_a_reorganiser[i]);
        
        // Si on est au dernier à remettre en ordre, alors on met aussi à jour la hauteur max des vaisseaux
        if (i == positions_blocs_a_reorganiser.size() - 1) {
            calcul_hauteur_vaisseau();
        }
    }
}

void Terrain::reorganise(int position){
    int pos_x = position / nb_blocs_abscisses;// indice en abscisse de l'explosion dans blocs
    int pos_y = position % nb_blocs_abscisses;// indice en ordonnée de l'explosion dans blocs
    
    vector<int>::iterator it;
    
    // on fait descendre les blocs situés au dessus
    for (int i=pos_y-1; i>=0; i--) {
        // on swap
        int tmp = blocs[pos_x][i];
        blocs[pos_x][i] = blocs[pos_x][i+1];
        blocs[pos_x][i+1] = tmp;
        
        // au passage, on met à jour les positions en cours d'explosion stockées
        it = std::find(_explosions_en_cours.begin(), _explosions_en_cours.end(), pos_x*nb_blocs_abscisses+i);
        if (it != _explosions_en_cours.end()) { // la position a été trouvée
            *it = *it + 1;
        }
    }
}

void Terrain::afficher(){
    
    cout << endl << "Terrain { _x_pix_deb_blocs=" << _x_pix_deb_blocs << ", _x_pix_fin_blocs=" << _x_pix_fin_blocs;
    cout << ", _y_pix_deb_blocs=" << _y_pix_deb_blocs << ", _y_pix_fin_blocs=" << _y_pix_fin_blocs;
    cout << ", hauteur_max_vaisseau=" << hauteur_max_vaisseau;
    cout << ", _longueur_bloc=" << _longueur_bloc << " }" << endl;
    
    for (int i=0; i<nb_blocs_ordonnees; i++) {
        cout << "[";
        for (int j=0; j<nb_blocs_abscisses; j++) {
            cout << blocs[j][i] << " ";
        }
        cout << "]" << endl;
    }
    
    //_vaisseau->afficher();
}

void Terrain::dessiner(){
    // On affiche le terrain
    ImageRenderer::draw("terrain.bmp", 0, 0, _longueur, _hauteur);

    // On affiche le vaisseau
    _vaisseau->dessiner();
    
    // On affiche la bombe du vaisseau si elle est défini
    if (_vaisseau->bombe != nullptr) {
        // On donne la longueur du carré de la bombe : 1/4 des blocs à exploser
        _vaisseau->bombe->dessiner(_longueur_bloc/4);
    }

    // On affiche les blocs
    for (int i=0; i<nb_blocs_abscisses; i++) {
        for (int j=0; j<nb_blocs_ordonnees; j++) {
            switch (blocs[i][j]) {
                case 1:
                    ImageRenderer::draw(
                        "bloc1.bmp",
                        _x_pix_deb_blocs + i*_longueur_bloc,
                        _y_pix_deb_blocs + j*_longueur_bloc,
                        _longueur_bloc,
                        _longueur_bloc
                    );
                    break;
                case 2:
                    ImageRenderer::draw(
                                        "bloc2.bmp",
                                        _x_pix_deb_blocs + i*_longueur_bloc,
                                        _y_pix_deb_blocs + j*_longueur_bloc,
                                        _longueur_bloc,
                                        _longueur_bloc
                                        );
                    break;
                case 3:
                    ImageRenderer::draw(
                                        "bloc3.bmp",
                                        _x_pix_deb_blocs + i*_longueur_bloc,
                                        _y_pix_deb_blocs + j*_longueur_bloc,
                                        _longueur_bloc,
                                        _longueur_bloc
                                        );
                    break;
                case -4:
                    ImageRenderer::draw(
                                        "explosion-4.bmp",
                                        _x_pix_deb_blocs + i*_longueur_bloc,
                                        _y_pix_deb_blocs + j*_longueur_bloc,
                                        _longueur_bloc,
                                        _longueur_bloc
                                        );
                    break;
                case -3:
                    ImageRenderer::draw(
                                        "explosion-3.bmp",
                                        _x_pix_deb_blocs + i*_longueur_bloc,
                                        _y_pix_deb_blocs + j*_longueur_bloc,
                                        _longueur_bloc,
                                        _longueur_bloc
                                        );
                    break;
                case -2:
                    ImageRenderer::draw(
                                        "explosion-2.bmp",
                                        _x_pix_deb_blocs + i*_longueur_bloc,
                                        _y_pix_deb_blocs + j*_longueur_bloc,
                                        _longueur_bloc,
                                        _longueur_bloc
                                        );
                    break;
                case -1:
                    ImageRenderer::draw(
                                        "explosion-1.bmp",
                                        _x_pix_deb_blocs + i*_longueur_bloc,
                                        _y_pix_deb_blocs + j*_longueur_bloc,
                                        _longueur_bloc,
                                        _longueur_bloc
                                        );
                    break;
                    
                default:
                    break;
            }
        }
    }
}

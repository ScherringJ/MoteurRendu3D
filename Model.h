#ifndef _MODEL_H__
#define _MODEL_H__

#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "struct.h"

class Model {

    private:

        std::vector<Vecteur> vecteurs;
        std::vector<Face> faces;
        std::vector<Pointf> coord_textures;
        std::vector<Vecteur> coord_textures_normal;

    public:

        /**
         * Constructeur de Model, prend le chemin relatif d'un fichier .obj et le lis/stocke les valeurs
         **/
        Model(std::string file_path);

        /**
         * Methode qui renvoie la liste des vecteurs
         **/
        std::vector<Vecteur> get_vecteurs();

        /**
         * Methode qui renvoie la liste des faces
         **/
        std::vector<Face> get_faces();

        /**
         * Methode qui renvoie le liste des coordonnees de textures
        **/
        std::vector<Pointf> get_coord_textures();

         /**
         * Methode qui renvoie le liste des coordonnees de la normal
        **/
        std::vector<Vecteur> get_coord_textures_normal();

};

#endif
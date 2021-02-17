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
        Model(std::string name_file);

        std::vector<Vecteur> get_vecteurs();
        std::vector<Face> get_faces();
        std::vector<Pointf> get_coord_textures();
        std::vector<Vecteur> get_coord_textures_normal();

};

#endif
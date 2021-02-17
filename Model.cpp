#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Model.h"
#include "struct.h"


Model::Model(std::string name_file) : vecteurs(), faces(), coord_textures(), coord_textures_normal() {

    std::ifstream fichier("obj/" + name_file + "/" + name_file + ".obj", std::ios::in);
    std::string line;
    while (!fichier.eof()){
        getline(fichier,line);
        std::istringstream iss(line);
        std::string temp;

        if (!line.compare(0, 2, "v ")){
            float v[3];
            iss >> temp;
            for (int i = 0; i < 3; i++) {
                iss >> v[i];
            }

            vecteurs.push_back(Vecteur(v[0], v[1], v[2]));

        } else if (!line.compare(0, 2, "f ")) {
            Face f;
            float idx, idx2;
            char trash;
            iss >> temp;
            for ( int i = 0; i < 3; i++) {
                iss >> idx >> trash >> idx2 >> temp;
                idx--;
                idx2--;
                f.coord[i] = idx;
                f.text_coord[i] = idx2;
            }
            
            faces.push_back(f);
            
        } else if (!line.compare(0, 2, "vt")) {

            float idx, idx2;
            iss >> temp >> idx >> idx2;
            Pointf uv(idx, idx2);
            coord_textures.push_back(uv);

        } else if (!line.compare(0, 2, "vn")) {
            Vecteur normal;
            iss >> temp;
            for (int i = 0; i < 3; i++){
                iss >> normal.vec[i];
            }
            coord_textures_normal.push_back(normal);

        }
    }
}

std::vector<Vecteur> Model::get_vecteurs()
{
    return vecteurs;
}

std::vector<Face> Model::get_faces()
{
    return faces;
}

std::vector<Pointf> Model::get_coord_textures()
{
    return coord_textures;
}

std::vector<Vecteur> Model::get_coord_textures_normal()
{
    return coord_textures_normal;
}
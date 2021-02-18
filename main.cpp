#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "struct.h"
#include "Matrix.h"
#include "Model.h"
#include "Draw.h"

const int width = 800;
const int height = 800;
const int depth = 255;

Vecteur camera(0, 0, 3);
Vecteur light(0, 0, 1);
Vecteur eye(-1, 1, 3);
Vecteur center(0, 0, 0);
Vecteur up(0, 1, 0);



Matrix vecteurToMatrix(Vecteur v) {
    Matrix m(4,1);
    m(0,0) = v.x;
    m(1,0) = v.y;
    m(2,0) = v.z;
    m(3,0) = 1;

    return m;
}

Vecteur matrixToVecteur(Matrix m) {

    return Vecteur(m(0,0)/m(3,0), m(1,0)/m(3,0), m(2,0)/m(3,0));
}


int main(int argc, char** argv) {

	TGAImage image(width, height, TGAImage::RGB);
    std::string file = "african_head";
    
    TGAImage texture;
    texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
    texture.flip_vertically();

    TGAImage textureNormal;
    textureNormal.read_tga_file("obj/african_head/african_head_nm.tga");
    textureNormal.flip_vertically();
  
    Model model(file);
    Draw draw(width, height, depth);
    
    std::vector<Vecteur> vecteurs = model.get_vecteurs();
    std::vector<Face> faces = model.get_faces();
    std::vector<Pointf> coord_textures = model.get_coord_textures();
    std::vector<Vecteur> coord_textures_normal = model.get_coord_textures_normal();

    float *zbuffer = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    };

    Matrix modelview = draw.lookat(eye, center, up);
    Matrix projection = draw.projection(eye, center);
    Matrix perspects = draw.perspect(width/8, height/8, width*3/4, height*3/4);
    
    float norm = light.norm();
    Vecteur n = light/norm; 

    for (int i = 0; i < faces.size(); i++) {
        Face f = faces.at(i);
        Vecteur triangle_coord[3];
        Vecteur coord[3];
        
        
        for (int j=0; j<3; j++) {

            Vecteur v = vecteurs.at(f.coord[j]);
            triangle_coord[j] = matrixToVecteur(perspects * projection * modelview * vecteurToMatrix(v));
            coord[j] = v;
                
        }

            Vecteur uv[3];
            Vecteur varying_intensity;
            for (int j = 0; j < 3; j++) {

                int index = f.text_coord[j];
                int indexn = f.text_coord_norm[j];
                Vecteur coord_norm = coord_textures_normal.at(indexn);
                norm = coord_norm.norm();
                Vecteur val = coord_norm/ norm;

                varying_intensity.vec[j] = val^n;
                uv[j] = Vecteur( coord_textures.at(index).x * texture.get_width(), coord_textures.at(index).y * texture.get_height(), 0.f);
                
            }
            draw.triangle(triangle_coord, zbuffer, uv,  image, texture, varying_intensity);
           } 

        


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
    
    

	return 0;
}

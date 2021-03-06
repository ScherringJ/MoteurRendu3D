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
Vecteur eye(1, 1, 3);
Vecteur center(0, 0, 0);
Vecteur up(0, 1, 0);

Matrix modelview(4,4);
Matrix projection(4,4);
Matrix perspects(4,4);


/**
* Methode pour passer un vecteur sous forme de matrice
**/
Matrix vecteurToMatrix(Vecteur v) {
    Matrix m(4,1);
    m(0,0) = v.x;
    m(1,0) = v.y;
    m(2,0) = v.z;
    m(3,0) = 1;

    return m;
}

/**
 * Methode pour passer une matrice sous forme de vecteur
**/
Vecteur matrixToVecteur(Matrix m) {

    return Vecteur(m(0,0)/m(3,0), m(1,0)/m(3,0), m(2,0)/m(3,0));
}

/**
 * Méthode qui parcour les faces et les dessines
**/
void render(Draw draw, Model model, TGAImage texture, TGAImage &image) {

    float norm = light.norm();
    Vecteur n = light/norm; 

    for (int i = 0; i < model.get_faces().size(); i++) {
        Face f = model.get_faces().at(i);
        Vecteur triangle_coord[3];
        Vecteur coord[3];
        
        
        for (int j=0; j<3; j++) {

            Vecteur v = model.get_vecteurs().at(f.coord[j]);
            triangle_coord[j] = matrixToVecteur(perspects * projection * modelview * vecteurToMatrix(v));
            coord[j] = v;
                
        }

            Vecteur uv[3];
            Vecteur varying_intensity;
            for (int j = 0; j < 3; j++) {

                int index = f.text_coord[j];
                int indexn = f.text_coord_norm[j];
                Vecteur coord_norm = model.get_coord_textures_normal().at(indexn);
                norm = coord_norm.norm();
                Vecteur val = coord_norm/ norm;

                varying_intensity.vec[j] = val^n;
                uv[j] = Vecteur( model.get_coord_textures().at(index).x * texture.get_width(), model.get_coord_textures().at(index).y * texture.get_height(), 0.f);
                
            }
            draw.triangle(triangle_coord, uv,  image, texture, varying_intensity);
    }


}


int main(int argc, char** argv) {

	TGAImage image(width, height, TGAImage::RGB);
    std::string file = "obj/african_head/african_head.obj";
    std::string file2 = "obj/african_head/african_head_eye_outer.obj";
    std::string file3 = "obj/african_head/african_head_eye_inner.obj";
    
    TGAImage texture;
    texture.read_tga_file("obj/african_head/african_head_diffuse.tga");
    texture.flip_vertically();

    TGAImage textureOeil;
    textureOeil.read_tga_file("obj/african_head/african_head_eye_outer_diffuse.tga");
    textureOeil.flip_vertically();

    TGAImage textureOeilInner;
    textureOeilInner.read_tga_file("obj/african_head/african_head_eye_inner_diffuse.tga");
    textureOeilInner.flip_vertically();
  
    Model model(file);
    Model oeil(file2);
    Model oeilI(file3);
    
    Draw draw(width, height, depth);

    modelview = draw.lookat(eye, center, up);
    projection = draw.projection(eye, center);
    perspects = draw.perspect(width/8, height/8, width*3/4, height*3/4);

    render(draw, model, texture, image);
    render(draw, oeilI, textureOeilInner, image);
    //render(draw, oeil, textureOeil, image);                       problème 
   
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output_african_head.tga");
    draw.resetZbuffer();

    TGAImage image2(width, height, TGAImage::RGB);
    file = "obj/diablo3_pose/diablo3_pose.obj";


    texture.read_tga_file("obj/diablo3_pose/diablo3_pose_diffuse.tga");
    texture.flip_vertically();

    model = Model(file);

    render(draw, model, texture, image2);

    image2.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image2.write_tga_file("output_diablo3_pose.tga");
    draw.resetZbuffer();
    

    TGAImage image3(width, height, TGAImage::RGB);
    file = "obj/boggie/body.obj";
    file2 = "obj/boggie/eyes.obj";
    file3 = "obj/boggie/head.obj";

    texture.read_tga_file("obj/boggie/body_diffuse.tga");
    texture.flip_vertically();

    TGAImage texture2;
    texture2.read_tga_file("obj/boggie/eyes_diffuse.tga");
    texture2.flip_vertically();

    TGAImage texture3;
    texture3.read_tga_file("obj/boggie/head_diffuse.tga");
    texture3.flip_vertically();

    model = Model(file);
    Model eye(file2);
    Model head(file3);

    render(draw, model, texture, image3);
    render(draw, eye, texture2, image3);
    render(draw, head, texture3, image3);

    image3.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image3.write_tga_file("output_boggie.tga");

	return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "struct.h"
#include "Matrix.h"
#include "Model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const int width = 800;
const int height = 800;
const int depth = 255;

Vecteur camera(0, 0, 3);
Vecteur light(0, 0, -1);
Vecteur eye(0, 0, 3);
Vecteur center(0, 0, 0);
Vecteur up(0, 1, 0);

Matrix perspect(int x, int y, int w, int h) {
    Matrix m = Matrix::identite(4);
    m(0, 3) = x+w/2.f;
    m(1, 3) = y+h/2.f;
    m(2, 3) = depth/2.f;

    m(0, 0) = w/2.f;
    m(1, 1) = h/2.f;
    m(2, 2) = depth/2.f;

    return m;
}

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

Matrix lookat(Vecteur eye, Vecteur center, Vecteur up) {

    Vecteur z = eye - center;
    float znorm = z.norm();
    z = z/znorm;

    Vecteur x = up | z;
    float xnorm = x.norm();
    x = x/xnorm;

    Vecteur y = z | x;
    float ynorm = y.norm();
    y = y/ynorm;

    Matrix Minv = Matrix::identite(4);
    Matrix Tr = Matrix::identite(4);

    for (int i = 0; i < 3; i++) {

        Minv(0, i) = x.vec[i];
        Minv(1, i) = y.vec[i];
        Minv(2, i) = z.vec[i];

        Tr(i, 3) = -center.vec[i];

    }

    return Minv * Tr;

}

void lines(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    
    bool steep = false;
    if ( std::abs(x0-x1) < std::abs(y0-y1) ) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if ( x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1-x0;
    int dy = y1-y0;

    int derror = std::abs(dy)*2;
    int error = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);

        } else {
            image.set(x, y, color);

        }
        error += derror;
        if (error > dx) {
            y += ( y1 > y0 ? 1 : -1);
            error -= dx*2;
        } 
    }


}


Vecteur barycentric(Vecteur *pts,  Pointi P) {
   
    Vecteur temp1(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
    Vecteur temp2(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
    
    //cross
    Vecteur u = temp1 | temp2;

    return Vecteur(1.f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z); 

}

void triangle(Vecteur *pts, float *zbuffer, Vecteur *pts_texture, TGAImage &image, TGAImage &texture, float intensity) {

    Pointf boxmin((float)(image.get_width() - 1), (float)(image.get_height() -1));
    Pointf boxmax;
    Pointf clamp((float)(image.get_width() - 1), (float)(image.get_height() -1));
    

    for (int i = 0; i < 3; i++) {
        boxmin.x = std::max(0.f, std::min(boxmin.x, pts[i].x));
        boxmin.y = std::max(0.f, std::min(boxmin.y, pts[i].y));

        boxmax.x = std::min(clamp.x, std::max(boxmax.x, pts[i].x));
        boxmax.y = std::min(clamp.y, std::max(boxmax.y, pts[i].y));
    }

    Pointi P;
    float Pz;
    for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
        for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
            Vecteur bary = barycentric(pts, P);

            
            
            if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;
            Pz = 0;

            for (int i = 0; i < 3; i++) {
                Pz += pts[i].z * bary.vec[i];
            }

            Pointi uv;
            uv.x = pts_texture[0].x * bary.x + pts_texture[1].x * bary.y + pts_texture[2].x * bary.z;
            uv.y = pts_texture[0].y * bary.x + pts_texture[1].y * bary.y + pts_texture[2].y * bary.z;

            if (zbuffer[int(P.x + P.y*width)] < Pz) {
                zbuffer[int(P.x + P.y*width)] = Pz;

                TGAColor color = texture.get(uv.x, uv.y);                
                image.set(P.x, P.y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity, 255));
            }


        }


    }
    
   
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
    
    std::vector<Vecteur> vecteurs = model.get_vecteurs();
    std::vector<Face> faces = model.get_faces();
    std::vector<Pointf> coord_textures = model.get_coord_textures();
    std::vector<Vecteur> coord_textures_normal = model.get_coord_textures_normal();


    float *zbuffer = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    };

    
    Matrix modelview = lookat(eye, center, up);
    Matrix projection = Matrix::identite(4);
    Matrix perspects = perspect(width/8, height/8, width*3/4, height*3/4);
    projection(3, 2) = -1.f/ (eye-center).norm() ;

    
    for (int i = 0; i < faces.size(); i++) {
        Face f = faces.at(i);
        Vecteur triangle_coord[3];
        Vecteur coord[3];
       
           
        for (int j=0; j<3; j++) {

            Vecteur v = vecteurs.at(f.coord[j]);
            Vecteur p((v.vec[0]+1.)*800/2., (v.vec[1]+1.)*800/2, 0);
                
            Matrix tempMatrix = perspects * projection * modelview * vecteurToMatrix(v);

            triangle_coord[j] = matrixToVecteur(tempMatrix);
            coord[j] = v;
                
        }

        Vecteur temp1 = coord[1] - coord[0];
        Vecteur temp2 = coord[2] - coord[1];
        Vecteur normal = temp2 | temp1;

        float norm = normal.norm();
        Vecteur n = normal/norm; 

           
        Vecteur intens = n * light;
        float intensity = intens.x + intens.y + intens.z;
           

        if (intensity > 0) {
            Vecteur uv[3];
            for (int j = 0; j < 3; j++) {
              int index = f.text_coord[j];
              uv[j] = Vecteur( coord_textures.at(index).x * texture.get_width(), coord_textures.at(index).y * texture.get_height(), 0.f);
            }
            triangle(triangle_coord, zbuffer, uv,  image, texture, intensity);
           } 
        } 




	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
    
    

	return 0;
}

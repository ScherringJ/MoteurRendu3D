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


Draw::Draw( int w,  int h,  int d) : width(w), height(h), depth(d) 
{
    zbuffer = new float[width*height];
    for (int i = 0; i < width*height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    };
}


Vecteur Draw::barycentric(Vecteur *pts,  Pointi P) 
{

    Vecteur temp1(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
    Vecteur temp2(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
    
    //cross
    Vecteur u = temp1 | temp2;

    return Vecteur(1.f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z); 

}

bool Draw::fragment(Vecteur bary, TGAColor &color, Vecteur varying_intensity, TGAColor colorText) {

        float intensity = varying_intensity^bary;  
        color = colorText*intensity; 
        return false;  
}

void Draw::triangle(Vecteur *pts, Vecteur *pts_texture, TGAImage &image, TGAImage &texture, Vecteur varying_intensity) {

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

            TGAColor color;
            TGAColor colorText = texture.get(uv.x, uv.y);
            bool discard = fragment(bary, color, varying_intensity, colorText);

            if (!discard) {

                if (zbuffer[int(P.x + P.y*width)] < Pz) {
                    zbuffer[int(P.x + P.y*width)] = Pz;
                    image.set(P.x, P.y, color);
                }
            }
        }
    }
}

Matrix Draw::perspect(int x, int y, int w, int h) {

    Matrix m = Matrix::identite(4);
    m(0, 3) = x+w/2.f;
    m(1, 3) = y+h/2.f;
    m(2, 3) = depth/2.f;

    m(0, 0) = w/2.f;
    m(1, 1) = h/2.f;
    m(2, 2) = depth/2.f;

    return m;
    
}

Matrix Draw::lookat(Vecteur eye, Vecteur center, Vecteur up) {

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

Matrix Draw::projection(Vecteur eye, Vecteur center) {
    
    Matrix projection = Matrix::identite(4);
    projection(3, 2) = -1.f/ (eye-center).norm() ;

    return projection;
}

void Draw::resetZbuffer() {
    
    for (int i = 0; i < width*height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    };
}
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
    
}


Vecteur Draw::barycentric(Vecteur *pts,  Pointi P) 
{

    Vecteur temp1(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
    Vecteur temp2(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
    
    //cross
    Vecteur u = temp1 | temp2;

    return Vecteur(1.f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z); 

}

void Draw::triangle(Vecteur *pts, float *zbuffer, Vecteur *pts_texture, TGAImage &image, TGAImage &texture, float intensity) {

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
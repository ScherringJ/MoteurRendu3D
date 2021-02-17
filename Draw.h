#ifndef _DRAW_H__
#define _DRAW_H__

#include "struct.h"
#include "Matrix.h"

class Draw
{
private:
   
   const int width, height, depth;
   Vecteur barycentric(Vecteur *pts,  Pointi P);


public:
   
   Draw(const int witdh,const int height, const int depth);
   void triangle(Vecteur *pts, float *zbuffer, Vecteur *pts_texture, TGAImage &image, TGAImage &texture, float intensity);
   Matrix perspect(int x, int y, int w, int h);
   Matrix lookat(Vecteur eye, Vecteur center, Vecteur up);
    
};





#endif
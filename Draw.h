#ifndef _DRAW_H__
#define _DRAW_H__

#include "struct.h"

class Draw
{
private:
   
   const int width, height, depth;
   Vecteur barycentric(Vecteur *pts,  Pointi P);


public:
   
   Draw(const int witdh,const int height, const int depth);
   void triangle(Vecteur *pts, float *zbuffer, Vecteur *pts_texture, TGAImage &image, TGAImage &texture, float intensity);
    
};





#endif
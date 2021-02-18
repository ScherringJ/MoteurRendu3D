#ifndef _DRAW_H__
#define _DRAW_H__

#include "struct.h"
#include "Matrix.h"

class Draw
{
private:
   
   const int width, height, depth;
   float *zbuffer;
   /**
    * Methode barycentrique :  retourne la barycentre.
   **/
   Vecteur barycentric(Vecteur *pts,  Pointi P);

   /**
    * Méthode qui donne la couleur en fonction du barycentre, de la variation d'intensite et de la couleur reçu
    **/
   void fragment(Vecteur bary, TGAColor &color, Vecteur varying_intensity, TGAColor colorText);


public:
   
   /**
    * Constructeur de la class Draw
    **/
   Draw(const int witdh,const int height, const int depth);

   /**
    * Methode qui dessine un triangle en fonction des parametre reçu
    **/
   void triangle(Vecteur *pts, Vecteur *pts_texture, TGAImage &image, TGAImage &texture, Vecteur varying_intensity);

   /**
    * Methode qui renvoie le matrice viewport 
    **/
   Matrix perspect(int x, int y, int w, int h);

   /**
    * Methode qui renvoie la matrice lookat
    **/
   Matrix lookat(Vecteur eye, Vecteur center, Vecteur up);

   /**
    * Methode qui renvoie la matrice de projection
    **/
   Matrix projection(Vecteur eye, Vecteur center);

   /**
    * Remet la zbuffer à zéro
    **/
   void resetZbuffer();
    
};





#endif
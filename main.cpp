#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "tgaimage.h"




const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

struct vecteur
{
    float vec[3];
};

struct face
{
    int faces[3];
};

struct point
{
    int x;
    int y;
};


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

void triangle(point t0, point t1, point t2,  TGAImage &image, TGAColor color) {

    if (t0.y > t1.y) 
        std::swap(t0, t1);
    if (t1.y > t2.y)
        std::swap(t1, t2);
    if (t0.y > t1.y)
        std::swap(t0, t1);

    int hauteur = t2.y - t0.y;

    for (int y = t0.y; y < t1.y; y++)
    {
       int hauteur_seg = t1.y - t0.y + 1;

       float alpha = (float)(y-t0.y)/hauteur;
       float beta = (float)(y-t0.y)/hauteur_seg; 

       int alpha_x = t0.x + (t2.x - t0.x) * alpha;
       int beta_x = t0.x + (t1.x - t0.x) * beta; 

        image.set(alpha_x, y, red);
        image.set(beta_x, y, green);
    }

    for (int y = t1.y ; y < t2.y; y++)
    {
        int hauteur_seg = t2.y - t1.y + 1;

        float alpha = (float)(y-t0.y)/hauteur;
        float beta = (float)(y-t1.y)/hauteur_seg;

        int alpha_x = t0.x + (t2.x - t0.x) * alpha;
        int beta_x = t1.x + (t2.x - t1.x) * beta;

        image.set(alpha_x, y, red);
        image.set(beta_x, y, green);
    }
    
    


   
}

int main(int argc, char** argv) {
	TGAImage image(800, 800, TGAImage::RGB);

    /*
    std::ifstream fichier("obj/african_head/african_head.obj", std::ios::in);

    std::string line;
    std::vector<vecteur> vecteurs;
    std::vector<face> faces;
    
    while (!fichier.eof()){
        getline(fichier,line);
        std::istringstream iss(line);
        std::string temp;

        if (!line.compare(0, 2, "v ")){
            vecteur v;
            iss >> temp;
            for (int i = 0; i < 3; i++) {
                iss >> v.vec[i];
            }
            vecteurs.push_back(v);

        } else if (!line.compare(0, 2, "f ")) {
            face f;
            int idx;
            iss >> temp;
            for ( int i = 0; i < 3; i++) {
                iss >> idx >> temp;
                idx--;
                f.faces[i] = idx;
            }
            
            faces.push_back(f);
            
        }


        for (int i = 0; i < faces.size(); i++) {
            face f = faces.at(i);

            for (int j=0; j<3; j++) {
                vecteur v0 = vecteurs.at(f.faces[j]);
                vecteur v1 = vecteurs.at(f.faces[(j+1)%3]);

                int x0 = (v0.vec[0]+1.)*800/2.;
                int y0 = (v0.vec[1]+1.)*800/2.;

                int x1 = (v1.vec[0]+1.)*800/2.;
                int y1 = (v1.vec[1]+1.)*800/2.;

                lines(x0, y0, x1, y1, image, white);
            }
        } 

        

    } */

        point t0;
        t0.x = 180;
        t0.y = 150; 

        point t1;
        t1.x = 650;
        t1.y = 300;

        point t2;
        t2.x = 270;
        t2.y = 580;
        
        triangle(t0, t1, t2, image, white);

    
    

    
   //TODO: Création d'une classe pour les vecteurs/faces.
    

    

    


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "tgaimage.h"




const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const int width = 800;
const int height = 800;

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

vecteur barycentric(point *pts,  point P) {
    vecteur u, temp1, temp2;

    temp1.vec[0] = pts[2].x - pts[0].x;
    temp1.vec[1] = pts[1].x - pts[0].x;
    temp1.vec[2] = pts[0].x - P.x;

    temp2.vec[0] = pts[2].y - pts[0].y;
    temp2.vec[1] = pts[1].y - pts[0].y;
    temp2.vec[2] = pts[0].y - P.y;

    //cross

    u.vec[0] = temp1.vec[1] * temp2.vec[2] - temp1.vec[2] * temp2.vec[1];
    u.vec[1]=  temp1.vec[2] * temp2.vec[0] - temp1.vec[0] * temp2.vec[2];
    u.vec[2] = temp1.vec[0] * temp2.vec[1] - temp1.vec[1] * temp2.vec[0];

    
    
    vecteur retour; 
    if (std::abs(u.vec[2]) < 1) {
        retour.vec[0] = -1;
        retour.vec[1] = 1;
        retour.vec[2] = 1;

    } else {
        retour.vec[0] = 1.f - (u.vec[0] + u.vec[1])/u.vec[2];
        retour.vec[1] = u.vec[1]/u.vec[2];
        retour.vec[2] = u.vec[0]/u.vec[2];
    }


    return retour;
}

void triangle(point *pts,  TGAImage &image, TGAColor color) {

    point boxmin;
    boxmin.x = image.get_width() - 1;
    boxmin.y = image.get_height() -1;

    point boxmax;
    boxmax.x = 0;
    boxmax.y = 0;

    point clamp;
    clamp.x = image.get_width() - 1;
    clamp.y = image.get_height() -1;


    for (int i = 0; i < 3; i++) {
        boxmin.x = std::max(0, std::min(boxmin.x, pts[i].x));
        boxmin.y = std::max(0, std::min(boxmin.y, pts[i].y));

        boxmax.x = std::min(clamp.x, std::max(boxmax.x, pts[i].x));
        boxmax.y = std::min(clamp.y, std::max(boxmax.y, pts[i].y));
    }

    point P;
    for (P.x = boxmin.x; P.x <= boxmax.x; P.x++) {
        for (P.y = boxmin.y; P.y <= boxmax.y; P.y++) {
            vecteur bary = barycentric(pts, P);
            
   
            if (bary.vec[0] < 0 || bary.vec[1] < 0 || bary.vec[2] < 0) continue;
            image.set(P.x, P.y, color);

        }


    }
    
   
}


int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);

    
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
    }

       
        for (int i = 0; i < faces.size(); i++) {
            face f = faces.at(i);
            point triangle_coord[3];
            vecteur coord[3];
            for (int j=0; j<3; j++) {
                vecteur v = vecteurs.at(f.faces[j]);
                point p;

                p.x = (v.vec[0]+1.)*800/2.;
                p.y = (v.vec[1]+1.)*800/2.;
                triangle_coord[j] = p;
                coord[j] = v;
                
            }



            vecteur temp1;
            temp1.vec[0] =   coord[1].vec[0] - coord[0].vec[0];
            temp1.vec[1] =   coord[1].vec[1] - coord[0].vec[1];
            temp1.vec[2] =   coord[1].vec[2] - coord[0].vec[2];

            vecteur temp2;
            temp2.vec[0] =   coord[2].vec[0] - coord[0].vec[0];
            temp2.vec[1] =   coord[2].vec[1] - coord[0].vec[1];
            temp2.vec[2] =   coord[2].vec[2] - coord[0].vec[2];

            
            vecteur normal;
            normal.vec[0] = temp2.vec[1] * temp1.vec[2] - temp2.vec[2] * temp1.vec[1];
            normal.vec[1] = temp2.vec[2] * temp1.vec[0] - temp2.vec[0] * temp1.vec[2];
            normal.vec[2] = temp2.vec[0] * temp1.vec[1] - temp2.vec[1] * temp1.vec[0];

            float norm = std::sqrt(normal.vec[0] * normal.vec[0] + normal.vec[1] * normal.vec[1] + normal.vec[2] * normal.vec[2]);

            vecteur n; 
            n.vec[0] = normal.vec[0]/norm;
            n.vec[1] = normal.vec[1]/norm;
            n.vec[2] = normal.vec[2]/norm;


           vecteur light;
           light.vec[0] = 0;
           light.vec[1] = 0;
           light.vec[2] = -1;

           float intensity = n.vec[0] * light.vec[0] + n.vec[1] * light.vec[1] + n.vec[2] * light.vec[2];
           

           if (intensity > 0) {
            triangle(triangle_coord, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
           } 
        } 

        

    

    //TODO: Création d'une classe pour les vecteurs/faces.



	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

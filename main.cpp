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
   std::string vec[3];
};

struct face
{
    int faces[3];
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

int main(int argc, char** argv) {
	TGAImage image(800, 800, TGAImage::RGB);

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
            std::cout << f.faces[0] << "|" << f.faces[1] << "|" << f.faces[2] << std::endl;
        }

        

    }

    
    

    
   
    

    

    


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

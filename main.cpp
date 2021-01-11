#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "tgaimage.h"

using namespace std;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

struct vecteur
{
    string vec[3];
};


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    
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
	TGAImage image(100, 100, TGAImage::RGB);
	
    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, blue);

    ifstream fichier("obj/african_head/african_head.obj", ios::in);

    string line;
    vector<vecteur> vecteurs;
    
    while (!fichier.eof()){
        getline(fichier,line);

        if (!line.compare(0, 2, "v ")){
            istringstream iss(line);
            string temp;
            vecteur v;
            iss >> temp;
            iss >> v.vec[0];
            iss >> v.vec[1];
            iss >> v.vec[2];
            vecteurs.push_back(v);
        }
    }

    for (int i = 0; i < vecteurs.size(); i++)
    {
        cout << vecteurs[i].vec[0] << vecteurs[i].vec[1] << vecteurs[i].vec[2] << endl;
    }
    

    
   
    

    

    


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;
}

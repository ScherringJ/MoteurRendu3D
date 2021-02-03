#ifndef _STRUCT_H__
#define _STRUCT_H__

#include <cmath>

struct Vecteur
{
    union
    {
        struct { float x, y, z;};
        float vec[3];
    };
    
    
    Vecteur() : x(0), y(0), z(0) {}
    Vecteur(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

    Vecteur operator -(Vecteur &vec) { 
        return Vecteur(x - vec.x, y - vec.y, z - vec.z);
    }

    Vecteur operator /(float &num) {
        return Vecteur(x/num, y/num, z/num);
    }

    Vecteur operator *(Vecteur &vec) {
        return Vecteur( x * vec.x, y * vec.x, z * vec.z);
    }

    Vecteur operator |(Vecteur &vec) {
        return Vecteur(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
    }

    float norm() {
        return std::sqrt(x * x + y * y + z * z);
    }



};



#endif
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
    
    
    Vecteur() : x(0.f), y(0.f), z(0.f) {}
    Vecteur(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

    Vecteur operator -(Vecteur &vec) { 
        return Vecteur(x - vec.x, y - vec.y, z - vec.z);
    }

    Vecteur operator /(float &num) {
        return Vecteur(x/num, y/num, z/num);
    }

    Vecteur operator *(Vecteur &vec) {
        return Vecteur( x * vec.x, y * vec.y, z * vec.z);
    }

    Vecteur operator *(float &fl) {
        return Vecteur(x * fl, y * fl, z * fl);
    }

    float operator ^(Vecteur &vec) {

        return (x * vec.x + y * vec.y + z * vec.z);
    }
    

    Vecteur operator |(Vecteur &vec) {
        return Vecteur(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
    }

    float norm() {
        return std::sqrt(x * x + y * y + z * z);
    }



};

struct Face {
    float coord[3];
    float text_coord[3];
    float text_coord_norm[3];
};


template<class t> struct Point
{
    struct { t x, y ;};

    Point() : x(0), y(0) {}
    Point(t X, t Y) : x(X), y(Y) {}

    
};

typedef Point<int> Pointi;
typedef Point<float> Pointf;

#endif
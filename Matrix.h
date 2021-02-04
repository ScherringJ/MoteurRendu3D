#ifndef _MATRIX_H__
#define _MATRIX_H__

#include <vector>

class Matrix
{
private:
    std::vector<std::vector<float> > matrix;
    int row, col;

public:
    Matrix(int row , int col);

    int get_row();
    int get_col();

    Matrix identite(int dim);
   

};









#endif
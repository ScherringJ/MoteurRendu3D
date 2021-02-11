#ifndef _MATRIX_H__
#define _MATRIX_H__

#include <vector>
#include <cstdlib>


class Matrix
{
private:
    std::vector<std::vector<float> > matrix;
    int row, col;

public:
    Matrix(int row , int col);

    int get_row();
    int get_col();
    float& operator()(const unsigned &row, const unsigned &col);
    Matrix operator*(const Matrix &m);
    static Matrix identite(int dim);
    void print();
   

};









#endif
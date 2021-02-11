#include <vector>
#include <cmath>
#include <iostream>
#include "Matrix.h"


Matrix::Matrix(int row, int col) : matrix(std::vector<std::vector<float> >(row, std::vector<float>(col, 0.f))), row(row), col(col) {};

int Matrix::get_row() 
{
    return row;
}

int Matrix::get_col() 
{
    return col;
}

float& Matrix::operator()(const unsigned &row, const unsigned &col) 
{
    return matrix.at(row).at(col);
}

Matrix Matrix::operator*(const Matrix &m) {

    Matrix res(row, m.col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < m.col; j++) {

            res(i, j) = 0;
            for (int n = 0; n < col; n++) {
                res(i, j) += matrix.at(i).at(n) * m.matrix.at(n).at(j);
            }
        }
    }

    return res;
}

Matrix Matrix::identite(int dim) 
{
    
    Matrix m(dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {

            if (i==j)
            {
                m(i, j) = 1.f;
            } else
            {
                m(i, j) = 0.f;
            }
            
        }
    }
    return m;
}

void Matrix::print() 
{
    std::cout << "Matrix: " << std::endl;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            std::cout << "[" << matrix.at(i).at(j) << "] ";
        }

        std::cout << std::endl;
    }


}
#include <vector>
#include <cmath>
#include "Matrix.h"

Matrix::Matrix(int row, int col) : matrix(std::vector<std::vector<float> >(row, std::vector<float>(col, 0.f))), row(row), col(col) {};

int Matrix::get_row() {
    return row;
}

int Matrix::get_col() {
    return col;
}

Matrix Matrix::identite(int dim) {
    Matrix m(dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {

            if (i==j)
            {
                m.matrix.at(i).at(j) = 1.f;
            } else
            {
                m.matrix.at(i).at(j) = 0.f;
            }
            
        }
    }
    return m;
}
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>

typedef struct Matrix
{
    double** matrix;
    long rows;
    long cols;
} Matrix;

Matrix* create_matrix_from_file(FILE* file);
Matrix* create_matrix(long row, long col);
void free_matrix(Matrix* matrix);
double get_elem(Matrix* matrix, long row, long col);
void set_elem(Matrix* matrix, long row, long col, double val);
long get_rows(Matrix* matrix);
long get_cols(Matrix* matrix);

#endif //_MATRIX_H_

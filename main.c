#include "matrix.h"
#include <math.h>
#include <errno.h>
#include <string.h>

double euclid_norm(Matrix* matrix)
{
	double res = 0.0;

	for (long i = 0L; i < matrix->rows; i++)
		for (long j = 0L; j < matrix->cols; j++)
			res += pow(fabs(get_elem(matrix, i, j)), 2.0);

	return sqrt(res);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Too few arguements\n");
		return 1;
	}

	for (int i = 1; i < argc; i++)
	{
		FILE* f = fopen(argv[i], "r");
		if (f == NULL)
		{
			printf("Error opening file: %s\n", strerror(errno));
			return 1;
		}
		Matrix* matr = create_matrix_from_file(f);
		if (matr != NULL)
		{
			printf("Euclid norm of %d matrix: %lf\n", i, euclid_norm(matr));
			free_matrix(matr);
		}
		fclose(f);
	}

	Matrix* matr = create_matrix(2L, 3L);
	if (matr != NULL)
	{
		set_elem(matr, 1L, 0L, 3.5);
		printf("Euclid norm of matrix: %lf\n", euclid_norm(matr));
		free_matrix(matr);
	}

	return 0;
}

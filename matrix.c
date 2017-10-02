#include "matrix.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define TRUE 1 
#define FALSE 0 
#define BOOL unsigned short

BOOL read_value(FILE* f, long* value)
{
	const int MAX_LEN = 11; // number of digits in LONG_MAX (10) + '\0'

	char* buffer = (char*)malloc(sizeof(char) * MAX_LEN);
	if (buffer == NULL)
	{
		printf("Memory error while reading value from file\n");
		return FALSE;
	}
	else if (fgets(buffer, MAX_LEN, f) != NULL)
	{
		char* end_ptr = NULL;
		*value = strtol(buffer, &end_ptr, 0);
		if (*end_ptr && *end_ptr != '\n')
		{
			printf("Error in input string: %s\n", buffer);
			free(buffer);
			return FALSE;
		}
		else if (errno == ERANGE)
		{
			printf("Error in input string: %s - %s\n", buffer, strerror(errno));
			free(buffer);
			return FALSE;
		}
	}
	else
	{
		printf("Error reading input string\n");
		free(buffer);
		return FALSE;
	}
	free(buffer);
	return TRUE;
}

BOOL read_matrix(FILE* f, Matrix* matrix, long nNozeros)
{
	for (long i = 0L; i < nNozeros; i++)
	{
		long r = 0L, c = 0L;
		double value = 0.0;
		fscanf(f, "%li %li %lf", &r, &c, &value);
		if (r >= matrix->rows || c >= matrix->cols)
		{
			printf("Error reading values\n");
			return FALSE;
		}
		set_elem(matrix, r, c, value);
	}

	return TRUE;
}

Matrix* create_matrix_from_file(FILE* file)
{
	Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
	if (matrix == NULL)
	{
		printf("Memory error while reading matrix from file\n");
		return NULL;
	}

	if (read_value(file, &matrix->rows) == FALSE)
	{
		free(matrix);
		return NULL;
	}
	else if (matrix->rows <= 0L)
	{
		printf("One of matrix's parameters is less or equal to 0\n");
		free(matrix);
		return NULL;
	}

	if (read_value(file, &matrix->cols) == FALSE)
	{
		free(matrix);
		return NULL;
	}
	else if (matrix->cols <= 0L)
	{
		printf("One of matrix's parameters is less or equal to 0\n");
		free(matrix);
		return NULL;
	}

	long nNozeros = 0L;
	if (read_value(file, &nNozeros) == FALSE)
	{
		free(matrix);
		return NULL;
	}
	else if (nNozeros < 0L)
	{
		printf("Number of non-zero elements is less than 0\n");
		free(matrix);
		return NULL;
	}

	if ((matrix->matrix = (double**)calloc(matrix->rows, sizeof(double*))) == NULL)
	{
		printf("Memory error while reading matrix from file\n");
		free(matrix);
		return NULL;
	}
	for (long i = 0L; i < matrix->rows; i++)
		if ((matrix->matrix[i] = (double*)calloc(matrix->cols, sizeof(double))) == NULL)
		{
			printf("Memory error while reading matrix from file\n");
			for (long j = 0L; j < i; j++)
				free(matrix->matrix[j]);
			free(matrix->matrix);
			free(matrix);
			return NULL;
		}

	if (read_matrix(file, matrix, nNozeros) == FALSE)
	{
		free_matrix(matrix);
		return NULL;
	}

	return matrix;
}

Matrix* create_matrix(long row, long col)
{
	if (row <= 0 || col <= 0)
	{
		printf("One of matrix's parameters is less or equal to 0\n");
		return NULL;
	}
	Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
	if (matrix == NULL)
	{
		printf("Memory error while creating matrix\n");
		return NULL;
	}
	matrix->rows = row;
	matrix->cols = col;
	if ((matrix->matrix = (double**)calloc(matrix->rows, sizeof(double*))) == NULL)
	{
		printf("Memory error while creating matrix\n");
		free(matrix);
		return NULL;
	}
	for (long i = 0L; i < matrix->rows; i++)
		if ((matrix->matrix[i] = (double*)calloc(matrix->cols, sizeof(double))) == NULL)
		{
			printf("Memory error while creating matrix\n");
			for (long j = 0L; j < i; j++)
				free(matrix->matrix[j]);
			free(matrix->matrix);
			free(matrix);
			return NULL;
		}
	return matrix;
}

void free_matrix(Matrix* matrix)
{
	for (long i = 0L; i < matrix->rows; i++)
		free(matrix->matrix[i]);
	free(matrix->matrix);
	free(matrix);
}

double get_elem(Matrix* matrix, long row, long col)
{
	if (row < 0L || row >= matrix->rows || col < 0L || col >= matrix->cols)
	{
		printf("Index is out of range\n");
		return 0.0;
	}
	return matrix->matrix[row][col];
}

void set_elem(Matrix* matrix, long row, long col, double val)
{
	if (row < 0L || row >= matrix->rows || col < 0L || col >= matrix->cols)
	{
		printf("Index is out of range\n");
		return;
	}
	matrix->matrix[row][col] = val;
}

long get_rows(Matrix* matrix)
{
	return matrix->rows;
}

long get_cols(Matrix* matrix)
{
	return matrix->cols;
}

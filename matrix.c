// Copyright 2021 Matei Hriscu (311CA) <matei.hriscu@stud.acs.upb.ro>

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "constants.h"
#include "functions.h"

void sum_elements(matrix *m)
{
	// Computes the sum of all elements of matrix m, modulo MOD

	m->sum = 0;
	for (int i = 0; i < m->rows; ++i)
		for (int j = 0; j < m->columns; ++j) {
			m->sum = m->sum + m->content[i][j];
			mod(&m->sum);
		}
}

void allocate(matrix *m, int rows, int columns)
{
	/*	Allocates memory for a matrix of rows*columns dimensions
		and initializes its rows/columns/sum values	*/

	// Initializing matrix values
	m->rows = rows;
	m->columns = columns;
	m->sum = 0;

	// Allocating all memory required by the lines and columns
	m->content = (int **)malloc(rows * sizeof(int *));
	check_allocation(m->content);

	for (int i = 0; i < rows; ++i) {
		m->content[i] = (int *)malloc(columns * sizeof(int));
		check_allocation(m->content[i]);
	}
}

void read_matrix(matrix *m)
{
	// Reads the dimensions of a matrix and its content

	scanf("%d%d", &m->rows, &m->columns);

	allocate(m, m->rows, m->columns);

	for (int i = 0; i < m->rows; ++i)
		for (int j = 0; j < m->columns; ++j)
			scanf("%d", &m->content[i][j]);

	sum_elements(m);
}

void print_matrix(matrix m)
{
	// Prints the content of matrix m

	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.columns; ++j)
			printf("%d ", m.content[i][j]);
		printf("\n");
	}
}

int check_mult_comp(matrix m, matrix n)
{
	/*	If m and n cannot be multiplied the error message is shown
		and the function returns 0, otherwise the function returns 1.	*/

	if (m.columns != n.rows) {
		printf("%s\n", MULTIPLICATION_ERROR);
		return 0;
	}

	return 1;
}

matrix multiply(matrix m, matrix n)
{
	// Returns the product of matrices m and n.

	matrix product;
	allocate(&product, m.rows, n.columns);

	for (int i = 0; i < m.rows; ++i)
		for (int j = 0; j < n.columns; ++j) {
			// All elements of the product matrix must be initialized to 0.
			product.content[i][j] = 0;

			for (int k = 0; k < m.columns; ++k) {
				// Computing each element % MOD
				product.content[i][j] += m.content[i][k] * n.content[k][j];
				mod(&product.content[i][j]);
			}
		}

	sum_elements(&product);
	return product;
}

void free_matrix(matrix *m)
{
	// Frees all dynamically allocated memory used by matrix m

	for (int i = 0; i < m->rows; ++i)
		free(m->content[i]);
	free(m->content);
}

void transpose(matrix *m)
{
	// Turns m into its transpose

	// The copy will be used to reconstruct m
	matrix copy = *m;

	// Allocating new memory for m to store the transpose
	allocate(m, copy.columns, copy.rows);

	// Transposing a matrix doesn't change the sum of all elements
	m->sum = copy.sum;

	for (int i = 0; i < m->rows; ++i)
		for (int j = 0; j < m->columns; ++j)
			m->content[i][j] = copy.content[j][i];

	// copy, which stores the old matrix, is no longer needed.
	free_matrix(&copy);
}

void resize_matrix(matrix *m, int *new_lines, int nr_new_lines,
				   int *new_columns, int nr_new_columns)
{
	// Resizes m with the new lines and new columns from each specific array.

	// The copy will be used to construct the resized matrix.
	matrix copy = *m;

	// Allocating new memory for m to store the result
	allocate(m, nr_new_lines, nr_new_columns);

	for (int i = 0; i < m->rows; ++i)
		for (int j = 0; j < m->columns; ++j)
			/*	Constructing the new matrix from the
				copy according to the new lines/columns.	*/
			m->content[i][j] = copy.content[new_lines[i]][new_columns[j]];

	sum_elements(m);

	// copy, which stores the old matrix, is no longer needed.
	free_matrix(&copy);
}

matrix **generate_submatrices(matrix m)
{
	// Splits m into 4 matrices in preparation for Strassen's algorithm.

	// Allocating all necessary memory for the "matrix of matrices".
	matrix **result = (matrix **)malloc(2 * sizeof(matrix *));
	check_allocation(result);

	for (int i = 0; i < 2; ++i) {
		result[i] = (matrix *)malloc(2 * sizeof(matrix));
		check_allocation(result[i]);
		for (int j = 0; j < 2; ++j)
			allocate(&result[i][j], m.rows / 2, m.rows / 2);
	}

	// Constructing each new matrix from m.
	for (int i = 0; i < m.rows / 2; ++i)
		for (int j = 0; j < m.rows / 2; ++j) {
			result[0][0].content[i][j] = m.content[i][j];
			result[0][1].content[i][j] = m.content[i][j + m.rows / 2];
			result[1][0].content[i][j] = m.content[i + m.rows / 2][j];
			result[1][1].content[i][j] =
			m.content[i + m.rows / 2][j + m.rows / 2];
		}

	/*	Even though the sum is not necessary for Strassen's algorithm
		to work I have still computed it for completeness.	*/
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			sum_elements(&result[i][j]);

	return result;
}

matrix add(matrix a, matrix b)
{
	// Computes the sum of matrices a and b.

	matrix sum;
	allocate(&sum, a.rows, a.columns);

	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < a.columns; ++j) {
			sum.content[i][j] = a.content[i][j] + b.content[i][j];
			mod(&sum.content[i][j]);
		}

	sum.sum = a.sum + b.sum;
	mod(&sum.sum);

	return sum;
}

matrix subtract(matrix a, matrix b)
{
	// Computes the difference of matrices a and b.

	matrix difference;
	allocate(&difference, a.rows, a.columns);

	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < a.columns; ++j) {
			difference.content[i][j] = a.content[i][j] - b.content[i][j];
			mod(&difference.content[i][j]);
		}

	difference.sum = a.sum - b.sum;
	mod(&difference.sum);
	return difference;
}

matrix strassen(matrix m, matrix n)
{
	/*	Computes the product of 2 square matrices (of dimensions
		which are powers of 2) using Strassen's algorithm	*/

	if (m.rows == 1)
		return multiply(m, n);

	matrix **m_submatrices = generate_submatrices(m),
		   **n_submatrices = generate_submatrices(n),
		   *s = (matrix *)malloc(6 * sizeof(matrix)),
		   *d = (matrix *)malloc(4 * sizeof(matrix)),
		   *r = (matrix *)malloc(7 * sizeof(matrix));

	/*	s stores all sums required for the recursion,
		while d stores all differences

		These matrices must be stored so they can later be freed	*/
	s[0] = add(m_submatrices[0][0], m_submatrices[1][1]);
	s[1] = add(n_submatrices[0][0], n_submatrices[1][1]);
	s[2] = add(m_submatrices[1][0], m_submatrices[1][1]);
	s[3] = add(m_submatrices[0][0], m_submatrices[0][1]);
	s[4] = add(n_submatrices[0][0], n_submatrices[0][1]);
	s[5] = add(n_submatrices[1][0], n_submatrices[1][1]);

	d[0] = subtract(n_submatrices[0][1], n_submatrices[1][1]);
	d[1] = subtract(n_submatrices[1][0], n_submatrices[0][0]);
	d[2] = subtract(m_submatrices[1][0], m_submatrices[0][0]);
	d[3] = subtract(m_submatrices[0][1], m_submatrices[1][1]);

	// r stores all matrices required to construct the product matrix
	r[0] = strassen(s[0], s[1]);
	r[1] = strassen(s[2], n_submatrices[0][0]);
	r[2] = strassen(m_submatrices[0][0], d[0]);
	r[3] = strassen(m_submatrices[1][1], d[1]);
	r[4] = strassen(s[3], n_submatrices[1][1]);
	r[5] = strassen(d[2], s[4]);
	r[6] = strassen(d[3], s[5]);

	// Constructing the result from r
	matrix result;
	allocate(&result, m.rows, m.rows);
	for (int i = 0; i < m.rows / 2; ++i)
		for (int j = 0; j < m.rows / 2; ++j) {
			result.content[i][j] =
			r[0].content[i][j] + r[3].content[i][j]
			- r[4].content[i][j] + r[6].content[i][j];

			result.content[i][j + m.rows / 2] =
			r[2].content[i][j] + r[4].content[i][j];

			result.content[i + m.rows / 2][j] =
			r[1].content[i][j] + r[3].content[i][j];

			result.content[i + m.rows / 2][j + m.rows / 2] =
			r[0].content[i][j] - r[1].content[i][j]
			+ r[2].content[i][j] + r[5].content[i][j];

			mod(&result.content[i][j]);
			mod(&result.content[i][j + m.rows / 2]);
			mod(&result.content[i + m.rows / 2][j]);
			mod(&result.content[i + m.rows / 2][j + m.rows / 2]);
		}

	sum_elements(&result);

	// Freeing all memory allocated during the algorithm
	free_matrix_array(s, 6);
	free_matrix_array(d, 4);
	free_matrix_array(r, 7);

	for (int i = 0; i < 2; ++i) {
		free_matrix_array(m_submatrices[i], 2);
		free_matrix_array(n_submatrices[i], 2);
	}

	free(m_submatrices);
	free(n_submatrices);

	return result;
}

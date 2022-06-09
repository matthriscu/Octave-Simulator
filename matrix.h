// Copyright 2021 Matei Hriscu (311CA) <matei.hriscu@stud.acs.upb.ro>

#pragma once

typedef struct {
	int **content, rows, columns, sum;
} matrix;

void sum_elements(matrix *m);

void allocate(matrix *m, int rows, int columns);

void read_matrix(matrix *m);

void print_matrix(matrix m);

int check_mult_comp(matrix m, matrix n);

matrix multiply(matrix m, matrix n);

void free_matrix(matrix *m);

void transpose(matrix *m);

void resize_matrix(matrix *m, int *new_lines, int nr_new_lines,
				   int *new_columns, int nr_new_columns);

matrix **generate_submatrices(matrix m);

matrix add(matrix m, matrix n);

matrix subtract(matrix m, matrix n);

matrix strassen(matrix m, matrix n);

// Copyright 2021 Matei Hriscu (311CA) <matei.hriscu@stud.acs.upb.ro>

#pragma once

#include "matrix.h"

int get_index(int *index, int size);

int get_indexes(int *index1, int *index2, int size);

void erase(matrix *mat_arr, int *size, int index);

void merge_sort(matrix *mat_arr, int left, int right);

int *read_array(int *size);

void free_matrix_array(matrix *mat_arr, int size);

void resize_arr(matrix **mat_arr, int size, int *max_size);

void check_allocation(void *p);

void mod(int *x);

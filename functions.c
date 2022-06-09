// Copyright 2021 Matei Hriscu (311CA) <matei.hriscu@stud.acs.upb.ro>

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "constants.h"

void check_allocation(void *p)
{
	/*	Prints an error message and returns 0 if p is null
		(memory allocation failed), otherwise returns 1.	*/

	if (!p) {
		printf("%s\n", FAILED_ALLOCATION_ERROR);
		exit(-1);
	}
}

int get_index(int *index, int size)
{
	/*	If the index is valid the operation will proceed,
		otherwise the error message will be shown.
		Usage: if (get_index(&index)) {operation code}	*/

	scanf("%d", index);
	if (*index < 0 || *index >= size) {
		printf("%s\n", INDEX_ERROR);
		return 0;
	}
	return 1;
}

int get_indexes(int *index1, int *index2, int size)
{
	// Works the same as get_index but handles 2 indexes.

	scanf("%d%d", index1, index2);
	if (*index1 < 0 || *index1 >= size || *index2 < 0 || *index2 >= size) {
		printf("%s\n", INDEX_ERROR);
		return 0;
	}
	return 1;
}

void erase(matrix *mat_arr, int *size, int index)
{
	/*	Frees the memory of the matrix that should be erased,
		then shifts all following matrices one position to the left.	*/

	free_matrix(&mat_arr[index]);

	for (int i = index; i < *size - 1; ++i)
		mat_arr[i] = mat_arr[i + 1];

	--(*size);
}

void merge_sort(matrix *mat_arr, int left, int right)
{
	/*	Sorts matrices using the merge sort algorithm in increasing
		order according to the sum of all elements of each matrix.	*/

	if (left < right) {
		int mid = left + (right - left) / 2;
		merge_sort(mat_arr, left, mid);
		merge_sort(mat_arr, mid + 1, right);

		int i = left, j = mid + 1, cnt = 0;
		matrix *temp = (matrix *)malloc((right - left + 1) * sizeof(matrix));
		check_allocation(temp);

		while (i <= mid && j <= right)
			if (mat_arr[i].sum < mat_arr[j].sum)
				temp[cnt++] = mat_arr[i++];
			else
				temp[cnt++] = mat_arr[j++];

		while (i <= mid)
			temp[cnt++] = mat_arr[i++];

		while (j <= right)
			temp[cnt++] = mat_arr[j++];

		cnt = 0;
		for (int i = left; i <= right; ++i)
			mat_arr[i] = temp[cnt++];

		free(temp);
	}
}

int *read_array(int *size)
{
	/*	This function reads the size of an array, then allocates memory
		for it, reads its elements and returns a pointer to the array.	*/

	scanf("%d", size);

	int *array = (int *)malloc(*size * sizeof(int));
	check_allocation(array);

	for (int i = 0; i < *size; ++i)
		scanf("%d", &array[i]);

	return array;
}

void free_matrix_array(matrix *mat_arr, int size)
{
	// Frees a matrix array

	for (int i = 0; i < size; ++i)
		free_matrix(&mat_arr[i]);
	free(mat_arr);
}

void resize_arr(matrix **mat_arr, int size, int *max_size)
{
	/*	Doubles the dimension of mat_arr if max_size was reached
		or frees half of the memory if it is no longer used.	*/

	if (size == *max_size) {
		*max_size *= 2;
		*mat_arr = (matrix *)realloc(*mat_arr, *max_size * sizeof(matrix));
		check_allocation(mat_arr);
	} else if (size < *max_size / 2) {
		*max_size /= 2;
		*mat_arr = (matrix *)realloc(*mat_arr, *max_size * sizeof(matrix));
		check_allocation(mat_arr);
	}
}

void mod(int *x)
{
	// Replaces x with x % MOD

	*x = (*x % MOD + MOD) % MOD;
}

// Copyright 2021 Matei Hriscu (311CA) <matei.hriscu@stud.acs.upb.ro>

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "functions.h"
#include "constants.h"

int main(void)
{
	int size = 0, max_size = 1, index, index2;
	char task;
	matrix *mat_arr = (matrix *)malloc(sizeof(matrix));
	check_allocation(mat_arr);

	scanf(" %c", &task);
	while (task != 'Q') {
		/*	Resizing the array if the maximum capacity was reached
			or less than half the capacity is being used. */
		resize_arr(&mat_arr, size, &max_size);

		switch (task) {
		case 'L':
			read_matrix(&mat_arr[size++]);
			break;
		case 'D':
			if (get_index(&index, size))
				printf("%d %d\n", mat_arr[index].rows, mat_arr[index].columns);
			break;
		case 'P':
			if (get_index(&index, size))
				print_matrix(mat_arr[index]);
			break;
		case 'C': {
			int temp, nr_new_lines, nr_new_columns;

			/* A temporary variable must be used to store the result
			of get_index because even if the index is invalid the input
			still has to be read after the index, but before the operation. */
			temp = get_index(&index, size);

			int *new_lines = read_array(&nr_new_lines),
				*new_columns = read_array(&nr_new_columns);

			if (temp)
				resize_matrix(&mat_arr[index], new_lines, nr_new_lines,
							  new_columns, nr_new_columns);

			free(new_lines);
			free(new_columns);
			break;
		}
		case 'M':
			if (get_indexes(&index, &index2, size) &&
				check_mult_comp(mat_arr[index], mat_arr[index2]))
				mat_arr[size++] = multiply(mat_arr[index], mat_arr[index2]);
			break;
		case 'O':
			merge_sort(mat_arr, 0, size - 1);
			break;
		case 'T':
			if (get_index(&index, size))
				transpose(&mat_arr[index]);
			break;
		case 'F':
			if (get_index(&index, size))
				erase(mat_arr, &size, index);
			break;
		case 'S':
			if (get_indexes(&index, &index2, size) &&
				check_mult_comp(mat_arr[index], mat_arr[index2]))
				mat_arr[size++] = strassen(mat_arr[index], mat_arr[index2]);
			break;
		default:
			printf("%s\n", UNRECOGNIZED_COMMAND_ERROR);
			break;
		}
		scanf(" %c", &task);
	}
	free_matrix_array(mat_arr, size);
	return 0;
}

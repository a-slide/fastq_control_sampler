#ifndef PTR_ALLOCATION_H
#define PTR_ALLOCATION_H

	// Characters
	char* malloc_char_string (int);
	void free_char_string (char*);
	char** malloc_char_mat (int, int);
	void print_char_mat (char**, int, int);
	void free_char_mat (char**, int);
	char*** malloc_char_array (int, int, int);
	void print_char_array (char***, int, int, int);
	void free_char_array (char***, int, int);

	// Integers
	int* malloc_int_line (int);
	int* calloc_int_line (int);
	void print_int_line (int*, int);
	void free_int_line (int*);
	int** malloc_int_mat (int, int);
	int** calloc_int_mat (int, int);
	void print_int_mat (int**, int, int);
	void free_int_mat (int**, int);
	int*** malloc_int_array (int, int, int);
	int*** calloc_int_array (int, int, int);
	void print_int_array (int***, int, int, int);
	void free_int_array (int***, int, int);

	// Doubles
	double* malloc_double_line (int);
	double* calloc_double_line (int);
	void print_double_line (double*, int);
	void free_double_line (double*);
	double** malloc_double_mat (int, int);
	double** calloc_double_mat (int, int);
	void print_double_mat (double**, int, int);
	void free_double_mat (double**, int);
	double*** malloc_double_array (int, int, int);
	double*** calloc_double_array (int, int, int);
	void print_double_array (double***, int, int, int);
	void free_double_array (double***, int, int);

	// Files
	FILE* init_file_ptr (char*, char*);

	// Others
	void error_and_exit();
	void encadre (char*);

#endif /* PTR_ALLOCATION_H */

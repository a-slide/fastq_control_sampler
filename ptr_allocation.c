#include <stdio.h>
#include <stdlib.h>
#include "ptr_allocation.h"

/*
// Main de test des fonctions
int main (void)
{ // Main de test des fonctions
	char* char_string;
	char** char_mat;
	char*** char_array;
	int* int_line;
	int** int_mat;
	int*** int_array;
	double* double_line;
	double** double_mat;
	double*** double_array;
	
	char_string = malloc_char_string (2);
		free_char_string (char_string);
	printf ("Test malloc and free_char_string\n");
	
	char_mat = malloc_char_mat (2,5);
		free_char_mat (char_mat, 2);
	printf ("Test malloc and free_char_mat\n");		
 	
	char_array = malloc_char_array (2,5,6);
		free_char_array (char_array, 2, 5);
	printf ("Test malloc and free_char_array\n");		
	
	int_line = malloc_int_line (2);
		free_int_line (int_line);
	printf ("Test malloc and free_int_line\n");
	int_line = calloc_int_line (2);
		free_int_line (int_line);
	printf ("Test calloc and free_int_line\n");
	
	int_mat = malloc_int_mat (2,5);
		free_int_mat (int_mat, 2);
	printf ("Test malloc and free_int_mat\n");
	int_mat = calloc_int_mat (2,5);
		free_int_mat (int_mat, 2);
	printf ("Test calloc and free_int_mat\n");
 	
	int_array = malloc_int_array (2,5,6);
		free_int_array (int_array, 2, 5);
	printf ("Test malloc and free_int_array\n");
	int_array = malloc_int_array (2,5,6);
		free_int_array (int_array, 2, 5);
	printf ("Test calloc and free_int_array\n");

	double_line = malloc_double_line (2);
		free_double_line (double_line);
	printf ("Test malloc and free_double_line\n");
	double_line = calloc_double_line (2);
		free_double_line (double_line);
	printf ("Test calloc and free_double_line\n");
	
	double_mat = malloc_double_mat (2,5);
		free_double_mat (double_mat, 2);
	printf ("Test malloc and free_double_mat\n");
	double_mat = calloc_double_mat (2,5);
		free_double_mat (double_mat, 2);
	printf ("Test calloc and free_double_mat\n");
 	
	double_array = malloc_double_array (2,5,6);
		free_double_array (double_array, 2, 5);
	printf ("Test malloc and free_double_array\n");
	double_array = malloc_double_array (2,5,6);
		free_double_array (double_array, 2, 5);
	printf ("Test calloc and free_double_array\n");

	// File
	FILE* init_file_ptr (char*, char*);

	// Others
	void error_and_exit();
	
	return 1;
}
*/

////////////////////////////////////////////////////////////////////////
//  CHARACTERS
////////////////////////////////////////////////////////////////////////

// 1 dimension

char* malloc_char_string (int dim1)
{
	char* string;
	
	string = malloc (sizeof(char) * dim1);
	if (string == NULL) error_and_exit();
	
	return string;
}

void free_char_string (char* string)
{
	free (string);
	return;
}

// 2 dimensions

char** malloc_char_mat (int dim1, int dim2)
{
	int i;
	char** mat;
	
	mat = malloc (sizeof(char*) * dim1);
	if (mat == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		mat[i] = malloc (sizeof (char) * dim2);
		if (mat[i] == NULL) error_and_exit();
	}	
	return mat;
}

void print_char_mat (char** mat, int dim1, int dim2)
{
	int i,j;
	
	printf("\n");
	for (i = 0 ; i < dim1 ; i++) 
	{
		printf("#%d\t", i); // Line index
		for (j = 0 ; j < dim2 ; j++) // Impression des valeurs du tableau
		{
			printf("%c ", mat [i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}

void free_char_mat (char** mat, int dim1)
{
	int i;
	
	for ( i = 0; i < dim1; i++)
	{
		free (mat[i]);
	}
	free (mat);
	return;
}

// 3 dimensions

char*** malloc_char_array (int dim1, int dim2, int dim3)
{
	int i, j;
	char*** array;

	array = malloc (sizeof (char**) * dim1);
	if (array == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		array[i] = malloc (sizeof (char*) * dim2);
		if (array[i] == NULL) error_and_exit();
		
		for ( j = 0; j < dim2; j++ )
		{
			array[i][j] = malloc (sizeof (char) * dim3);
			if (array[i][j] == NULL) error_and_exit();
		}
	}
	return array;
}

void print_char_array (char*** array, int dim1, int dim2, int dim3)
{
	int i, j, k ;
	
	for (i = 0 ; i < dim1 ; i++) 
	{
		printf("x = %d\n", i);
		for (j = 0 ; j < dim2 ; j++) 
		{
			printf("#%d\t", j); // Line index
			for (k = 0 ; k < dim3 ; k++) // Impression des valeurs du tableau
			{
				printf("%c ", array [i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	return;
}

void free_char_array (char*** array, int dim1, int dim2)
{
	int i, j;
	
	for ( i = 0; i < dim1; i++)
		for ( j = 0; j < dim2; j++)
			free (array[i][j]);
	
	for ( i = 0; i < dim1; i++)
		free (array[i]);
	
	free (array);
	
	return;
}

////////////////////////////////////////////////////////////////////////
//  INTEGRALS
////////////////////////////////////////////////////////////////////////

// 1 dimension

int* malloc_int_line (int dim1)
{
	int* line;
	
	line = malloc (sizeof(int) * dim1);
	if (line == NULL) error_and_exit();
	
	return line;
}

int* calloc_int_line (int dim1)
{
	int* line;
	
	line = calloc (sizeof(int), dim1);
	if (line == NULL) error_and_exit();
	
	return line;
}

void print_int_line (int* line, int dim1)
{
	int i;
	
	printf("\n");
	for (i = 0 ; i < dim1 ; i++) 
		printf("%d ", line [i]);

	printf("\n");
	return;
}

void free_int_line (int* line)
{
	free (line);
	return;
}

// 2 dimensions

int** malloc_int_mat (int dim1, int dim2)
{
	int i;
	int** mat;
	
	mat = malloc (sizeof(int*) * dim1);
	if (mat == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		mat[i] = malloc (sizeof (int) * dim2);
		if (mat[i] == NULL) error_and_exit();
	}	
	return mat;
}

int** calloc_int_mat (int dim1, int dim2)
{
	int i;
	int** mat;
	
	mat = calloc (sizeof(int*), dim1);
	if (mat == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		mat[i] = calloc (sizeof (int), dim2);
		if (mat[i] == NULL) error_and_exit();
	}	
	return mat;
}

void print_int_mat (int** mat, int dim1, int dim2)
{
	int i,j;
	
	printf("\n");
	for (i = 0 ; i < dim1 ; i++) 
	{
		printf("#%d\t", i); // Line index
		for (j = 0 ; j < dim2 ; j++) // Impression des valeurs du tableau
		{
			printf("%d ", mat [i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}

void free_int_mat (int** mat, int dim1)
{
	int i;
	
	for ( i = 0; i < dim1; i++)
	{
		free (mat[i]);
	}
	free (mat);
	return;
}

// 3 dimensions

int*** malloc_int_array (int dim1, int dim2, int dim3)
{
	int i, j;
	int*** array;

	array = malloc (sizeof (int**) * dim1);
	if (array == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		array[i] = malloc (sizeof (int*) * dim2);
		if (array[i] == NULL) error_and_exit();
		
		for ( j = 0; j < dim2; j++ )
		{
			array[i][j] = malloc (sizeof (int) * dim3);
			if (array[i][j] == NULL) error_and_exit();
		}
	}
	return array;
}

int*** calloc_int_array (int dim1, int dim2, int dim3)
{
	int i, j;
	int*** array;

	array = calloc (sizeof (int**), dim1);
	if (array == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		array[i] = calloc (sizeof (int*), dim2);
		if (array[i] == NULL) error_and_exit();
		
		for ( j = 0; j < dim2; j++ )
		{
			array[i][j] = calloc (sizeof (int), dim3);
			if (array[i][j] == NULL) error_and_exit();
		}
	}
	return array;
}

void print_int_array (int*** array, int dim1, int dim2, int dim3)
{
	int i, j, k ;
	
	for (i = 0 ; i < dim1 ; i++) 
	{
		printf("x = %d\n", i);
		for (j = 0 ; j < dim2 ; j++) 
		{
			printf("#%d\t", j); // Line index
			for (k = 0 ; k < dim3 ; k++) // Impression des valeurs du tableau
			{
				printf("%d ", array [i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	return;
}

void free_int_array (int*** array, int dim1, int dim2)
{
	int i, j;
	
	for ( i = 0; i < dim1; i++)
		for ( j = 0; j < dim2; j++)
			free (array[i][j]);
	
	for ( i = 0; i < dim1; i++)
		free (array[i]);
	
	free (array);
	
	return;
}


////////////////////////////////////////////////////////////////////////
//  DOUBLE
////////////////////////////////////////////////////////////////////////

// 1 dimension

double* malloc_double_line (int dim1)
{
	double* line;
	
	line = malloc (sizeof(double) * dim1);
	if (line == NULL) error_and_exit();
	
	return line;
}

double* calloc_double_line (int dim1)
{
	double* line;
	
	line = calloc (sizeof(double), dim1);
	if (line == NULL) error_and_exit();
	
	return line;
}

void print_double_line (double* line, int dim1)
{
	int i;
	
	printf("\n");
	for (i = 0 ; i < dim1 ; i++) 
		printf("%.2e ", line [i]);

	printf("\n");
	return;
}

void free_double_line (double* line)
{
	free (line);
	return;
}

// 2 dimensions

double** malloc_double_mat (int dim1, int dim2)
{
	int i;
	double** mat;
	
	mat = malloc (sizeof(double) * dim1);
	if (mat == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		mat[i] = malloc (sizeof (double) * dim2);
		if (mat[i] == NULL) error_and_exit();
	}	
	return mat;
}

double** calloc_double_mat (int dim1, int dim2)
{
	int i;
	double** mat;
	
	mat = calloc (sizeof(double*), dim1);
	if (mat == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		mat[i] = calloc (sizeof (double), dim2);
		if (mat[i] == NULL) error_and_exit();
	}	
	return mat;
}

void print_double_mat (double** mat, int dim1, int dim2)
{
	int i,j;
	
	printf("\n");
	for (i = 0 ; i < dim1 ; i++) 
	{
		printf("#%d\t", i); // Line index
		for (j = 0 ; j < dim2 ; j++) // Impression des valeurs du tableau
		{
			printf("%.2e ", mat [i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}

void free_double_mat (double** mat, int dim1)
{
	int i;
	
	for ( i = 0; i < dim1; i++)
	{
		free (mat[i]);
	}
	free (mat);
	return;
}

// 3 dimensions

double*** malloc_double_array (int dim1, int dim2, int dim3)
{
	int i, j;
	double*** array;

	array = malloc (sizeof (double**) * dim1);
	if (array == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		array[i] = malloc (sizeof (double*) * dim2);
		if (array[i] == NULL) error_and_exit();
		
		for ( j = 0; j < dim2; j++ )
		{
			array[i][j] = malloc (sizeof (double) * dim3);
			if (array[i][j] == NULL) error_and_exit();
		}
	}
	return array;
}

double*** calloc_double_array (int dim1, int dim2, int dim3)
{
	int i, j;
	double*** array;

	array = calloc (sizeof (double**), dim1);
	if (array == NULL) error_and_exit();

	for ( i = 0; i < dim1; i++)
	{
		array[i] = calloc (sizeof (double*), dim2);
		if (array[i] == NULL) error_and_exit();
		
		for ( j = 0; j < dim2; j++ )
		{
			array[i][j] = calloc (sizeof (double), dim3);
			if (array[i][j] == NULL) error_and_exit();
		}
	}
	return array;
}

void print_double_array (double*** array, int dim1, int dim2, int dim3)
{
	int i, j, k ;
	
	for (i = 0 ; i < dim1 ; i++) 
	{
		printf("x = %d\n", i);
		for (j = 0 ; j < dim2 ; j++) 
		{
			printf("#%d\t", j); // Line index
			for (k = 0 ; k < dim3 ; k++) // Impression des valeurs du tableau
			{
				printf("%.2e ", array [i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	return;
}

void free_double_array (double*** array, int dim1, int dim2)
{
	int i, j;
	
	for ( i = 0; i < dim1; i++)
		for ( j = 0; j < dim2; j++)
			free (array[i][j]);
	
	for ( i = 0; i < dim1; i++)
		free (array[i]);
	
	free (array);
	
	return;
}

////////////////////////////////////////////////////////////////////////
//  FILES
////////////////////////////////////////////////////////////////////////

// initialise un ptr de fichier en  mode r, w ou a 

 FILE* init_file_ptr (char* name, char* mode)
{
	FILE* file = NULL;
	
	file = fopen (name, mode);
	if (file == NULL)
	{
		fprintf (stderr, "Initialisation of %s impossible\n\n", name );
		exit (EXIT_FAILURE);
	}
	return file;
}

////////////////////////////////////////////////////////////////////////
// ERROR AND EXIT
////////////////////////////////////////////////////////////////////////

void error_and_exit()
{
	fprintf (stderr, "Memory allocation error\n\n");
	exit (EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////////////
// ENCADRE = encadre un chaine de charactere par des #
////////////////////////////////////////////////////////////////////////

void encadre (char* name)
{
	int i;
	
	printf("\n\n");
	
	for (i = 0; i < 75; i++)
		printf("#");
		
	printf("\n# %s\n", name);

	for (i = 0; i < 75; i++)
		printf("#");
	
	printf("\n\n");
	return;
}

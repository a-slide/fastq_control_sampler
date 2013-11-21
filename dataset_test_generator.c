#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "dataset_test_generator.h"


////////////////////////////////////////////////////////////////////////
//  MAIN                                                              //
////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	T_info* list_seq;

	// Usage test checking parameters
	usage(argv[0], argc);
	// Import the differents reference sequence contained in fasta files
	list_seq = create_ref_list (argv, argc-1);
	// Print valued contained in the list
	print_list (list_seq);
	// Generate fastq files R1 and R2 from the reference sequences
	generate_fastq (list_seq);
	
	return 0;
}
 
////////////////////////////////////////////////////////////////////////
//  USAGE                                                             //
//////////////////////////////////////////////////////////////////////// 
void usage (char* nom_prog, int argc)
{
	if (argc < 2)
	{
		fprintf (stderr, "Usage : %s [ref_1.fa] [ref_2.fa] ... [ref_n.fa]\n", nom_prog);
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////
// create_ref_list = import fasta ref genomes within a structure tab  //
////////////////////////////////////////////////////////////////////////

T_info* create_ref_list (char** argv, int nb_fasta)
{
	int i, j;
	int n;
	FILE* file = NULL;
	T_info* pHead = NULL;
	T_info* ptri = NULL;
	T_info* ptrj = NULL;
	
	// ADDITION OF A RANDOM REFERENCE
	printf ("\nGenerating a random reference\n\n");
	ptrj = create_info_cell();
	ptrj -> size_name = 16;
	ptrj -> name = "random_reference";
	ptrj -> size_seq = SIZE_OF_RANDOM_REF;
	ptrj -> seq = generate_random_ref(SIZE_OF_RANDOM_REF);
	ptrj -> next = NULL;
	
	pHead = ptrj; // the list head now point to the adress of the first cell until the end of the function
	ptri = ptrj; // ptri point to the first cell for now but will move latter
	
	// DISCOVERY AND ADDITION OF REFERENCES CONTAINED IN THE REFERENCE FASTA FILES
	for (i = 0; i < nb_fasta; i++) // for each reference fasta file
	{
		printf ("\nProcessing File %s\n\n", argv[i+1]);
		file = init_file_ptr( argv[i+1], "r");
		
		n = nref(file, argv[i+1]);
		for (j = 0; j < n ; j++)

		{
			// Creating new cell
			ptrj = create_info_cell();
			// Filling fields of the new cell
			ptrj -> size_name = (evaluate_name_size (file));
			ptrj -> name = output_name(file, ptrj -> size_name);
			printf ("Importation of the reference sequence : %s\n", ptrj -> name);
			ptrj -> size_seq = (evaluate_seq_size (file));
			verify_ref_size (ptrj -> size_seq, i);
			ptrj -> seq = output_seq(file, ptrj -> size_seq);
			ptrj -> next = NULL;
			// Linking the new cell to the previous one and moving forward the active cell pointer
			ptri -> next = ptrj;
			ptri = ptrj;
		}
		fclose(file);
	}				
	return pHead;
}

////////////////////////////////////////////////////////////////////////
// nref = retourne le nombre de séquence de ref dans un fasta         //
////////////////////////////////////////////////////////////////////////

int nref (FILE* file, char* filename)
{
	int c = 0;
	int nref = 0;
	
	while ((c = fgetc(file)) != EOF) // Pas trés econome en resource je suis sur qu'on peu se contenter du seul 1er char...
		if ( c== '>')
			nref++;
	
	printf("\nThe %s file contains %d reference sequence(s)\n", filename, nref);
	rewind (file);
	return nref;
}

////////////////////////////////////////////////////////////////////////
// create_info_cell = create an empty info cell                       //
////////////////////////////////////////////////////////////////////////

T_info* create_info_cell(void)
{
	T_info* cell;
	cell = malloc( sizeof (T_info));
	if (cell == NULL)
	{
		fprintf (stderr, "Allocation Error\n\n");
		exit (EXIT_FAILURE);
	}
	return cell;
}

////////////////////////////////////////////////////////////////////////
// evaluate_name_size AND evaluate_seq_size                           //
////////////////////////////////////////////////////////////////////////

int evaluate_name_size (FILE* file)
{
	long position;
	int c, i = 0;
	
	position = ftell(file); // Save of file ptr position
	
	while ((c = fgetc(file)) != '\n') // continue until the end of the line
		if (isprint(c) && c != '>')// count characters excluding > and non printable
			i++;

	printf ("\n");
	fseek (file, position, SEEK_SET); // restore the position of the file ptr
	return i;
}

////////////////////////////////////////////////////////////////////////

int evaluate_seq_size (FILE* file)
{
	long position;
	int c, i = 0;
	
	position = ftell(file); // Save of file ptr position
	
	while (((c = fgetc(file)) != '>') && (c != EOF))
		if (isalpha(c)) // count only alphabetical characters
			i++; 

	fseek (file, position, SEEK_SET); // restore the position of the file ptr
	return i;
}

////////////////////////////////////////////////////////////////////////
// output_name AND output_seq                                         //
////////////////////////////////////////////////////////////////////////

char* output_name (FILE* file, int size)
{
	int c, i = 0;
	char* name;
	
	name = malloc (sizeof(char) * (size + 1)); // Allocate the require space
	if (name == NULL)
	{
		fprintf (stderr, "Allocation Error\n\n");
		exit (EXIT_FAILURE);
	}
	
	while ((c = fgetc(file)) != '\n') // get char by char
		if (isprint(c) && c != '>') // register only if printable and different from >
		{
			name[i] = c;
			i++;
		}
	name [size] = '\0'; // ajoute un /0 en fin de ligne
	return name;
}

////////////////////////////////////////////////////////////////////////

char* output_seq (FILE* file, int size)
{
	int c, i = 0;
	char* seq;
	
	seq = malloc (sizeof(char) * (size + 1));
	if (seq == NULL)
	{
		fprintf (stderr, "Allocation Error\n\n");
		exit (EXIT_FAILURE);
	}
	
	while (((c = fgetc(file)) != '>') && (c != EOF)) // continue until the next sequence or the end of file
	{
		if (isalpha(c)) // count only alphabetical characters
		{
			if islower(c)
				seq[i] = 'N'; // hard mask repeated sequences
			else
				seq[i] = c;
			i++;
		}
	}

	seq [size] = '\0'; // ajoute un /0 en fin de ligne
	return seq;
}



////////////////////////////////////////////////////////////////////////
// verify_ref_size = evaluate if ref size is long enough              //
////////////////////////////////////////////////////////////////////////

void verify_ref_size (int size, int ref_nb)
{
	if (size <= SIZE_MAX_SONIC)
	{
		fprintf (stderr, "Size of reference %d too short to be parsed \n", ref_nb);
		fprintf (stderr, "Use a longer reference or decrease SIZE_MAX_SONIC\n\n");
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////
// generate_random_ref = generate a random DNA string of a given size //
////////////////////////////////////////////////////////////////////////

char* generate_random_ref (int size)
{
	char* seq;
	int i;
	
	seq = malloc (sizeof(char) * (size + 1));
	
	for (i = 0; i < size; i++)
	{
		seq[i] = DNA_BASES [rand() % 4];
	}
	seq [size] = '\0';
	
	return seq;
}

////////////////////////////////////////////////////////////////////////
// print_tab_info													  //
////////////////////////////////////////////////////////////////////////

void print_list (T_info* list_head)
{
	T_info* ptri = list_head;
	
	while (ptri != NULL)
	{
		printf("\nSize Name : %d", ptri -> size_name);
		printf("\n%s", ptri -> name);
		printf("\nSize Seq: %d", ptri -> size_seq);	
///		printf("\n%s\n", ptri -> seq);
		printf("\n");
		
		ptri = ptri -> next;
		
	}
	return;
}

////////////////////////////////////////////////////////////////////////
// generate_fastq = generate fastq pairs from ref genomes + scramble  //
////////////////////////////////////////////////////////////////////////
 
void generate_fastq (T_info* list_head)
{
	int j, k, k1, k2;
	int size_frag;
	int pos_init;
	FILE* file1 = init_file_ptr( "R1.fastq", "w"); // Fichier pour liste read R1
	FILE* file2 = init_file_ptr( "R2.fastq", "w"); // Fichier pour liste read R1
	
	T_info* ptri = NULL;
	ptri = list_head;
	srand (time(NULL)); // seed value for rand
		
	while (ptri != NULL) // Pour chaque reference dans la liste
	{
		for (j = 0; j < NB_PAIR_BY_REF; j++)  // Pour n couples par reference
		{
			fprintf(file1, "@%s:%d\n", ptri -> name, j+1); // ligne de titre du fastq
			fprintf(file2, "@%s:%d\n", ptri -> name, j+1);

			size_frag = rand() % (SIZE_MAX_SONIC - SIZE_MIN_SONIC) + SIZE_MIN_SONIC; //range SIZE_MIN_SONIC to SIZE_MAX_SONIC
			pos_init = rand() % (ptri -> size_seq - size_frag); // range from 0 to (size_seq - taille_frag)
			
			for (k = 0, k1 = pos_init, k2 = (pos_init + size_frag) ; k < SIZE_READ ; k++, k1++, k2--)  // Pour k positions dans le read
			{ 
				fprintf(file1, "%c", ptri -> seq[k1]); 
				fprintf(file2, "%c", complementary (ptri -> seq[k2]));
			}
			
			fprintf(file1, "\n+\n"); // separation sequence and quality score
			fprintf(file2, "\n+\n");
			
			for (k = 0 ; k < SIZE_READ; k++)  // Pour k positions dans le read
			{ 
				fprintf(file1, "%c", QUALITY_SCORE [rand() % (strlen(QUALITY_SCORE) - 1)]); // separation sequence and quality score
				fprintf(file2, "%c", QUALITY_SCORE [rand() % (strlen(QUALITY_SCORE) - 1)]);
			}
			fprintf(file1, "\n"); // saut de ligne pour sequence suivante
			fprintf(file2, "\n");
		}
		ptri = ptri -> next;
	}
	fclose (file1);
	fclose (file2);
}	

////////////////////////////////////////////////////////////////////////
// complementary = return DNA compementary base including IUPAC code  //
////////////////////////////////////////////////////////////////////////
 
char complementary (char c)
{
	switch (c)
	{
		case 'A' :
			return 'T';
		case 'T' :
			return 'A';
		case 'C' :
			return 'G';
		case 'G' :
			return 'C';		
		case 'R' : // A or G
			return 'Y'; // T or C
		case 'Y' :
			return 'R';
		case 'K' : // A or C
			return 'M'; // T or G
		case 'M' :
			return 'K';
		case 'B' : // C or G or T
			return 'V'; // A or C or G
		case 'V' :
			return 'B';
		case 'D' : // A or G or T
			return 'H'; // A or C or T
		case 'H' :
			return 'D';		
		default :
			return c;
	}
}

////////////////////////////////////////////////////////////////////////
//  init_file_ptr = initialise un ptr de fichier en  mode r, w ou a   //
////////////////////////////////////////////////////////////////////////

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

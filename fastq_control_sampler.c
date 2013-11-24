/* fastq_control_sampler
 * 
 * Copyright 2013 adrien <adrien.leger@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as 
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "ptr_allocation.h"
#include "fastq_control_sampler.h"

////////////////////////////////////////////////////////////////////////
// main                                                             
////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	T_info* list_seq;

	// Usage test checking parameters
	usage(argv[0], argc);
	// Import the differents reference sequence contained in fasta files
	list_seq = create_ref_list (argv, argc-1);
	// Print valued contained in the list
	///print_list (list_seq);
	// Generate fastq files R1 and R2 from the reference sequences
	generate_fastq (list_seq);
	return 1;
}
 
////////////////////////////////////////////////////////////////////////
// usage                                                            
//////////////////////////////////////////////////////////////////////// 
void usage (char* nom_prog, int argc)
{
	if (argc < 2)
	{
		fprintf (stderr, "Usage : %s [ref_1.fa] [ref_2.fa] ... [ref_n.fa]\n", nom_prog);
		fprintf (stderr, "\t'N' containing sequences will be excluded from the analyses\n");
		fprintf (stderr, "\tUnless the variable MASK_REPEAT is set to 0, lowercase characters will also be excluded\n");
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////
// create_ref_list = import fasta ref genomes within a structure tab
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
		printf ("Processing File %s\n", argv[i+1]);
		file = init_file_ptr( argv[i+1], "r"); // open the current fasta file
		
		n = nref(file); // output the number of ref seq
		printf("\tThe file contain %d reference sequence(s)\n", n);
		for (j = 0; j < n ; j++) // for each reference sequences within the current file

		{
			// Create a new cell
			ptrj = create_info_cell();
			// Fill fields of the new cell
			ptrj -> size_name = (evaluate_name_size (file));
			ptrj -> name = output_name(file, ptrj -> size_name);
			printf ("\t\tImportation of the reference sequence : %s\n", ptrj -> name);
			ptrj -> size_seq = (evaluate_seq_size (file));
			verify_ref_size (ptrj -> size_seq, i);
			ptrj -> seq = output_seq(file, ptrj -> size_seq);
			ptrj -> next = NULL;
			// Link the new cell to the previous one and push forward the active cell pointer
			ptri -> next = ptrj;
			ptri = ptrj;
		}
		fclose(file);
	}				
	return pHead;
}

////////////////////////////////////////////////////////////////////////
// nref = retourne le nombre de séquence de ref dans un fasta
////////////////////////////////////////////////////////////////////////

int nref (FILE* file)
{
	int c = 0;
	int n = 0;
	
	while ((c = fgetc(file)) != EOF) // Pas trés econome en resource je suis sur qu'on peu se contenter du seul 1er char...
		if ( c== '>')
			n++;

	rewind (file);
	return n;
}

////////////////////////////////////////////////////////////////////////
// create_info_cell = create an empty info cell
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
// evaluate_name_size AND evaluate_seq_size determine the size of a string in a file
////////////////////////////////////////////////////////////////////////

int evaluate_name_size (FILE* file)
{
	long position;
	int c, i = 0;
	
	position = ftell(file); // Save of file ptr position
	
	while ((c = fgetc(file)) != '\n') // continue until the end of the line
		if (isprint(c) && c != '>')// count characters excluding > and non printable
			i++;

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
// output_name AND output_seq return the string in the file
////////////////////////////////////////////////////////////////////////

char* output_name (FILE* file, int size)
{
	int c, i = 0;
	char* name;
	
	name = malloc_char_string (size + 1);
	
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
	
	seq = malloc_char_string (size + 1);
	
	if (MASK_REPEAT == 1) // in case of masking of repeats
	{
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
	}
	
	else // in case of non masking of repeats
	{
		while (((c = fgetc(file)) != '>') && (c != EOF)) // continue until the next sequence or the end of file
		{
			if (isalpha(c)) // count only alphabetical characters
			{
				seq[i] = c;
				i++;
			}
		}
	}

	seq [size] = '\0'; // ajoute un /0 en fin de ligne
	return seq;
}

////////////////////////////////////////////////////////////////////////
// verify_ref_size = evaluate if ref size is long enough
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
// generate_random_ref = generate a random DNA string of a given size
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
// print_tab_info												
////////////////////////////////////////////////////////////////////////

void print_list (T_info* list_head)
{
	T_info* ptri = list_head;
	
	while (ptri != NULL)
	{
		printf("\nSize Name : %d", ptri -> size_name);
		printf("\n%s", ptri -> name);
		printf("\nSize Seq: %d", ptri -> size_seq);	
		printf("\n%s\n", ptri -> seq);
		printf("\n\n");
		
		ptri = ptri -> next;
		
	}
	return;
}

////////////////////////////////////////////////////////////////////////
// generate_fastq = generate fastq pairs from ref genomes + scramble
////////////////////////////////////////////////////////////////////////
void generate_fastq (T_info* list_head)
{
	int i;
	char* forward_read;
	char* reverse_read;
	char* forward_quality;
	char* reverse_quality;
	T_info* ptri = NULL;
	FILE* file1 = NULL;
	FILE* file2 = NULL;

	ptri = list_head;
	file1 = init_file_ptr( "CONTROL_R1.fastq", "w"); // File for writing forward fastq reads
	file2 =  init_file_ptr( "CONTROL_R2.fastq", "w"); // File for writing reverse fastq reads
	srand (time(NULL)); // seed value for rand
	
	while (ptri != NULL) // For each reference in the list
	{
		printf ("\nProcessing reference %s\n", ptri -> name);
		forward_read = malloc_char_string (SIZE_READ + 1);
		reverse_read =  malloc_char_string (SIZE_READ + 1);
		forward_quality = malloc_char_string (SIZE_READ + 1);
		reverse_quality =  malloc_char_string (SIZE_READ + 1);
		
		for (i = 0; i < NB_PAIR_BY_REF; i++)  // For i couple per reference
		{
			// printf ("\nPair %d\t", i);
			// If it is possible to select a valid pair
			if (select_read_pair (ptri, &forward_read, &reverse_read) == 1)
			{					
				generate_random_quality (&forward_quality, &reverse_quality);
				fprintf(file1, "@CONTROL_%s:%d\n%s\n+\n%s\n", ptri -> name, i+1, forward_read, forward_quality);
				fprintf(file2, "@CONTROL_%s:%d\n%s\n+\n%s\n", ptri -> name, i+1, reverse_read, reverse_quality);
			}
			// If it is not possible to select a valid pair = skip this reference
			else
			{
				printf("\tAttempt to generate a valid pair failed after %d tries\n", MAX_TRY_VALID_PAIR);
				printf("\tSkipping to the next reference sequence\n");
				break;
			}
		}
		ptri = ptri -> next;
		free (forward_read);
		free (forward_quality);
		free (reverse_read);
		free (reverse_quality);
	}
	fclose (file1);
	fclose (file2);
}	

////////////////////////////////////////////////////////////////////////
// select_read_pair
////////////////////////////////////////////////////////////////////////

int select_read_pair (T_info* ptri, char** p_forward_read, char** p_reverse_read)
{
	int i, j, k;
	int try = 0;
	int size_frag;
	int pos_init;
	int valid_pair;
	
	do // loop while finding a pair of reads with no 'N' in their sequences
	{
		size_frag = rand() % (SIZE_MAX_SONIC - SIZE_MIN_SONIC) + SIZE_MIN_SONIC; //range SIZE_MIN_SONIC to SIZE_MAX_SONIC
		pos_init = rand() % (ptri -> size_seq - size_frag); // range from 0 to (size_seq - taille_frag)
		try ++;
		valid_pair = 0;
		
		// scan all the lenght of the sequence
		for (i = 0, j = pos_init, k = (pos_init + size_frag) ; i < SIZE_READ ; i++, j++, k--)
		{
			// If a N is found in the sequence try to resample an other pair somewher else
			if ((ptri -> seq[j] == 'N') || (ptri -> seq[k] == 'N'))
				break;
			
			// If no N is found in the sequence = the pair is valid and can be returned
			else
			{
				p_forward_read [0][i] = ptri->seq[j];
				p_reverse_read [0][i] = complementary (ptri->seq[k]);
				valid_pair = 1;
			}
		}
	} while ((valid_pair == 0) && (try < MAX_TRY_VALID_PAIR));

	if (valid_pair == 1)
	{
		//printf ("%d tries needed", try);
		p_forward_read[0][SIZE_READ] = '\0';
		p_reverse_read[0][SIZE_READ] = '\0';
		return 1;
	}
	else
	{
		*p_forward_read = NULL;
		*p_reverse_read = NULL;
		return 0;
	}
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
// generate_random_quality
////////////////////////////////////////////////////////////////////////

void generate_random_quality (char** p_forward_quality, char** p_reverse_quality)
{
	int i;
	
	for (i = 0 ; i < SIZE_READ; i++)  // generate random quality string for each position in the read
	{ 
		p_forward_quality[0][i] = (QUALITY_SCORE [rand() % (strlen(QUALITY_SCORE) - 1)]);
		p_reverse_quality[0][i] = (QUALITY_SCORE [rand() % (strlen(QUALITY_SCORE) - 1)]);
	}
	p_forward_quality[0][SIZE_READ] = '\0';
	p_reverse_quality[0][SIZE_READ] = '\0';
	
	return;
}

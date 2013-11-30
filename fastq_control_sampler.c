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
#include <unistd.h>
#include <limits.h>
#include "ptr_allocation.h"
#include "fastq_control_sampler.h"

/// /////////////////////////////////////////////////////////////////////
/// main                                                             
/// /////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	T_info* list_seq;
	int nb_arg;
	int optch;
    extern int opterr;
 
    char format[] = "Mmp:q:e:s:l:u:n:r:v:ah";
    opterr = 1;
 
    while ((optch = getopt(argc, argv, format)) != -1)
    switch (optch)
    {
        case 'm':
            MASK_REPEAT = 0;
            break;
        case 'M':
            MASK_REPEAT = 1;
            break;
        case 'e':
            ERR_FREQ = atof(optarg);
            if ((ERR_FREQ > 1) || (ERR_FREQ < 0)) usage(argv[0]);
            break;
        case 's':
            SIZE_READ = atoi(optarg);
            if (SIZE_READ < 1) usage(argv[0]);
            break;
         case 'p':
            MIN_QUAL = atoi(optarg);
            if ((MIN_QUAL < 1) || (MAX_QUAL > 40)) usage(argv[0]);
            break;
         case 'q':
            MAX_QUAL = atoi(optarg);
            if ((MAX_QUAL > 40) || (MAX_QUAL < MIN_QUAL)) usage(argv[0]);
            break;
        case 'l':
            SIZE_MIN_SONIC = atoi(optarg);
			if (SIZE_MIN_SONIC < SIZE_READ) usage(argv[0]);
            break;
        case 'u':
            SIZE_MAX_SONIC = atoi(optarg);
            if ((SIZE_MIN_SONIC < SIZE_READ) || (SIZE_MAX_SONIC < SIZE_MIN_SONIC)) usage(argv[0]);
			break;
        case 'n':
            NB_PAIR_BY_REF = atoi(optarg);
            if (NB_PAIR_BY_REF < 1) usage(argv[0]);
			break;
        case 'r':
            SIZE_OF_RANDOM_REF = atoi(optarg);
            if (SIZE_OF_RANDOM_REF < (10*SIZE_READ)) usage(argv[0]);
			break;        
        case 'v':
			MAX_TRY_VALID_PAIR = atoi(optarg);
			if (MAX_TRY_VALID_PAIR < 1) usage(argv[0]);
            break;
		case 'a':
			FASTA = 1;
            break;
        case 'h':
			usage(argv[0]);
            break;
    }
	
	encadre ("PARAMETERS");
	printf ("Size of the randomly generated reference sequence : %d\n", SIZE_OF_RANDOM_REF);
	printf ("Size the reads: %d\n", SIZE_READ);
	printf ("Lower sonication size of fragments: %d\n", SIZE_MIN_SONIC);
	printf ("Upper sonication size of fragments: %d\n", SIZE_MAX_SONIC);
	printf ("Maximal PHRED quality in quality string: %d\n", MAX_QUAL);
	printf ("Minimal PHRED quality in quality string: %d\n", MIN_QUAL);
	printf ("Frequency of errors in sequence strings: %f\n", ERR_FREQ);
    printf ("Max Number of pairs to be generated for each reference sequences (if possible) : %d\n", NB_PAIR_BY_REF);
    printf ("Maximum number of tries to generate a valid read pair : %d\n", MAX_TRY_VALID_PAIR);
  	printf ("Pairs %s selected in repeat regions\n", ((MASK_REPEAT == 1) ? "will not be" : "may be"));
    printf ("Fasta files %s be created\n", ((FASTA == 1) ? "will also" : "won't"));
	
	// Calcul le nombre d'arguments restants = fasta files
	nb_arg = argc - optind;
	 
	// Import the differents reference sequence contained in fasta files
	encadre ("IMPORTATION OF FASTA REFERENCES");
	list_seq = create_ref_list (argv, optind, nb_arg); 
	
	// Print valued contained in the list
	///encadre ("LIST OF REFERENCES IMPORTED");ln
	///print_list (list_seq);
	
	// Generate fastq files R1 and R2 from the reference sequences
	encadre ("SELECTION OF READ PAIRS AND CREATION OF FASTQ FILES");
	generate_fastq (list_seq);
	
	printf ("\n%d mutation%s introduced in sequences", MUTATION,((MUTATION > 1) ? "s were" : " was"));
	encadre ("DONE");
	return 0;
}

////////////////////////////////////////////////////////////////////////
// usage                                                            
//////////////////////////////////////////////////////////////////////// 
void usage (char* prog_name)
{
	
	fprintf (stderr, "\nUSAGE:\t%s [M S <int> L <int> U <int> N <int> R <int> V <int> h ], [FASTA FILE]..\n\n", prog_name);
	
	fprintf (stderr, "DESCRIPTION\n\n\
	Generate control pair end fastq files from DNA fasta references and a randomly generated reference\n\n\
	The program can be run without any parameter, in this case only the ramdom reference will be included\n\n\
	The fasta file should follow the international standard and can contain multiple sequences\n\
	Unsure DNA bases identified with 'N' 'Y' 'R' in reference sequences are excluded from the fastq file\n\
	By default the lowercase DNA bases that usually indicates repeated regions are also masked.\n\n");
	
	fprintf (stderr, "OPTIONS\n\n\
	-M	Exclude repeat regions (lowercase characters) during fastq sampling (option by defaut)\n\
	-m	Include repeat regions (lowercase characters) during fastq sampling\n\
		The options -M et -m are incompatible (in case of conflict -M will be used)\n\n\
	-e	Frequency of errors per base in fastq (by default = 0.01)\n\
		The value must be chosen between 0 and 1\n\n\
	-p	Minimal PHRED quality in fastq quality string. Min = 1, Max = 40 (by default = 30)\n\
	-q	Maximal PHRED quality in fastq quality string. Min = Minimal PHRED quality, Max = 40, (by default = 40)\n\n\
	-s	Size of reads to be selected in fasta references\n\
		The value must be strictly positive (by default = 150)\n\n\
	-l	Lower size of fragments obtained after sonication (by default = 200)\n\
	-u	Upper size of fragments obtained after sonication (by default = 600)\n\
		Upper size must be > or = than Lower size and both must be > or = than the size of reads\n\n\
	-n	Maximal Number of pairs to be generated for each reference sequences (if possible)\n\
		The value must be strictly positive (by default = 10)\n\n\
	-v	Maximal Number of tries to obtain a valid pair (ie with no 'N')\n\
		The value must be strictly positive (by default = 100)\n\n\
	-r	Size of random reference to be generated\n\
		The value must be a least 10 times the lenght of the read size (by default = 100000)\n\n\
	-a	Create additional fasta files containing (inactivated by default )\n\n\
	-h	Print this usefull help screen\n\n");

	fprintf (stderr, "EXEMPLES\n\n\
	%s -M -S 200 -L 400 -U 1000 -N 10 -V 10 fasta_file1.fa fasta_file2.fa \n\n", prog_name);
	
	exit (EXIT_FAILURE);
}

/// /////////////////////////////////////////////////////////////////////
/// create_ref_list = import fasta ref genomes within a structure tab
/// /////////////////////////////////////////////////////////////////////

T_info* create_ref_list (char** argv, int start_index, int nb_fasta)
{
	int i, j;
	int n;
	FILE* file = NULL;
	T_info* pHead = NULL;
	T_info* ptri = NULL;
	T_info* ptrj = NULL;
	
	// ADDITION OF A RANDOM REFERENCE
	printf ("Generating a random reference\n\n");
	ptrj = create_info_cell();
	ptrj -> size_name = 16;
	ptrj -> name = "random_reference";
	ptrj -> size_seq = SIZE_OF_RANDOM_REF;
	ptrj -> seq = generate_random_ref(SIZE_OF_RANDOM_REF);
	ptrj -> next = NULL;
	
	pHead = ptrj; // the list head now point to the adress of the first cell until the end of the function
	ptri = ptrj; // ptri point to the first cell for now but will move latter
	
	if (nb_fasta > 0) // si l'utilisateur a indiqué des fichiers fasta en paramètre
	{
		// DISCOVERY AND ADDITION OF REFERENCES CONTAINED IN THE REFERENCE FASTA FILES
		for (i = 0; i < nb_fasta; i++) // for each reference fasta file
		{
			printf ("Processing File %s\n", argv[start_index + i]);
			file = init_file_ptr( argv[start_index + i], "r"); // open the current fasta file
			
			n = nref(file); // output the number of ref seq
			printf("\tThe file contains %d reference sequence(s)\n", n);
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
				ptrj -> seq =  output_seq(file, ptrj -> size_seq) ;
				ptrj -> next = NULL;
				// Link the new cell to the previous one and push forward the active cell pointer
				ptri -> next = ptrj;
				ptri = ptrj;
			}
			fclose(file);
		}				
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
	if (cell == NULL) error_and_exit();
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
	int c, i, j;
	char* seq;
	int found;
	int max_index;
	
	// if mask repeat is true lowercase characters in DNA_BASES will not be considered
	// else lowercase characters will also be included in the reference sequences
	max_index = (MASK_REPEAT == 1) ? 4 : 8;
	i = 0;
	
	seq = malloc_char_string (size + 1);
	
	while (((c = fgetc(file)) != '>') && (c != EOF)) // continue until the next sequence or the end of file
	{
		if (isalpha(c)) // count only alphabetical characters
		{
			seq[i] = 'N';
			
			for (j = 0; j < max_index; j++) // for all possible DNA bases
			{
				if (c == DNA_BASES[j])
					seq[i] = toupper(c);
			}
			i++;
		}
	}
	seq [size] = '\0'; // add /0 at the end of line
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
	
	seq = malloc_char_string (size + 1);
	
	for (i = 0; i < size; i++)
		seq[i] = DNA_BASES [rand() % 4];
	
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
		
		ptri = ptri -> next;
		
	}
	return;
}

/// /////////////////////////////////////////////////////////////////////
/// generate_fastq = generate fastq pairs from ref genomes + scramble
/// /////////////////////////////////////////////////////////////////////

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
	FILE* file3 = NULL;
	FILE* file4 = NULL;
		
	ptri = list_head;
	file1 = init_file_ptr( "CONTROL_R1.fastq", "w"); // File for writing forward fastq reads
	file2 = init_file_ptr( "CONTROL_R2.fastq", "w"); // File for writing reverse fastq reads
	
	if (FASTA == 1)
	{
		file3 = init_file_ptr( "CONTROL_R1.fa", "w"); // File for writing forward fasta reads
		file4 = init_file_ptr( "CONTROL_R2.fa", "w"); // File for writing reverse fasta reads
	}
	
	srand (time(NULL)); // seed value for rand
	
	while (ptri != NULL) // For each reference in the list
	{
		printf ("Processing reference %s\n", ptri -> name);
		forward_read = malloc_char_string (SIZE_READ + 1);
		reverse_read =  malloc_char_string (SIZE_READ + 1);
		forward_quality = malloc_char_string (SIZE_READ + 1);
		reverse_quality =  malloc_char_string (SIZE_READ + 1);
		
		for (i = 0; i < NB_PAIR_BY_REF; i++)  // For i couple per reference
		{
			// If it is possible to select a valid pair
			if (select_read_pair (ptri, &forward_read, &reverse_read) == 1)
			{					
				generate_random_quality (&forward_quality, &reverse_quality);
				fprintf(file1, "@CONTROL_%s:%d\n%s\n+\n%s\n", ptri -> name, i+1, forward_read, forward_quality);
				fprintf(file2, "@CONTROL_%s:%d\n%s\n+\n%s\n", ptri -> name, i+1, reverse_read, reverse_quality);
				if (FASTA == 1)
				{
					fprintf(file3, ">CONTROL_%s:%d\n%s\n", ptri -> name, i+1, forward_read);
					fprintf(file4, ">CONTROL_%s:%d\n%s\n", ptri -> name, i+1, reverse_read);
				}
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
	if (FASTA == 1)
	{
		fclose (file3);
		fclose (file4);
	}
}	

////////////////////////////////////////////////////////////////////////
// select_read_pair = sample a valid read pair in the ref sequence if possible
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
			// If a N is found in the sequence try to resample an other pair somewhere else
			if ((ptri -> seq[j] == 'N') || (ptri -> seq[k] == 'N'))
				break;
			
			// If no N is found in the sequence, the reading of the sequence can continue
			else
			{
				p_forward_read [0][i] = forward_seq(ptri->seq[j]);
				p_reverse_read [0][i] = reverse_seq(ptri->seq[k]);
				valid_pair = 1;
			}
		}
	} while ((valid_pair == 0) && (try < MAX_TRY_VALID_PAIR));

	if (valid_pair == 1) // if a valid pair had been found return 1 and the valid pair
	{
		//printf ("%d tries needed", try);
		p_forward_read[0][SIZE_READ] = '\0';
		p_reverse_read[0][SIZE_READ] = '\0';
		return 1;
	}
	else // if no valid pair was found return 0
	{
		*p_forward_read = NULL;
		*p_reverse_read = NULL;
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////
// forward_seq = return DNA base including eventual errors
////////////////////////////////////////////////////////////////////////
 
char forward_seq(char c)
{
	// if a random frequency is above the error frequency = > no error will be added in the sequence
	if  ((ERR_FREQ == 0) || ((rand()) >= ((RAND_MAX) * ERR_FREQ)))
		return c;		

	else // a random error will be added in the sequence
	{
		MUTATION ++;
		switch (c)
		{
		  case 'A' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'T';
			  case 1 :
				return 'C';
			  case 2 :
				return 'G';
			}
		  case 'T' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'A';
			  case 1 :
				return 'C';
			  case 2 :
				return 'G';
			}
		  case 'C' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'A';
			  case 1 :
				return 'T';
			  case 2 :
				return 'G';
			}
		  case 'G' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'A';
			  case 1 :
				return 'T';
			  case 2 :
				return 'C';
			}
		}
	}
	return 'X';
}

////////////////////////////////////////////////////////////////////////
// reverse_seq = return DNA complementary base including eventual errors
////////////////////////////////////////////////////////////////////////
 
char reverse_seq(char c)
{
	// if a random frequency is above the error frequency = > no error will be added in the sequence
	if  ((ERR_FREQ == 0) || ((rand()) >= ((RAND_MAX) * ERR_FREQ)))
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
		}
	}
	else // a random error will be added in the sequence
	{
		MUTATION ++;
		switch (c)
		{
		  case 'A' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'A';
			  case 1 :
				return 'C';
			  case 2 :
				return 'G';
			}
		  case 'T' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'T';
			  case 1 :
				return 'C';
			  case 2 :
				return 'G';
			}
		  case 'C' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'T';
			  case 1 :
				return 'C';
			  case 2 :
				return 'A';
			}
		  case 'G' :
			switch (rand() % 3)
			{
			  case 0 :
				return 'T';
			  case 1 :
				return 'G';
			  case 2 :
				return 'A';
			}
		}
	}
	return 'X';
}

////////////////////////////////////////////////////////////////////////
// generate_random_quality
////////////////////////////////////////////////////////////////////////

void generate_random_quality (char** p_forward_quality, char** p_reverse_quality)
{
	int i;
	
	for (i = 0 ; i < SIZE_READ; i++)  // generate random quality string for each position in the read
	{ 
		p_forward_quality[0][i] = 35 + rand() % (MAX_QUAL - MIN_QUAL +1) + (MIN_QUAL-1);
		p_reverse_quality[0][i] = 35 + rand() % (MAX_QUAL - MIN_QUAL +1) + (MIN_QUAL-1);
	}
	p_forward_quality[0][SIZE_READ] = '\0';
	p_reverse_quality[0][SIZE_READ] = '\0';
	
	return;
}

////////////////////////////////////////////////////////////////////////
//  STRUCTURES ET VARIABLES GLOBALES                                  //
////////////////////////////////////////////////////////////////////////


typedef struct info T_info;
struct info
{
	char* name;
	int size_name;
	char* seq;
	int size_seq;
	T_info* next;
};

const int MASK_REPEAT = 1; //if 1 pairs will not be chosen in lowercase sequences
const int SIZE_READ = 150;
const int SIZE_MIN_SONIC = 200;
const int SIZE_MAX_SONIC = 600;
const int NB_PAIR_BY_REF = 10;
const int SIZE_OF_RANDOM_REF = 1000;
const char QUALITY_SCORE [] =  "ABCDEFGHIJ"; // high quality scores
// Illumina 1.8 = #$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ
const char DNA_BASES [] =  "ACTG";

////////////////////////////////////////////////////////////////////////
//  PROTOTYPES                                                        //
////////////////////////////////////////////////////////////////////////

void usage (char*, int);
T_info* create_ref_list (char**, int);
void print_list (T_info*);
void generate_fastq (T_info*);

FILE* init_file_ptr (char*, char*);
char complementary (char);
char* generate_random_ref (int);
void verify_ref_size (int, int);
int nref (FILE*);
T_info* create_info_cell(void);
int evaluate_name_size (FILE*);
int evaluate_seq_size (FILE*);
char* output_name (FILE*, int);
char* output_seq (FILE*, int);
void generate_sequences (FILE*, FILE*, T_info*);

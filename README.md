fastq_control_sampler
======================

This program generate control fastq files R1 and R2 from fasta reference sequences.
In addition to the fasta reference provided by the users a scramble reference is also generated.
These control files can be used to develop new bioinformatics pipelines for NGS. 
Fasta files can contain multiple reference sequences if each sequence is prececed by the name of the reference begining by the symbol ">"
Undetermined DNA bases identified with 'N' in reference sequences are excluded from the regions were fastq are generated.
By default the lowercase DNA bases that usually indicates repeated regions are also masked.

IF THE REFERENCE SEQUENCES CONTAIN LOWERCASE CHARACTERS THE WILL BE EXCLUDED OF THE CONTROL FASTQ FILES.
BEWARE OF USING UPPERCASE CHARACTERS FOR THE PART OF THE REFERENCE SEQUENCES THAT ARE NEEDED IN THE FASTQ FILES
Alternatively users can modify the value MASK_REPEAT to 0 in the header file before compilation

The default settings are suitable for illumina 2*150 pb sequencing with a classical trueseq preparation of the library 
Users can edit the global variables contains in the header file to ajust the settings to their experimental procedure.
 


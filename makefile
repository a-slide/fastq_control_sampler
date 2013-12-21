############################# VARIABLES ################################

CC = gcc
	# Compilateur
	
CFLAGS = -g -Wall -W -Wno-unused-parameter
	# Options de compilation
	# -Wall -W et -Wno-unused-parameter pour apporter des informations

LDFLAGS = 
	# Options d'edition de lien

BIN = fastq_control_sampler
	# Nom des executables a creer


#################### INSTRUCTIONS DE COMPILATION #######################
# $@ =  Cible # $^ = liste des dépendances # $< Première dépendance #

all: $(BIN)

fastq_control_sampler: fastq_control_sampler.o ptr_allocation.o
	$(CC) $^ $(LDFLAGS) -o $@
	# Edition de lien a partir des fichiers objets


fastq_control_sampler.o: fastq_control_sampler.c fastq_control_sampler.h
	$(CC) -c $< $(CFLAGS) -o $@
	# Compilation de $<
	
ptr_allocation.o: ptr_allocation.c ptr_allocation.h
	$(CC) -c $< $(CFLAGS) -o $@ 
	# Compilation de $<

##################### INSTRUCTIONS DE NETTOYAGE ########################

.PHONY: clean mrproper
	# PHONY = Dependances systematiquement reconstruites

clean:
	rm -f ptr_allocation.o fastq_control_sampler.o
	# Supprimer tous les fichiers intermédiaires

mrproper: clean
	rm -rf $(BIN) $(OBJ)
	# Supprimer tout ce qui peut être régénéré et reconstruit complètement

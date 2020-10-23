# 
# Makefile for automatic building
#
# Written by @Jerome Dh
#

# Particularies cibles
.PHONY: clean mrproper

# Desactivating the implicites rules
.SUFFIXES:

# Config
WITH_OPENMP = yes

# Variables
# CC = gcc
CC = mpicc
SOURCES_DIR = sources
CFLAGS = -I headers -W -Wall -v 

ifeq ($(WITH_OPENMP), yes)
	OMP_FLAGS = -fopenmp
else
	OMP_FLAGS =
endif

# Output
all: main.o errors.o html5_template.o http2.o server.o utils.o concurency.o
	$(CC) $(OMP_FLAGS) $^ -o server

html5_template.o: $(SOURCES_DIR)/html5_template.c
	$(CC) $^ -c $(CFLAGS)

utils.o: $(SOURCES_DIR)/utils.c
	$(CC) $^ -c $(CFLAGS)

errors.o: $(SOURCES_DIR)/errors.c
	$(CC) $^ -c $(CFLAGS)

concurency.o: $(SOURCES_DIR)/concurency.c
	$(CC) $^ -c $(CFLAGS)

http2.o: $(SOURCES_DIR)/http2.c
	$(CC) $^ -c $(CFLAGS)

server.o: $(SOURCES_DIR)/server.c
	$(CC) $^ -c $(CFLAGS)

main.o: $(SOURCES_DIR)/main.c
	$(CC) $^ -c $(CFLAGS)

# Deleting temporary files
clean:
	rm -rf *.o
	rm -rf server

# Deleting executable file "server"
mrproper:
	rm -rf server

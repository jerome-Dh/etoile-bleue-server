/**
 * Perform concurencies operations
 * We use here MPI and OMP
 * 
 * @author Jerome Dh
 * @date 19/09/2020
 */

#ifndef CONCURENCY_H
#define CONCURENCY_H

#include "config.h"

#define MAT_MULT    "/matmult"
#define SEARCH      "/search"

typedef enum
{
    _MAT_MULT,
    _SEARCH,

} CONCURRENCY_OP;

// The keywords of query search
typedef struct Keyword Keyword;
struct Keyword {
    char chr[BUF_SIZE];
    Keyword *next;
};

// List of keywords
typedef struct KeywordList KeywordList;
struct KeywordList
{
    Keyword *first;
};

typedef struct MPI_RESULT MPI_RESULT;
struct MPI_RESULT
{
    int p; // Processes length
    int q; // Threads length
    int status; // Return value
};

// Matrix multiplication
void matmult(const char *query, char *result);

// Execute the MPI program for matrix
MPI_RESULT execute_mpi_matrix(const char *query);

// Search in DB by keyword
void db_search(const char *query, char *result);

// Get all keywords of query
void get_keywords(KeywordList *keyword_list, const char *query);

// Helper to add a string to keywords list
void add_to_keyword(KeywordList *keyword_list, const char *str);

#endif  // CONCURENCY_H
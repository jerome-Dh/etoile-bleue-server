/**
 * Query representation for HTTP/2
 */ 

#ifndef QUERY_H
#define QUERY_H

#include "config.h"

// Sting representation of a query
typedef struct Query Query;
struct Query {
    char chr[BUF_SIZE];
    Query *next;
};

#endif // QUERY_H
/**
 * Http/2 protocol header
 * 
 * This is the http/2 implementation protocol to use in both server and client http
 * 
 * @author Jerome Dh
 * @date 07/09/2020
 */

#ifndef HTTP2_H
#define HTTP2_H

#include "utils.h"
#include "concurency.h"

/**
 * Parse the request to get informations
 */ 
REQUEST_STATE parseRequest(Query *query, Request *request);

/**
 * Perform operation
 */ 
int performOperation(Request *request, Response *response);

// Perform operation by concurrence
int perform_concurrency(Request *request, Response *response, const CONCURRENCY_OP op);

// Perform file access
int perform_file_access(Request *request, Response *response);

/**
 * Perform for error
 */ 
int performError(const REQUEST_STATE state, Response *response);

/**
 * Compose response to send to client
 */ 
void composeResponse(const Request *request, Response *response);

/**
 * Parse the line to find verb, uri and protocol conformly to HTTP/2
 */ 
static REQUEST_STATE parse_verb_and_protocol(char *line, Request *request, const int M);

/**
 * Parse the line to find host conformly to HTTP/2
 */ 
static REQUEST_STATE parse_host(char *line, Request *request, const int M);

/**
 * Parse the line to find others keywords conformly to HTTP/2
 */
static REQUEST_STATE parse_others(char *line, Request *request, const int M);

/**
 * Convert a Response to char* to send it to the client
 */
int response_to_string(const HTTP_VERB verb, const Response *response, char *str);

#endif // HTTP2_H
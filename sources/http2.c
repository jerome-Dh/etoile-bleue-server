/**
 * HTTP/2 Core implementation
 * 
 * @see http2.h
 * @author Jerome Dh
 * @date 10/09/2020 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "http2.h"
#include "errors.h"
#include "html5_template.h"

/**
 * Parse the request to get informations
 */ 
REQUEST_STATE parseRequest(Query *query, Request *request)
{

    Query *p = query;
    int k = 0;

    // Examine every query line
    while(p != NULL) {
        
        char line[BUF_SIZE];

        strncpy(line, p->chr, BUF_SIZE);
        remove_crlf(line, BUF_SIZE); // Remove CRLF

        REQUEST_STATE status;
        switch (k)
        {
        case 0:
            /*  //  k=0; check verb, uri and protocol version */
            status = parse_verb_and_protocol(line, request, BUF_SIZE);
            if(status != _OK) {
                // printf("Invalid: %s\n", line);
                return status;
            }
            break;

        case 1:
            /*  //  k=1; check hostname */
            status = parse_host(line, request, BUF_SIZE);
            if(status != _OK) {
                // printf("Invalid: %s\n", line);
                return status;
            }
            break;
       
        default:

             /*  //  k>1; check others params (Accept, Keep-Alive, ..) */
            status = parse_others(line, request, BUF_SIZE);
            if(status != _OK) {
                printf("Invalid: %s\n", line);
                return status;
            }
            break;
        }

        // Go to the next line
        k++;
        p = p->next;
    }

    return k < 2 ? _BAD_REQUEST : _OK;

}

/**
 * Perform for HTTP error
 */ 
int performError(const REQUEST_STATE state, Response *response)
{
    int ret = 0;

    if(response == NULL){
        return ret;
    }

    // Initialize content
    response->content = malloc(sizeof(char) * response->error_content_size);

    // Check state
    switch (state)
    {
    case _BAD_REQUEST:

        response->status = _400; // Status code
        strncpy(response->status_text, "400 Bad Request", BUF_SIZE); // Status Text
        response->content_type = _TEXT_HTML; // Content-Type
        ret = get_400_error(response->content, response->error_content_size); // Content HTML
        break;

    case _UNAUTHORIZED:
        /* code */
        break;

    case _FORBIDDEN:
        /* code */
        break;

    case _NOT_FOUND:
       
        response->status = _404;
        strncpy(response->status_text, "404 Not Found", BUF_SIZE);
        response->content_type = _TEXT_HTML;
        ret = get_404_error(response->content, response->error_content_size);
        break;

    case _INTERNAL_SERVER_ERROR:

        response->status = _500;
        strncpy(response->status_text, "500 Internal Server Error", BUF_SIZE);
        response->content_type = _TEXT_HTML;
        ret = get_500_error(response->content, response->error_content_size);
        break;

    case _NOT_IMPLEMENTED:
        response->status = _501;
        strncpy(response->status_text, "501 Not Implemented", BUF_SIZE);
        response->content_type = _TEXT_HTML;
        ret = get_501_error(response->content, response->error_content_size);
        break;

    case _BAD_GATEWAY:
        /* code */
        break;

    case _SERVICE_UNAVAILABLE:
        /* code */
        break;
    
    default:
        /* _INTERNAL_SERVER_ERROR */
        break;
    }

    return ret;

}

/**
 * Perform operations with MPI/OMP modules : Parallel code
 */ 
int performOperation(Request *request, Response *response)
{
    int ret = 0;

    // Request type is GET or HEAD
    if(request->verb == _GET || request->verb == _HEAD) {

        //// MPI operations
        if( ! strncmp(request->uri, MAT_MULT, strlen(MAT_MULT))
            || ! strncmp(request->uri, SEARCH, strlen(SEARCH)))
        {
            CONCURRENCY_OP op = ! strncmp(request->uri, MAT_MULT, strlen(MAT_MULT))? _MAT_MULT : _SEARCH;
            ret = perform_concurrency(request, response, op);
        }
        else { // Others operations, such as GET, HEAD of file content

            ret = perform_file_access(request, response);

        }
    }

    // Request type is POST
    else if(request->verb == _POST) {

        ret = performError(_NOT_IMPLEMENTED, response);
    }

    // Not implemented
    else {
        ret = performError(_NOT_IMPLEMENTED, response);
    }

    return ret;

}

/**
 * Perform operation by concurrence
 * Using MPI/OMP lib
 */ 
int perform_concurrency(Request *request, Response *response, const CONCURRENCY_OP op)
{
    // Matrice multiplication
    size_t CONTENT_LENGTH = BUF_SIZE * 100;
    char *content = calloc(sizeof(char), CONTENT_LENGTH);
    char *title = calloc(sizeof(char), BUF_SIZE);
 
    switch (op)
    {
    case _MAT_MULT:

        matmult(request->uri, content);
        strcpy(title,  "Operations sur les matrices");
        break;

    case _SEARCH:

        db_search(request->uri, content);
        strcpy(title,  "Recherche dans la BD");
        break;

    default:
        break;
    }

    response->status = _200;
    strncpy(response->status_text, "200 OK", 7);
    response->content_type = _TEXT_HTML;

    // Templating with HTML5
    templating(title, content, 1, CONTENT_LENGTH);

    size_t size = strlen(content);
    response->content_length = size;
    response->content = calloc(sizeof(char), size);
    strncpy(response->content, content, size);

    return 1;
}

/**
 * Perform file access
 * Such as to get file content in the www dir
 */ 
int perform_file_access(Request *request, Response *response)
{
    int ret = 1;

    // If the uri is "/", added index.html
    if( ! strncmp(request->uri, "/", BUF_SIZE)) {
        char *default_page = "index.html";
        strncat(request->uri, default_page, strlen(default_page));
    }

    // Get the www dir name
    char filename[BUF_SIZE];
    get_www_dir(filename);

    // Adding the filename
    strncat(filename, request->uri, BUF_SIZE - strlen(filename) - 1);
    // printf("DIR: %s\n", dir);

    // Get the file size
    size_t size = file_size(filename);
    if(size == (size_t)-1)
    {
        ret = performError(_NOT_FOUND, response);
    }
    else
    {
        // printf("size: %ld\n", size);
        response->status = _200;
        strncpy(response->status_text, "200 OK", BUF_SIZE);
        response->content_type = get_content_type_of_file(filename);

        // Check if filename is a file as image, set the filename either the content
        switch (response->content_type)
        {
        case _IMAGE_PNG:
        case _IMAGE_JPG:
        case _IMAGE_JPEG:
        case _IMAGE_GIF:
        case _IMAGE_ALL:
            response->content_length = size;
            response->filename = calloc(sizeof(char), BUF_SIZE);
            strncpy(response->filename, filename, BUF_SIZE);
            break;

        default:
            response->content = calloc(sizeof(char), size);
            size_t r = get_file_content(filename, response->content);
            break;
        }
    }

    return ret;
}

/**
 * Compose response to send to client
 */ 
void composeResponse(const Request *request, Response *response)
{

    if(request == NULL || response == NULL) {
        return;
    }

    // Adding extras informations to response
    response->version = _HTTP_2;

    // Time
    time_t now = time(NULL);
    char* ctm = ctime(&now);
    remove_crlf(ctm, strlen(ctm));;
    strncpy(response->date, ctm, BUF_SIZE);

    // Server name
    strcpy(response->server, SERVER_NAME);
    strcat(response->server, "/");
    strcat(response->server, SERVER_VERSION);
    strcat(response->server, ".");
    strcat(response->server, SERVER_PATCH);

    // Last modified
    strncpy(response->last_modified, ctm, BUF_SIZE);

    // ETag
    char etag[BUF_SIZE];
    sprintf(etag, "%ld", time(NULL));
    strncpy(response->etag, etag, BUF_SIZE);

    // Vary
    strncpy(response->vary, "Accept-Encoding", BUF_SIZE);

    // Accept-Range
    strncpy(response->accept_range, "bytes", BUF_SIZE);

    // Content-lenght
    if(response->content != NULL) 
    {
        response->content_length = strlen(response->content);
    }

}

/**
 * Convert a Response to string (char*) to send it to the client
 */
int response_to_string(const HTTP_VERB verb, const Response *response, char *str)
{

    if(response == NULL || str == NULL) {
        return 0;
    }

    // Header Line 1: HTTP Version, status text
    char version[VERSION_LENGTH];
    version_to_string(response->version, version);
    strncpy(str, version, VERSION_LENGTH);
    strncat(str, " ", 1);
    strncat(str, response->status_text, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF)); // Line end

    // Header Line 2: Date
    strncat(str, "Date: ", 6);
    strncat(str, response->date, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 3: Server
    strncat(str, "Server: ", 8);
    strncat(str, response->server, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 4: Last modified
    strncat(str, "Last-Modified: ", 15);
    strncat(str, response->last_modified, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 5: ETag
    strncat(str, "ETag: ", 6);
    strncat(str, response->etag, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 6: Accept-Range
    strncat(str, "Accept-Range: ", 14);
    strncat(str, response->accept_range, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 6: Accept-Range
    strncat(str, "Vary: ", 6);
    strncat(str, response->vary, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 7: Content-Length
    long int length = response->content_length;
    char size[BUF_SIZE];
    sprintf(size, "%ld", length);
    strncat(str, "Content-Length: ", 16);
    strncat(str, size, BUF_SIZE);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 8: Content-Type
    char ct_type[CONTENT_TYPE_LENGTH];
    content_type_to_string(response->content_type, ct_type);
    strncat(str, "Content-Type: ", 14);
    strncat(str, ct_type, CONTENT_TYPE_LENGTH);
    strncat(str, CRLF, strlen(CRLF));

    // Header Line 9: Connection state
    strncat(str, "Connection: close", 18);
    strncat(str, CRLF, strlen(CRLF));
    strncat(str, CRLF, strlen(CRLF));

    // Body: Content
    if(verb != _HEAD)
    {
        if(response->content_length > 0 && response->content != NULL) 
        {
            strncat(str, response->content, response->content_length);
        }
    }

    return 1;
}

/**
 * Get the HTTP verb, uri and protocol in the text line
 */ 
static REQUEST_STATE parse_verb_and_protocol(char *line, Request *request, const int M)
{
    char tempo[M];
    long diff, total = 0;
    int k = 0;

    strncpy(tempo, line, M);
    do {
        if( ! strncmp(tempo, " ", M)) {
            return _UNKNOWN;
        }
        char *token;
        token = strtok(tempo, " ");
        remove_crlf(token, M);

        // Getters
        switch (k)
        {
        case 0:
            /*  Get the verb */
            request->verb = string_to_action(token);

            if(request->verb == _NOT_SUPPORTED){
                return _BAD_REQUEST;
            }

            break;

        case 1:
            /* Get the uri */
            if(token[0] != '/') {
                return _BAD_REQUEST;
            }

            strncpy(request->uri, token, M);

            break;

        case 2:

            /* Get the HTTP version */
            request->version = string_to_version(token);

            if(request->version == _VERSION_ERROR) {
                return _BAD_REQUEST;
            }

            break;

        default:

            return _BAD_REQUEST;
        }

        // Go to the next token
        total += strlen(token);
        diff = strlen(line) - total;

        if(diff > 1) {
            total++;
            char* pt = line + total;
            strncpy(tempo, pt, M);
            k++;
        }
    }
    while (diff > 1);

    return k == 2 ? _OK : _BAD_REQUEST;

}

/**
 * Parse the line to find hostname conformly to HTTP/2
 */ 
static REQUEST_STATE parse_host(char *line, Request *request, const int M)
{
    char tempo[M];
    long diff, total = 0;
    int k = 0;

    strncpy(tempo, line, M);

    do {
        if( ! strncmp(tempo, " ", M)) {
            return _BAD_REQUEST;
        }
        char *token;
        token = strtok(tempo, " ");
        remove_crlf(token, M);

        // Getters
        switch (k)
        {
        case 0:
            /*  Check the "Host:" keyword */
            if(strncmp(token, HOST_KEYWORD, M)) { // "Host:" Keyword Not found
                // printf("ERROR: %s\n", token);
                return _BAD_REQUEST;
            }
            break;

        case 1:
            /* Get the host name */
            if( ! ckeck_hostname(token)) {
                // printf("ERROR: %s\n", token);
                return _BAD_REQUEST;
            }

            strncpy(request->host, token, M);

            break;

        default:
            // printf("ERROR: OUT\n");
            return _BAD_REQUEST;
        }

        // Go to the next token
        total += strlen(token);
        diff = strlen(line) - total;

        if(diff > 1) {
            total++;
            char* pt = line + total;
            strncpy(tempo, pt, M);
            k++;
        }
    }

    while (diff > 1);

    return k == 1 ? _OK : _BAD_REQUEST;

}

/**
 * Parse the line to find others keywords conformly to HTTP/2
 */
static REQUEST_STATE parse_others(char *line, Request *request, const int M) 
{
    char tempo[M];
    long diff, total = 0;
    int k = 0;

    strncpy(tempo, line, M);

    do {
        if( ! strncmp(tempo, " ", M)) {
            return _UNKNOWN;
        }
        char *token;
        token = strtok(tempo, ":"); // Couple "Name: value"
        remove_crlf(token, M);

        if(token == NULL) {
            return _BAD_REQUEST;
        }

        /**
         *  Get the informations
         */

        // Check Content-type
        if( ! strncmp(token, "Accept", M)) {
            request->content_type = string_to_content_type(token);
        }

        // Go to the next token
        total += strlen(token);
        diff = strlen(line) - total;

        if(diff > 1) {
            total++;
            char* pt = line + total;
            strncpy(tempo, pt, M);
            k++;
        }
    }
    while (diff > 1);

    return (k == 0 || k >= 1) ? _OK : _BAD_REQUEST;
}
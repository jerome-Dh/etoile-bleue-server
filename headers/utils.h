/**
 * Utilities for HTTP implementations on the server side
 * 
 * @author Jerome Dh
 * @date 08/09/2020
 */

#ifndef UTILS_H
#define UTILS_H

#include "query.h"

#ifdef WIN32
#define clear() system("cls")
#else
#define clear() system("clear")
#endif

// HTTP versions
#define HTTP_1_0 "HTTP/1.0"
#define HTTP_1_1 "HTTP/1.1"
#define HTTP_1_2 "HTTP/1.2"
#define HTTP_2 "HTTP/2"
#define VERSION_ERROR "VERSION ERROR"
#define VERSION_LENGTH 20

// HTTP Methods
#define GET "GET"
#define POST "POST"
#define HEAD "HEAD"
#define DELETE "DELETE"
#define PUT "PUT"
#define OPTIONS "OPTIONS"
#define CONNECT "CONNECT"
#define TRACE "TRACE"
#define NOT_SUPPORTED_VERB "NOT SUPPORTED"
#define VERB_LENGTH 20

// HTTP Headers parameters
#define From "From:"
#define Accept "Accept:"
#define Date "Date:"
#define Referer "Referer:"
#define Pragma "Pragma:"
#define Server "Server:"
#define Location "Location:"
#define User_Agent "User-Agent:"

#define Content_Type "Content-Type:"
#define Content_Length "Content-Length:"
#define Content_Encoding "Content-Encoding:"

#define Expires "Expires:"
#define Keep_Alive "Keep-Alive:"
#define Connection "Connection:"
#define Last_Modified "Last-Modified:"
#define If_Modified_Since "If-Modified-Since:"

#define HOST_KEYWORD "Host:"

// Types of content-type of files
#define TEXT_PLAIN  "text/plain"
#define TEXT_HTML   "text/html"
#define IMAGE_PNG   "image/png"
#define IMAGE_JPG   "image/jpg"
#define IMAGE_JPEG  "image/jpeg"
#define IMAGE_GIF   "image/gif"
#define IMAGE_ALL   "image/*"
#define OTHER_CONTENT_TYPE "*/*"
#define CONTENT_TYPE_LENGTH 100

#define ERROR_MAX_SIZE 4096
#define HEAD_MAX_SIZE 4096

// HTTP actions verbs 
typedef enum { _GET, _POST, _PUT, _DELETE, _HEAD, _TRACE, _OPTIONS, _CONNECT, _NOT_SUPPORTED } HTTP_VERB;

// HTTP protocol versions
typedef enum { _HTTP_1_0, _HTTP_1_1, _HTTP_1_2, _HTTP_2, _VERSION_ERROR } HTTP_VERSION;

// The content types of the requests/responses
typedef enum { 
    _TEXT_PLAIN, 
    _TEXT_HTML,
    _IMAGE_PNG,
    _IMAGE_JPG,
    _IMAGE_JPEG,
    _IMAGE_GIF,
    _IMAGE_ALL,
    _OTHERS_CONTENT_TYPE,
} CONTENT_TYPE;

// State of a HTTP request
typedef enum { 
    _OK,
    _BAD_REQUEST,
    _UNAUTHORIZED, 
    _FORBIDDEN,
    _NOT_FOUND,
    _INTERNAL_SERVER_ERROR,
    _NOT_IMPLEMENTED,
    _BAD_GATEWAY,
    _SERVICE_UNAVAILABLE,
    _UNKNOWN,

} REQUEST_STATE;

// HTTP status code
typedef enum {
    _100=100,

    _200=200, // Ok
    _201=201, // Created
    _202=202, // Accepted
    _204=204, // No Content

    _300=300, // Multiple Choices
    _301=301, // Moved Permanently
    _302=302, // Moved Temporarily

    _400=400, // (Bad Request)
    _401=401, // (Unauthorized)
    _403=403, // (Forbidden)
    _404=404, // (Not Found)

    _500=500, // (Internal Server Error)
    _501=501, // (Not Implemented)
    _502=502, // (Bad Gateway)
    _503=503, // (Service Unavailable)

    _OTHERS_STATUS_CODE,

} STATUS_CODE;

// HTTP Request format
typedef struct
{
    HTTP_VERB verb;
    char uri[BUF_SIZE];
    HTTP_VERSION version;
    char host[BUF_SIZE];
    CONTENT_TYPE content_type;
    Query* others;
 
} Request;

// HTTP Response format
typedef struct
{
    HTTP_VERSION version;
    STATUS_CODE status;
    char status_text[BUF_SIZE];
    char date[BUF_SIZE];
    char server[BUF_SIZE];
    char last_modified[BUF_SIZE];
    char etag[BUF_SIZE];
    char accept_range[BUF_SIZE];
    char vary[BUF_SIZE];
    unsigned long content_length;
    CONTENT_TYPE content_type;

    char *filename;
    char *content;
    unsigned long error_content_size;

} Response;

// Convertion between ACTION verbs and his ENUM 
void action_to_string(const HTTP_VERB verb, char* convert);
HTTP_VERB string_to_action(const char *verb);

// Conversion between protocol versions and his ENUM
void version_to_string(const HTTP_VERSION version, char* convert);
HTTP_VERSION string_to_version(const char *version);

// Convertion between content-type and his ENUM
void content_type_to_string(const CONTENT_TYPE content_type, char* convert);
CONTENT_TYPE string_to_content_type(const char *content_type);

// Transform end "\n" character to NULL
void remove_crlf(char* chr, int len);

// Check the host name
int ckeck_hostname(const char* name);

// Get the file size
size_t file_size(const char* filename);

// Get the name of www directory from config
void get_www_dir(char *dir);

// Print all char asci
void print_all_chars(const char* str);

// Get the file content
size_t get_file_content(const char* filename, char *content);

// Get the content type of existing filename
CONTENT_TYPE get_content_type_of_file(const char *filename);

#endif // UTILS_H
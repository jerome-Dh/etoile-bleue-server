/**
 * Utils implementation
 * 
 * @author Jerome Dh
 * @date 09/09/2020
 */

#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "config.h"

/**
 * Convert a HTTP_VERB enum in string
 */ 
void action_to_string(const HTTP_VERB verb, char* convert) 
{
    switch (verb)
    {
    case _GET:
        strncpy(convert, GET, VERB_LENGTH - 1);
        break;
    
    case _POST:
        strncpy(convert, POST, VERB_LENGTH - 1);
        break;

    case _HEAD:
        strncpy(convert, HEAD, VERB_LENGTH - 1);
        break;

    case _PUT:
        strncpy(convert, PUT, VERB_LENGTH - 1);
        break;

    case _DELETE:
        strncpy(convert, DELETE, VERB_LENGTH - 1);
        break;

    case _OPTIONS:
        strncpy(convert, OPTIONS, VERB_LENGTH - 1);
        break;

    case _CONNECT:
        strncpy(convert, CONNECT, VERB_LENGTH - 1);
        break;
    
    case _TRACE:
        strncpy(convert, TRACE, VERB_LENGTH - 1);
        break;

    default:
        strncpy(convert, NOT_SUPPORTED_VERB, VERB_LENGTH - 1);
        break;
    }

}

/**
 * Convert a string action to enum
 */ 
HTTP_VERB string_to_action(const char *verb)
{
    HTTP_VERB convert;

    if( ! strncmp(verb, GET, VERB_LENGTH)) {
        convert = _GET;
    }
    else if( ! strncmp(verb, POST, VERB_LENGTH)) {
        convert = _POST;
    }
    else if( ! strncmp(verb, HEAD, VERB_LENGTH)) {
        convert = _HEAD;
    }
    else if( ! strncmp(verb, PUT, VERB_LENGTH)) {
        convert = _PUT;
    }
    else if( ! strncmp(verb, DELETE, VERB_LENGTH)) {
        convert = _DELETE;
    }
    else if( ! strncmp(verb, CONNECT, VERB_LENGTH)) {
        convert = _CONNECT;
    }
    else if( ! strncmp(verb, OPTIONS, VERB_LENGTH)) {
        convert = _OPTIONS;
    }
    else if( ! strncmp(verb, TRACE, VERB_LENGTH)) {
        convert = _TRACE;
    }
    else {
        convert = _NOT_SUPPORTED;
    }

    return convert;

}

/**
 * Convert HTTP_VERSION enum to an string
 */ 
void version_to_string(const HTTP_VERSION version, char* convert)
{
    
    switch(version) 
    {
    case _HTTP_1_0:
        strncpy(convert, HTTP_1_0, VERSION_LENGTH - 1);
        break;

    case _HTTP_1_1:
        strncpy(convert, HTTP_1_1, VERSION_LENGTH - 1);
        break;

    case _HTTP_1_2:
        strncpy(convert, HTTP_1_2, VERSION_LENGTH - 1);
        break;

    case _HTTP_2:
        strncpy(convert, HTTP_2, VERSION_LENGTH - 1);
        break;

    default:
         strncpy(convert, VERSION_ERROR, VERSION_LENGTH - 1);
        break;
    }

}

/**
 * Convert a string to HTTP_VERSION enum
 */ 
HTTP_VERSION string_to_version(const char *version)
{
    HTTP_VERSION convert;

    if( ! strncmp(version, HTTP_1_0, strlen(HTTP_1_0))) {
        convert = _HTTP_1_0;
    }
    else if( ! strncmp(version, HTTP_1_1, strlen(HTTP_1_1))) {
        convert = _HTTP_1_1;
    }
    else if( ! strncmp(version, HTTP_1_2, strlen(HTTP_1_2))) {
        convert = _HTTP_1_2;
    }
    else if( ! strncmp(version, HTTP_2, strlen(HTTP_2))) {
        convert = _HTTP_2;
    }
    else {
        convert = _VERSION_ERROR;
    }

    return convert;
}

/**
 * Convert Content-Type enum to string
 */ 
void content_type_to_string(const CONTENT_TYPE content_type, char* convert) 
{

    switch (content_type)
    {
    case _TEXT_PLAIN:
        strncpy(convert, TEXT_PLAIN, CONTENT_TYPE_LENGTH - 1);
        strncat(convert, ";charset=UTF-8", strlen(TEXT_PLAIN) - CONTENT_TYPE_LENGTH - 1);
        break;

    case _TEXT_HTML:
        strncpy(convert, TEXT_HTML, CONTENT_TYPE_LENGTH - 1);
        strncat(convert, ";charset=UTF-8", strlen(TEXT_HTML) - CONTENT_TYPE_LENGTH - 1);
        break;

    case _IMAGE_PNG:
        strncpy(convert, IMAGE_PNG, CONTENT_TYPE_LENGTH - 1);
        break;

    case _IMAGE_JPG:
        strncpy(convert, IMAGE_JPG, CONTENT_TYPE_LENGTH - 1);
        break;

    case _IMAGE_JPEG:
        strncpy(convert, IMAGE_JPG, CONTENT_TYPE_LENGTH - 1);
        break;

    case _IMAGE_GIF:
        strncpy(convert, IMAGE_GIF, CONTENT_TYPE_LENGTH - 1);
        break;

    case _IMAGE_ALL:
        strncpy(convert, IMAGE_ALL, CONTENT_TYPE_LENGTH - 1);
        break;

    default:
        strncpy(convert, OTHER_CONTENT_TYPE, CONTENT_TYPE_LENGTH - 1);
        break;
    }

}

/**
 * Convert a string representation of content-type to enum 
 */ 
CONTENT_TYPE string_to_content_type(const char *content_type)
{
    CONTENT_TYPE convert;

    if( ! strncmp(content_type, TEXT_PLAIN, CONTENT_TYPE_LENGTH)) {
        convert = _TEXT_PLAIN;
    }
    else if( ! strncmp(content_type, TEXT_HTML, CONTENT_TYPE_LENGTH)) {
        convert = _TEXT_HTML;
    }
    else if( ! strncmp(content_type, IMAGE_PNG, CONTENT_TYPE_LENGTH)) {
        convert = _IMAGE_PNG;
    }
    else if( ! strncmp(content_type, IMAGE_JPG, CONTENT_TYPE_LENGTH)) {
        convert = _IMAGE_JPG;
    }
    else if( ! strncmp(content_type, IMAGE_JPEG, CONTENT_TYPE_LENGTH)) {
        convert = _IMAGE_JPEG;
    }
    else if( ! strncmp(content_type, IMAGE_GIF, CONTENT_TYPE_LENGTH)) {
        convert = _IMAGE_GIF;
    }
    else if( ! strncmp(content_type, IMAGE_ALL, CONTENT_TYPE_LENGTH)) {
        convert = _IMAGE_ALL;
    }
    else {
        convert = _OTHERS_CONTENT_TYPE;
    }

    return convert;
}

/**
 * Transform "\n" and "\r"  character to null-terminated byte string // crlf
 */ 
void remove_crlf(char* chr, int len) 
{
    char *ptr = strstr(chr, "\n"); // Code char = 10

    if(ptr != NULL) {
        *ptr = 0;
    }
    else {
        chr[len - 1] = 0;
    }

    ptr = strstr(chr, "\r"); // Code char = 13
    if(ptr != NULL) {
        *ptr = 0;
    }

}

/**
 * Ckeck the hostname
 */ 
int ckeck_hostname(const char* name) 
{
    char host_port[BUF_SIZE];
    sprintf(host_port, "%s:%d", HOST_NAME, PORT);
    if( ! strncmp(name, HOST_NAME, strlen(HOST_NAME))
        || ! strncmp(name, host_port, strlen(host_port))) {
        return 1;
    }

    return 0;
}

/** 
 * Printing all characters in the string
 */ 
void print_all_chars(const char* str) {
    
    for(size_t i=0; i <= strlen(str); ++i) {
        printf("%d, ", str[i]);
    }
    printf("\n");
}

/**
 * Get the file size
 * 
 * @param filename
 * @return Number of char in the file or -1 if file not exists
 */
size_t file_size(const char* filename)
{
    FILE *fic = fopen(filename, "r");

    size_t size = 0;

    if(fic != NULL) {

        fseek(fic, 0, SEEK_END);
        size = ftell(fic);
        fseek(fic, 0, SEEK_SET);

        fclose(fic);
    }
    else
    {
        size = -1;
    }

    return size;
}

/**
 * Get the file content
 * @param filename
 * @param content The content to return
 * @return Size of content successful read
 */ 
size_t get_file_content(const char* filename, char *content)
{
    FILE *fic = fopen(filename, "r");
    size_t k = 0;

    if(fic != NULL)
    {
        int c;
        while((c = fgetc(fic)) != EOF){
            content[k++] = c;
        }

        fclose(fic);
    }

    return k;
}

/**
 * Get the www dir name from config file
 * 
 * @param dir The name of dir to return
 */ 
void get_www_dir(char *dir) {

    // The default www dir
    strncpy(dir, DEFAULT_WWW_DIR, BUF_SIZE - 1);

    FILE *fic = fopen(CONFIG_FILE, "r");

    if(fic != NULL) {

        char line[BUF_SIZE];
        while (fgets(line, BUF_SIZE, fic) != NULL)
        {
            /* Caption */
            char *caption = "WWWDir";
            if( ! strncmp(caption, line, strlen(caption))) { // Caption found !

                remove_crlf(line, BUF_SIZE);
                char *shift = line + strlen(caption) + 1;
                strncpy(dir, shift, BUF_SIZE - strlen(caption) - 1);
                break;

            }
        }

        fclose(fic);
    }

}

/**
 * Get the content type of a filename
 */ 
CONTENT_TYPE get_content_type_of_file(const char *filename)
{
    CONTENT_TYPE contentType = _OTHERS_CONTENT_TYPE;

    if(filename != NULL)
    {
        char *pt = strrchr(filename, '.');
        if(pt != NULL)
        {
            if( ! strcmp(pt, ".html")){
                contentType = _TEXT_HTML;
            }
            else if( ! strcmp(pt, ".txt")) {
                contentType = _TEXT_PLAIN;
            }
            else if( ! strcmp(pt, ".png")) {
                contentType = _IMAGE_PNG;
            }
            else if( ! strcmp(pt, ".jpg")) {
                contentType = _IMAGE_JPG;
            }
            else if( ! strcmp(pt, ".jpeg")) {
                contentType = _IMAGE_JPEG;
            }
            else if( ! strcmp(pt, ".gif")) {
                contentType = _IMAGE_GIF;
            }
            else if( ! strcmp(pt, ".tiff") || ! strcmp(pt, ".ico")) {
                contentType = _IMAGE_ALL;
            }
        }
    }

    return contentType;
}
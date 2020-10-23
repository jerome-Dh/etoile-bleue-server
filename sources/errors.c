/**
 * HTTP Errors implementation
 * 
 * @author Jerome Dh
 * @date 12/09/2020
 */

#include <string.h>
#include "errors.h"
#include "html5_template.h"

/**
 * The 400 error
 */ 
int get_400_error(char* html, const int size)
{
    char* title = "400 Bad Request";
    char *tmp = "<h1>Bad Request</h1>\n\
<p>Your browser sent a request that this server could not understand.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 404 error
 */ 
int get_401_error(char* html, const int size)
{
    char* title = "401 Unauthorized";
    char *tmp = "<h1>Unauthorized</h1>\n\
<p>Your request is unauthorized on this server.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 404 error
 */ 
int get_403_error(char* html, const int size)
{
    char* title = "403 Forbidden";
    char *tmp = "<h1>Forbidden</h1>\n\
<p>Your request is forbidden on this server.</p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 404 error
 */ 
int get_404_error(char* html, const int size)
{
    char* title = "404 Not Found";
    char *tmp = "<h1>Not Found</h1>\n\
<p>The requested URL was not found on this server.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 500 error
 */ 
int get_500_error(char* html, const int size)
{
    char* title = "500 Internal Server Error";
    char *tmp = "<h1>Internal Server Error</h1>\n\
<p>There is an internal server error.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 501 error
 */ 
int get_501_error(char* html, const int size)
{
    char* title = "501 Not Implemented";
    char *tmp = "<h1>Not Implemented</h1>\n\
<p>Your request method is not Implemented on this server.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 502 error
 */ 
int get_502_error(char* html, const int size)
{
    char* title = "502 Bad Gateway";
    char *tmp = "<h1>Bad Gateway</h1>\n\
<p>The server encounter a bad gateway.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}

/**
 * The 503 error
 */ 
int get_503_error(char* html, const int size)
{
    char* title = "503 Service Unavailable";
    char *tmp = "<h1>Service Unavailable</h1>\n\
<p>There is a service unavailable on this server.<br/></p>\n\
<hr>\n";

    strncpy(html, tmp, size);

    return templating(title, html, 1, size);
}




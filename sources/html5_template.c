/**
 * HTML5 Template implementation
 * 
 * @author Jerome Dh
 * @date 12/09/2020
 */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "html5_template.h"
/**
 * Wrapping the content with the HTML5
 * 
 * @return 1 if content is successful wrapping with title, otherwise returns 0
 */  
int templating(const char *title, char* content, const int add_extra, const size_t size)
{
    char tmp[size];

    // HTML5 header
    strncpy(tmp, content, size);
    strcpy(content, "<!Doctype html>\n<head>\n<title>");
    strncat(content, title, size - strlen(title));
    strcat(content, "</title>\n<link rel=\"shortcut icon\" href=\"favicon.png\" type=\"image/png\"/>\n");
    strcat(content, "</head><body>\n");

    // Adding content
    strncat(content, tmp, size - strlen(title) - 100);

    if(add_extra) {
        // Adding extra informations
        char address[255];
        if(get_server_address(address))
        {
            strcat(content, "<address><small>");
            strcat(content, address);
            strcat(content, "</small></address>\n");
        }
    }

    // HTML close tags
    strcat(content, "</body></html>\n");

    return strlen(content) < size ? 1 : 0;

}

/**
 * Get the server Address
 */ 
int get_server_address(char* address)
{
    strcpy(address, SERVER_NAME);
    strcat(address, "/");
    strcat(address, SERVER_VERSION);
    strcat(address, ".");
    strcat(address, SERVER_PATCH);
    strcat(address, " Server at ");
    strcat(address, HOST_NAME);
    strcat(address, " Port ");
    char _port[5];
    sprintf(_port, "%d", PORT);
    strcat(address, _port);

    return 1;
}
/**
 * HTML Template
 * 
 * @author Jerome Dh
 * @date 12/09/2020
 */

#ifndef HTML5_TEMPLATE
#define HTML5_TEMPLATE

// Wrapping the content with the HTML 5
int templating(const char *title, char* content, const int add_extra, const size_t size);

// Get the server informations
int get_server_address(char* address);

#endif // HTML5_TEMPLATE
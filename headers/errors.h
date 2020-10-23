/**
 * Manage HTTP Errors
 * 
 * @author Jerome Dh
 * @date 12/09/2020
 * 
 */

#ifndef ERROR_H
#define ERROR_H

/**
 * Client errors
 */ 
int get_400_error(char* html, const int size);
int get_401_error(char* html, const int size);
int get_403_error(char* html, const int size);
int get_404_error(char* html, const int size);

/**
 * Server errors
 */
int get_500_error(char* html, const int size);
int get_501_error(char* html, const int size);
int get_502_error(char* html, const int size);
int get_503_error(char* html, const int size);

#endif // ERROR_H
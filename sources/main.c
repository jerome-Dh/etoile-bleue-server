/**
 * Server side implementation
 * 
 * @author Jerome Dh
 * @date 05/09/2020
 */

#include <stdlib.h>
#include "server.h"

/**
 * Main app
 */ 
int main()
{
    init();

    app();

    end();

    return EXIT_SUCCESS;
}
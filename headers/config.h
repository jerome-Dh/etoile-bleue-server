/**
 * Server Configurations
 * 
 * @author Jerome Dh
 * @date 12/09/2020
 */

#ifndef CONFIG_H
#define CONFIG_H

#define PORT            1977
#define CRLF            "\r\n"
#define MAX_CLIENTS     100
#define SERVER_VERSION  "1"
#define SERVER_PATCH    "0"
#define SERVER_NAME     "Etoile Bleue"
#define BUF_SIZE        1024
#define HOST_NAME       "localhost"
#define CONFIG_FILE     "etoileBleue.conf"
#define DEFAULT_WWW_DIR "www"

#ifdef WIN32
#define PATH_SEPARATOR  "\\"
#else
#define PATH_SEPARATOR  "/"
#endif

#endif // CONFIG_H
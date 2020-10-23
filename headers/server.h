/**
 * Server header
 * 
 * @author Jerome Dh
 * @date 05/09/2020
 */
#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // close
#include <netdb.h> // gethostbyname

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error Not defined for this platform

#endif // (linux)

#include "http2.h"

// A client
typedef struct 
{
    SOCKET sock;
    char name[BUF_SIZE];
    Query* query;
    time_t timeout;

} Client;

void init(void);
void end(void);
void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static void write_client_file(SOCKET sock, const char *filename, const size_t size) ;
static void remove_client(Client *clients, int to_remove, int *actual);
static void clear_clients(Client *clients, int actual);
static int examineQuery(Client *client, char* buffer);
static void add_to_query(Client *client, char *line);
static int processQuery(const Client *client);
static void printQuery(Query *q);
static void printRequest(const Request *q);
static void printResponse(const Response *response);
static int send_to_client(const HTTP_VERB verb, const Client *client, const Response *response);
static void init_response(Response *reponse);
static void print_server_informations();

#endif // SERVER_H

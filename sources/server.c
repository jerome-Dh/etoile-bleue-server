/**
 * Lib core server
 * 
 * @author Jerome 
 * @date 06/09/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "server.h"

/**
 * Init the server
 */ 
void init(void) {
    
    #ifdef WIN32
        WSADATA wsa;
        int err = WSAStartup(MAKEWORD(2, 2), &wsa);
        if(err < 0) {
            puts("WSAStartup failed !");
            exit(EXIT_FAILURE);
        }
    #endif
}

/**
 * End up to server
 */ 
void end(void) {
    #ifdef WIN32
        WSACleanup();
    #endif
}

/**
 * Run app
 */ 
void app(void) {

    SOCKET sock = init_connection();
    char buffer[BUF_SIZE];

    // The index of the array
    int actual = 0;
    int max = sock;

    // Array for all clients
    Client clients[MAX_CLIENTS];

    fd_set rdfs;

    print_server_informations();

    while(1) {

        int i = 0;
        FD_ZERO(&rdfs);

        // Add STDIN_FILENO
        FD_SET(STDIN_FILENO, &rdfs);

        // Add the connection socket
        FD_SET(sock, &rdfs);

        // Add socket of each client
        for(i = 0; i < actual; i++) {
            FD_SET(clients[i].sock, &rdfs);
        }

        if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1) {

            perror("select()");
            exit(errno);
        }

        // Something from standard input : i.e keyboard
        if(FD_ISSET(STDIN_FILENO, &rdfs)) {

            // Stop process when typed on keyboard
            printf("\n\t***** A key typed, Bye ! *****\n\n");
            break;
        }
        else if(FD_ISSET(sock, &rdfs)) {

            // New client
            SOCKADDR_IN csin = { 0 };
            unsigned int sinsize = sizeof csin;
            int csock = accept(sock, (SOCKADDR *) &csin, &sinsize);

            if(csock == SOCKET_ERROR) {
                perror("accept()");
                continue;
            }

            // What is the new maximum fd ?
            max = csock > max ? csock : max;

            FD_SET(csock, &rdfs);

            Client c = { csock };
            strncpy(c.name, buffer, BUF_SIZE - 1);
            clients[actual] = c;

            // Configure the client
            clients[actual].query = NULL;
            clients[actual].timeout = 0;

            printf("\n-> New client %d connected\n", actual);

            actual++;
        }
        else {

            int i = 0;

            for(i = 0; i < actual; i++) {

                // A client is talking
                if(FD_ISSET(clients[i].sock, &rdfs)) {

                    Client client = clients[i];
                    int c = read_client(clients[i].sock, buffer);

                    // Client disconnected
                    if(c == 0) {

                        closesocket(clients[i].sock);
                        remove_client(clients, i, &actual);
                        printf("Client %d disconnected.\n", i);
                    }
                    else {

                        // Check message according to HTTP/2 protocol
                        printf("Client send a new message\n");
                        // printf("%s\n", buffer);
                        // print_all_chars(buffer);

                        // Adding the query to another string
                        int rep = examineQuery(&clients[i], buffer);

                        if( ! rep) {
                            /* Process operation, response to client and then close the socket */
                            printf("Client %d request is complete\n", i);

                            ///// Main processing
                            rep = processQuery(&clients[i]);
                            printf("\nResponse Status: %s\n", (rep == 1 ? "OK" : "NOT"));

                            // Set the timeout
                            time_t nw = time(NULL);
                            clients[i].timeout = nw + 5; // 05 secondes

                            closesocket(clients[i].sock);
                            clients[i].query = NULL; // Clean up old query
                            remove_client(clients, i, &actual);

                        }

                    }

                    break;

                }

                // Waiting until timeout and closing the connection
                // time_t now = time(NULL);
                // if(clients[i].timeout > 0 && now >= clients[i].timeout)
                // {
                //     closesocket(clients[i].sock);
                //     clients[i].query = NULL; // Clean up old query
                //     clients[i].timeout = 0;
                //     remove_client(clients, i, &actual);
                //     printf("Timeout.\n");
                // }
                // else
                // {
                //     printf("NOT.\n");
                // }

            }

        }

    }

    clear_clients(clients, actual);
    end_connection(sock);
}


/**
 * Close connection to clients
 */ 
static void clear_clients(Client *clients, int actual) {

    int i = 0;
    for(i = 0; i < actual; i++) {
        closesocket(clients[i].sock);
    }
}

/**
 * Remove a client from a list
 */ 
static void remove_client(Client *clients, int to_remove, int *actual) {

    // Remove the client in the array
    memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(clients));

    // Number client - 1
    (*actual)--;
}

/**
 * Init the connection
 */
static int init_connection(void) {

    printf("Creating socket..\n");
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sin = { 0 };

    if(sock == INVALID_SOCKET) {

        perror("socket()");
        exit(errno);
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    printf("Binding address to socket..\n");
    if(bind(sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR) {

        perror("bind()");
        exit(errno);
    }

    printf("Listening incoming connexions..\n");
    if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR) {

        perror("listen()");
        exit(errno);
    }

    return sock;

}

/**
 * Close all connections
 */ 
static void end_connection(int sock) {

    closesocket(sock);
}

/**
 * Receive a message from a client
 */ 
static int read_client(SOCKET sock, char *buffer) {

    int n = 0;
    if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        n = 0; // Disconnect the client
    }

    buffer[n] = 0;

    return n;

}

/**
 * Send the message to a client
 */ 
static void write_client(SOCKET sock, const char *buffer) {

    if(send(sock, buffer, strlen(buffer), 0) < 0) {

        perror("send()");
        exit(errno);
    }

}

/**
 * Send a file to a client
 */ 
static void write_client_file(SOCKET sock, const char *filename, const size_t size) 
{
    FILE *fic = fopen(filename, "r");

    if(fic != NULL)
    {
        char *file_buf = calloc(sizeof(char), size);
        if(fread(file_buf, 1, size, fic) >= 0)
        {
            if(send(sock, file_buf, size, 0) < 0) {
                perror("send()");
                exit(errno);
            }
        }

        fclose(fic);
    }

}

/**
 * Examine the reveived message from client
 * and add string to query of client
 */ 
static int examineQuery(Client *client, char* buffer)
{
    // print_all_chars(buffer);

    // If message is "\r\n", so the client is finished his query
    if( ! strncmp(buffer, CRLF, BUF_SIZE)){
        return 0;
    }
    else {

        // Check if the end of message contain "\r\n\r\n"
        char _end[BUF_SIZE];
        strncpy(_end, CRLF, BUF_SIZE / 2);
        strncat(_end, CRLF, BUF_SIZE / 2);        
        int ret = (strstr(buffer, _end) != NULL) ? 0 : 1;

        // Check if the string has more than one "\r\n" and then
        // split it into multiples lines
        int j = 0;
        char *t = malloc(sizeof(char) * BUF_SIZE);
        for(size_t i = 0; i < strlen(buffer); ++i)
        {
            if(buffer[i] == '\r' && buffer[i+1] == '\n')
            {
                t[j] = '\0';

                if(strlen(t) > 0) {
                    add_to_query(client, t);
                }

                j = 0;
            }
            else if(buffer[i] == '\r' || buffer[i] == '\n') {
                continue;
            }
            else // Copy one char from buffer to t
            {
                t[j++] = buffer[i];
            }
        }

        return ret;
    }

}

/**
 * Adding to end of query list
 */ 
static void add_to_query(Client *client, char *line)
{
    Query *new;
    new = malloc(sizeof *new);
    new->next = NULL;
    remove_crlf(line, BUF_SIZE); // Remove CRLF
    strncpy(new->chr, line, BUF_SIZE);

    // Insert at the end of Query list
    Query *p = client->query;
    if(p == NULL) {
        client->query = new;
    }
    else {
        while(p->next != NULL) {
            p = p->next;
        }
        p->next = new;
    }

}

/**
 * Process the HTTP query
 */ 
static int processQuery(const Client *client)
{
    int success = 0;

    if(client != NULL) {

        printQuery(client->query);

        Request request;
        Response response;

        // 0. Initialize the response content
        init_response(&response);

        // 1. Check valid query conform to HTTP/2
        REQUEST_STATE state = parseRequest(client->query, &request);

        printf("\nRequest status: %s\n", (state == _OK ? "OK" : "NOT"));

        // 2. Perform operations
        switch (state)
        {
        case _OK:
            /* Good request conform HTTP/2 */
            success = performOperation(&request, &response);
            break;

        default:
            /* Others status such as _BAD_REQUEST, _UNAUTHORIZED, _FORBIDDEN, etc. */
            success = performError(state, &response);
            break;
        }

        // 3. Compose response to send to client
        composeResponse(&request, &response);
        // printResponse(&response);

        // 4. Send back response to client
        success = send_to_client(request.verb, client, &response);

    }

    return success;
}

/**
 * Send the response to client
 */
static int send_to_client(const HTTP_VERB verb, const Client *client, const Response *response)
{
    if(client != NULL && response != NULL)
    {
        // Content-lenght of all = Head content + body content
        size_t TOTAL_LENGTH = HEAD_MAX_SIZE + 1;
        TOTAL_LENGTH += (verb != _HEAD) ? (response->filename == NULL ? response->content_length : 0) : 0;

        // Convert response to string
        char *content = calloc(sizeof(char), TOTAL_LENGTH);
        response_to_string(verb, response, content);
        // print_all_chars(content);

        // Write to the client socket
        write_client(client->sock, content);

        // If message content a file, then send it to client
        if(verb != _HEAD && response->filename != NULL) {
            write_client_file(client->sock, response->filename, response->content_length);
        }

        return 1;
    }
    
    return 0;
}

/**
 * Initialize the response
 */ 
static void init_response(Response *response)
{
    if(response != NULL) {

        strncpy(response->status_text, "", BUF_SIZE);
        strncpy(response->date, "", BUF_SIZE);
        strncpy(response->server, "", BUF_SIZE);
        strncpy(response->last_modified, "", BUF_SIZE);
        strncpy(response->etag, "", BUF_SIZE);
        strncpy(response->accept_range, "", BUF_SIZE);
        strncpy(response->vary, "", BUF_SIZE);

        response->filename = NULL;
        response->content = NULL;
        response->content_length = 0;
        response->error_content_size = ERROR_MAX_SIZE;

        response->version = _HTTP_2;
        response->status = _OTHERS_STATUS_CODE;
        response->content_type = _OTHERS_CONTENT_TYPE;
    }

}

/**
 * Print the query string
 */ 
static void printQuery(Query *q) {

    Query *p = q;

    printf("---------------------------\n");
    while(p != NULL) {
        printf("%s\n", p->chr);
        p = p->next;
    }
    printf("---------------------------\n");

}

/**
 * Print the request
 */ 
static void printRequest(const Request *request) {

    if(request != NULL) {

        printf("-----------------\n");
        printf("Verb: %d\n", request->verb);
        printf("URI: %s\n", request->uri);
        printf("Version: %d\n", request->version);
        printf("Host: %s\n", request->host);
        printQuery(request->others);
    }

}

/**
 * Print the response
 */ 
static void printResponse(const Response *response) {

    if(response != NULL) {
        
        printf("-----------------\n");
        printf("HTTP_VERSION: %d\n", response->version);
        printf("STATUS_CODE: %d\n", response->status);
        printf("status_text: %s\n", response->status_text);
        printf("server: %s\n", response->server);
        printf("Date: %s\n", response->date);
        printf("last_modified: %s\n", response->last_modified);
        printf("etag: %s\n", response->etag);
        printf("accept_range: %s\n", response->accept_range);
        printf("vary: %s\n", response->vary);
        printf("content_type: %d\n", response->content_type);
        printf("content_length: %ld\n", response->content_length);

        if(response->filename != NULL){

            printf("Filename: %s\n", response->filename);

        }

        if(response->content != NULL){

            printf("content_size: %ld\n", response->error_content_size);
            printf("content: %s\n", response->content);

        }
    }

}

/**
 * Print the servers informations
 */
static void print_server_informations()
{
    clear();
    printf("\n\t***********************************************************\n\n");
    printf("\tWelcome to %s Server %s.%s\n\n", SERVER_NAME, SERVER_VERSION, SERVER_PATCH);
    printf("\tServer has started on %s Port %d\n", HOST_NAME, PORT);
    printf("\tThe Server listening all incoming connexions on this port.\n");
    printf("\n\t***** To exit the server, press ENTER key *****\n");
    printf("\n\t***********************************************************\n");
    printf("\n\tWaiting: _\n");

}

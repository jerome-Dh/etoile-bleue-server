/**
 * Concurent algorithms implementation
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "concurency.h"
#include "matrice-mpi.h"

/**
 * Matrix multiplication
 */ 
void matmult(const char *query, char *result)
{
    MPI_RESULT ret = execute_mpi_matrix(query);
    FILE *file_out = fopen(OUT_FILENAME, "r");

    if(ret.status == 0 && file_out != NULL) {

        double diff_time;
        fscanf(file_out, "%lf", &diff_time);
        fclose(file_out);

        char *r = "<div style=\"width: 700px; border: 1px solid gray; border-radius: 4px;\
padding: 10px; margin: auto;\">\
<h1 style=\"text-decoration: underline; text-align: center\">\
<img src=\"favicon.png\" width=\"48\"/><br/>\
Produit des matrices</h1>\
<p>Operations de multiplication realisees sur les matrices <strong>A[%d,%d]</strong>\
et <strong>B[%d,%d]</strong></p>\
<ul><li>Generation des valeurs des matrices A et B..</li>\
<li>Lancement du calcul sur <strong style=\"color: blue; font-size: 18px;\">0%d noeud(s) MPI</strong> \
et <strong style=\"color: blue; font-size: 18px;\">0%d thread(s) OpenMP</strong> \
en mode <strong>concurrent</strong></li>\
<li>Operations en cours..</li>\
<li>Agregation des resultats..</li>\
<li>Terminee.</li></ul>\
<h2 style=\"color: blue\">Temps processeur ecoule: %.5lf secondes</h2>\
<p><small><a href=\"/result.txt\">Cliquer pour voir le resultat</a></small>\
</div><hr/>";

        char t[BUF_SIZE + strlen(r)];
        sprintf(
            t,
            r,
            T1, // Length for A[x]
            T2, // Length for A[y]
            T1, // Length for B[x]
            T2, // Length for B[y]
            ret.p, // Processes length
            ret.q, // Threads length
            diff_time // Execution time
        );

        strcpy(result, t);
    }
    else
    {
        strcpy(result, "Une erreur s'est produite pendant le traitement de votre requete, veuillez reessayez !\n");
    }

}

/**
 * Execute the MPI program for matrix
 */ 
MPI_RESULT execute_mpi_matrix(const char *query)
{
    // /search?p=1&q=1
    int p = 2, q = 2;
    if(strlen(query) >= (strlen(SEARCH) + 8)) 
    {
        // Search for processes length and thread
        char *ptr = strstr(query, "p="),
            *qtr = strstr(query, "q=");

        if(ptr != NULL && qtr != NULL)
        {
            char pChar[1] = "",
                qChar[1] = "";

            pChar[0] = ptr[2];
            qChar[0] = qtr[2];
            p = atoi(pChar);
            q = atoi(qChar);
        }
    }
    p = p >= 1 && p <= 2 ? p : 2; 
    q = q >= 1 && q <= 4 ? q : 4;

     // Deleting existing result file
    char system_cmd[BUF_SIZE];
    sprintf(system_cmd, "rm -f %s", RESULT_FILENAME);
    system(system_cmd);

    char export[BUF_SIZE],
        command[BUF_SIZE];
    sprintf(export, "export OMP_NUM_THREADS=%d", q);
    sprintf(command, "mpirun -np %d %s", p, OUTPUT_MATRIX_NAME);

    printf("%s\n%s\n", export, command);
    int ret = system(export);
    if(ret == 0)
    {
        ret = system(command);
    }

    MPI_RESULT result = {p, q, ret};

    return result;
}

/**
 * Search in DB
 */ 
void db_search(const char *query, char *result)
{
    strcpy(result, "<h1>Toutes les operations sont completes</h1>");
    strcat(result, "<p>Recherche terminee</p>");

    KeywordList *keyword_list = malloc(sizeof(*keyword_list));
    keyword_list->first = NULL;

    // Break splite keywords
    get_keywords(keyword_list, query);

    // Examine every keyword
    int k = 0;
    Keyword *p = keyword_list->first;
    strcat(result, "<stong>Mot cles: </strong>");
    while(p != NULL) {

        char tok[BUF_SIZE];
        sprintf(tok, "%s, ", p->chr);
        strncat(result, tok, strlen(tok));

        // Next token
        k++;
        p = p->next;
    }

    strcat(result, "<hr/>");
}

/**
 * Get keywords list of a query
 */ 
void get_keywords(KeywordList *keyword_list, const char *query)
{
    char *ptr = strchr(query, '?');

    if(ptr != NULL)
    {
        if(strlen(ptr) > 3 &&  ptr[1] == 'q' && ptr[2] == '=')
        {
            // Shift to first name
            ptr = ptr + 3;
            // printf("Substring is: %s, len: %ld\n", ptr, strlen(ptr));
            
            int j = 0;
            char *t = calloc(sizeof(char), BUF_SIZE);
            for(size_t i = 0; i < strlen(ptr); ++i)
            {
                
                if(ptr[i] == '+' || i+1 ==  strlen(ptr))
                {
                    if(i+1 == strlen(ptr) && ptr[i] != '+')
                    {
                        t[j++] = ptr[i];
                    }

                    t[j] = '\0';

                    if(strlen(t) > 0) {
                        add_to_keyword(keyword_list, t);
                        // printf("Sub: %s\n", t);
                    }

                    j = 0;
                }
                else // Copy one char from buffer to t
                {
                    t[j++] = ptr[i];
                }
            }
        }
    }
}

/**
 * Adding to end of keywords list
 */ 
void add_to_keyword(KeywordList *keyword_list, const char *str)
{
    Keyword *new;
    new = malloc(sizeof(*new));
    new->next = NULL;
    strncpy(new->chr, str, BUF_SIZE);

    // Insert at the end of Query list
    Keyword *p = keyword_list->first;
    if(p == NULL) {
        keyword_list->first = new;
    }
    else {
        while(p->next != NULL) {
            p = p->next;
        }
        p->next = new;
    }

}
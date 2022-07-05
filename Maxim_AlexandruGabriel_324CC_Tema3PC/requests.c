#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "json.hpp"

char *compute_get_request(char *host, const char *url, char *query_params,
                            char **cookies, int cookies_count, char * authorization)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcpy(line, "Cookie: ");
        for(int i = 0; i < cookies_count; i ++){
            strcat(line, cookies[i]);
            strcat(line, "; ");
        }
        if(cookies_count - 1 != 0){
            strcat(line, cookies[cookies_count - 1]);
        }
        compute_message(message, line);
    }

    if (authorization != NULL) {
        memset(line, 0, LINELEN);
        strcpy(line, "Authorization: Bearer ");
        strcat(line, authorization);
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, const char *url, const char* content_type, const char *body_data,
         char **cookies, int cookies_count, char *authorization)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s\r\nContent-Length: %ld", content_type, strlen(body_data));
    compute_message(message, line);
    
    if (authorization != NULL) {
        memset(line, 0, LINELEN);
        strcpy(line, "Authorization: Bearer ");
        strcat(line, authorization);
        compute_message(message, line);
    }

    if (cookies != NULL) {
        memset(line, 0, LINELEN);
        strcpy(line, "Cookie: ");
        for(int i = 0; i < cookies_count; i ++){
            strcat(line, cookies[i]);
            strcat(line, "; ");
        }
        strcat(line, cookies[cookies_count - 1]);
        compute_message(message, line);
    }
    
    compute_message(message, "");

    memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}

char *compute_delete_request (char *host, char *url, char * authorization)
{
    
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    
    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (authorization != NULL) {
        memset(line, 0, LINELEN);
        strcpy(line, "Authorization: Bearer ");
        strcat(line, authorization);
        compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}
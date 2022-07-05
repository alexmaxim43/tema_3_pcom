#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <iostream>
#include "json.hpp"


using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    int sockfd;

    char *message;
    char *response;
    char *cookie = (char *)malloc(200 * sizeof(char));
    char *token = (char *)malloc(500 * sizeof(char));
    
    int ok = 1;

    char server_ip[15] = "34.241.4.235";

    while (ok)
    {   
        // Citim comanda de la tastatura
        char *command = (char *) malloc(20 * sizeof(char)); 
        cin >> command;

        // Deschidem conexiunea
        sockfd = open_connection(server_ip, 8080, AF_INET, SOCK_STREAM, 0);

        // Register
        if(!strncmp(command, "register", 8))
        {
            // Citim username-ul si parola, iar apoi salvam datele in json

            char username[20];
            cout << "username = ";
            cin >> username;
            
            char password[20];
            cout << "password = ";
            cin >> password;
            
            json j;
            j["username"] = username;
            j["password"] = password;

            // Trimitem un request de tip POST cu username-ul si parola citite
            message = compute_post_request(server_ip, "/api/v1/tema/auth/register", "application/json", j.dump().c_str(), NULL, 0, NULL);
            cout << message;
            send_to_server(sockfd, message);
            
            response = receive_from_server(sockfd);
            cout << response;

            continue;
        }

        // Login
        if(!strncmp(command, "login", 5))
        {
            char username[20];
            cout << "username = ";
            cin >> username;
            
            char password[20];
            cout << "password = ";
            cin >> password;
            
            json j;
            j["username"] = username;
            j["password"] = password;
            
            // Trimitem un request de tip POST cu credentialele citite de la tastatura
            message = compute_post_request(server_ip, "/api/v1/tema/auth/login", "application/json", j.dump().c_str(), NULL, 0, NULL);
            cout <<  message;
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            cout << response;

            // Retinem cookie-ul returnat pentru a valida accesul la restul comenzilor
            strcpy(cookie, "");

            if(strstr(response, "connect.sid=") != NULL)
            {
                char *tmp = strstr(response, "connect.sid=");
                char delim[2] = ";";
                tmp = strtok(tmp, delim);
                strcpy(cookie, tmp);
            }

            continue;
        }

        // Cere acces in bibloteca
        if(!strncmp(command, "enter_library", 13))
        {
            // Trimitem un GET cu cookie ul retinut la login pentru a vedea daca user-ul are acces
            message = compute_get_request(server_ip, "/api/v1/tema/library/access", NULL, &cookie, 1, NULL);
            cout << message;
            send_to_server(sockfd, message);
            
            response = receive_from_server(sockfd);
            cout << response;

            // Retinem cheia de autorizare daca login-ul s-a efectuat cu succes
            if (strcmp(cookie, "")) 
            {
                char *payload = (char *)malloc(500 * sizeof(char));

                if (strstr(response, "{") != NULL)
                {
                    strcpy(payload, strstr(response, "{"));
                }

                json j = json::parse(payload);
                string tkn = j["token"];
                strcpy(token, tkn.c_str());
            }

            continue;
        }

        // Primim informatii despre toate cartile
        if(!strncmp(command, "get_books", 9))
        {
            // Trimitem un request GET in care trimitem cheia de autorizare
            message = compute_get_request(server_ip, "/api/v1/tema/library/books", NULL, NULL, 0, token);
            cout << message;
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            cout << response;
               
            continue;
        }

        // Primim informatii despre cartea cu id-ul citit de la tastatura
        if(!strncmp(command, "get_book", 8))
        {
            int id;

            cout << "id = ";
            cin >> id;

            // Cream url-ul cu id ul citit de la tastatura.
            char new_url[100];
            sprintf(new_url, "/api/v1/tema/library/books/%d", id);

            // Efectuam un request de tip get cu url-ul nou si cheaia de autorizare
            message = compute_get_request(server_ip, new_url, NULL, NULL, 0, token);
            cout << message;
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            cout << response;
            
            continue;
        }
        
        // Adaugam o carte
        if (!strncmp(command, "add_book", 8))
        {
            char data[50];
            json j;

            // Introducem datele cartii si le salvam in json
            cin.ignore();
            cout << "title = ";
            cin.getline(data, sizeof(data));
            j["title"] = data;

            cout << "author = ";
            cin.getline(data, sizeof(data));
            j["author"] = data;

            cout << "genre = ";
            cin.getline(data, sizeof(data));
            j["genre"] = data;

            cout << "publisher = ";
            cin.getline(data, sizeof(data));
            j["publisher"] = data;

            cout << "page_count = ";
            cin.getline(data, sizeof(data));
            j["page_count"] = atoi(data);

            // Trimitem un POST request cu fisierul json pentru a salva datele despre carte
            message = compute_post_request(server_ip, "/api/v1/tema/library/books", "application/json", j.dump().c_str(), NULL, 0, token);
            cout << message;
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            printf("%s", response);

            continue;
        }

        // Stergem cartea cu id-ul citit de la tastatura
        if (!strncmp(command, "delete_book", 11))
        {
            int id;

            cout << "id = ";
            cin >> id;

            // Cream url-ul nou
            char new_url[100];
            sprintf(new_url, "/api/v1/tema/library/books/%d", id);

            // Trimitem un request DELETE pe url-ul cu id-ul cartii si cheia de autorizare
            message = compute_delete_request(server_ip, new_url, token);
            cout << message;
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            cout << response;

            continue;
        }

        // Logout
        if (!strncmp(command, "logout", 6))
        {
            // Efectuam un request GET in care trimitem cookie-ul de la login
            message = compute_get_request(server_ip, "/api/v1/tema/auth/logout", NULL, &cookie, 1, NULL);
            cout << message;
            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            cout << response;

            // Resetam cookie, cheia de autorizarie si inchidem conexiunea
            strcpy(cookie, "");
            strcpy(token, "");

            continue;
        }

        // Exit
        if (!strncmp(command, "exit", 4))
        {
            // Oprim programul
            ok = 0;
            break;
        }

        //Inchidem conexiunea
        close_connection(sockfd);
    }
    return 0;
}
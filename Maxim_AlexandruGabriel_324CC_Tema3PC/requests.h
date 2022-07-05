#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, const char *url, char *query_params,
							char **cookies, int cookies_count, char *authorization);

// computes and returns a POST request string (cookies and authorization can be NULL if not needed)
char *compute_post_request(char *host, const char *url, const char* content_type, const char *body_data, 
		char** cookies, int cookies_count, char * authorization);

// computes and returns a DELETE request string (authorization can be NULL if not needed)
char *compute_delete_request (char *host, char *url, char * authorization);
#endif

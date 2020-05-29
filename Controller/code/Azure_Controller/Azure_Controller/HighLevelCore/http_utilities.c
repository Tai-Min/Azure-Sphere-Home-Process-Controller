#include "http_utilities.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/timerfd.h>
#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <pthread.h>
#include "common.h"
#include <stdbool.h>
#include "applibs/log.h"
#include <string.h>
#include <sys/time.h>

static char* emptyCallback(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]) {};

static bool isThreadUp = false;
static pthread_t client_daemon;
static pthread_mutex_t sockMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t killMutex = PTHREAD_MUTEX_INITIALIZER;
static bool killThreadRequest = false;
static int sock = -1;
static const char*(*siteGETCallback)(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]) = emptyCallback;

/* external functions */
int httpServerInit();
void httpServerStop();
void httpServerKillSubthread();
bool httpIsServerUp();
void registerGETCallback(const char* (*cb)(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31]));

/* helpers */
/*
* @brief Read available response from client file descriptor.
*
* @param fd File descriptor of a client.
* @param bytes Buffer to keep bytes read from client.
* @param size Buffer size.
* @param err Flag that will be set if client's request is bigger than buffer size.
* @return Number of bytes read.
*/
int readResponse(int fd, char* bytes, int size, bool* err);

/*
* @brief Parse content of client's request into type of request, site location and key-value pairs
*
* @param content Content to parse.
* @param len Size of content buffer.
* @param type Buffer where type of request will be storer.
* @param siteLocation Buffer where request took place.
* @param argKeys Buffer in which keys of key-value pairs will be stored.
* @param argValues Buffer in which values of key-value pairs will be stored.
* @param err Error flag that will be set if parsing failed.
*/
void parseContent(char* content, int len, char type[8], char siteLocation[21], bool* hasParams, char argKeys[20][31], char argValues[20][31], bool* err);

/*
* @brief Main function of http server's subthread.
*/
void httpRequestHandler();

/* definitions external functions*/
int httpServerInit() {
    int one = 1;
    int client_fd;
    struct sockaddr_in svr_addr, cli_addr; 

    pthread_mutex_lock(&sockMutex);
    sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sock < 0) {
        Log_Debug("Can't open socket");
        pthread_mutex_unlock(&sockMutex);
        httpServerStop();
        return -1;
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    int port = 8181;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*) & svr_addr, sizeof(svr_addr)) == -1) {
        Log_Debug("Can't bind\n");
        pthread_mutex_unlock(&sockMutex);
        httpServerStop();
        return -1;
    }

    listen(sock, 5);
    pthread_mutex_unlock(&sockMutex);

    if (!isThreadUp) {
        if (pthread_create(&client_daemon, NULL, httpRequestHandler, NULL)) {
            Log_Debug("Failed to start server daemon.\n");
            httpServerStop();
            return -1;
        }
        isThreadUp = true;
    }
    
    Log_Debug("Server daemon started on port %d\n", htons(svr_addr.sin_port));
    return 0;
}

void httpServerStop() {
    pthread_mutex_lock(&sockMutex);
    if (sock != -1) close(sock); 
    sock = -1;
    pthread_mutex_unlock(&sockMutex);
    Log_Debug("HTTP server stopped\n");
}

void httpServerKillSubthread() {
    pthread_mutex_lock(&killMutex);
    killThreadRequest = true;
    pthread_mutex_unlock(&killMutex);
    pthread_join(client_daemon, NULL);
    isThreadUp = false;
}

bool httpIsServerUp() {
    pthread_mutex_lock(&sockMutex);
    bool result = (sock != -1);
    pthread_mutex_unlock(&sockMutex);
    return result;
}

void registerGETCallback(const char* (*cb)(char siteLocation[21], bool hasParams, char argKeys[20][31], char argValues[20][31])) {
    siteGETCallback = cb;
}

/* definitions helpers */
int readResponse(int fd, char *bytes, int size, bool *err) {
    
    int res = 1;
    int bytesRead = 0;

    while (res > 0) {
        res = read(fd, bytes + bytesRead, size-bytesRead);
        if(res > 0) {
            bytesRead += res;
            if (bytesRead >= size) {
                *err = true;
                return bytesRead;
            }
        }
    }
    return bytesRead;
}

void parseContent(char* content, int len, char type[8], char siteLocation[21], bool* hasParams, char argKeys[20][31], char argValues[20][31], bool* err) {
    char * paramSymbol = strchr(content, '?');
    if (paramSymbol != NULL)
        *hasParams = true;
    
    char* methodstr = strtok(content, " \t\n");
    
    if (strlen(methodstr) > 7) {
        *err = true;
        return;
    }
    memcpy(type, methodstr, strlen(methodstr) + 1);
    type[strlen(methodstr)] = '\0';

    char* locstr;
    if(*hasParams)
        locstr = strtok(NULL, "?");
    else
        locstr = strtok(NULL, " ");

    if (locstr == NULL) {
        *err = true;
        return;
    }
    if (strlen(locstr) > 20) {//too long location or no args provided
        *err = true;
        return;
    }

    memcpy(siteLocation, locstr, strlen(locstr) + 1);
    siteLocation[strlen(locstr)] = '\0';

    if (!*hasParams)
        return;

    char* args = strtok(NULL, " ");
    if (args == NULL) {
        *err = true;
        return;
    }

    int cntr = 0;
    char* kv = kv = strtok(args, "=");;
    if (kv == NULL) {
        *err = true;
        return;
    }
    if (strlen(kv) > 30) {
        *err = true;
        return;
    }
    memcpy(argKeys[cntr], kv, strlen(kv) + 1);
    argKeys[cntr][strlen(kv)] = '\0';

    while (kv != NULL) {
        kv = strtok(NULL, "&");
        if (kv == NULL) {
            *err = true;
            return;
        }
        if (strlen(kv) > 30) {
            *err = true;
            return;
        }
        memcpy(argValues[cntr], kv, strlen(kv) + 1);
        argValues[cntr][strlen(kv)] = '\0';

        cntr++;

        kv = strtok(NULL, "=");
        if (kv != NULL) {
            if (strlen(kv) > 30) {
                *err = true;
                return;
            }
            memcpy(argKeys[cntr], kv, strlen(kv) + 1);
            argKeys[cntr][strlen(kv)] = '\0';
        }
    }
}

void httpRequestHandler() {
    while (1) {
        if (httpIsServerUp()) {
            struct sockaddr_in cli_addr;
            socklen_t sin_len = sizeof(cli_addr);

            pthread_mutex_lock(&sockMutex);
            int client_fd = accept4(sock, (struct sockaddr*) & cli_addr, &sin_len, SOCK_NONBLOCK);
            pthread_mutex_unlock(&sockMutex);

            if (client_fd != -1) {
                Log_Debug("Client connected\n");
                bool err = false;
                char content[2048];
                int bytesRead = readResponse(client_fd, content, 2048, &err);

                if (!err) {
                    char type[8] = { 0 };
                    char siteLocation[21] = { 0 };
                    bool hasParams = false;
                    char argKeys[20][31] = { 0 };
                    char argValues[20][31] = { 0 };

                    parseContent(content, bytesRead, type, siteLocation, &hasParams, argKeys, argValues, &err);
                    if (!err) {
                        const char* site = siteGETCallback(siteLocation, hasParams, argKeys, argValues);
                        write(client_fd, site, strlen(site));
                    }
                }
                close(client_fd);
            }
        }

        pthread_mutex_lock(&killMutex);
        if (killThreadRequest) {
            killThreadRequest = false;
            pthread_mutex_unlock(&killMutex);
            return;
        }
        pthread_mutex_unlock(&killMutex);

        waitMs(100);
    }
}
//
//  IPC.c
//  QueueTest
//
//  Created by yuzhou on 16/6/12.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include "IPC.h"
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h> //for unlink header
#include <pthread.h>
#include <string.h>


#define SOCKET_NAME "/tmp/msg_sock"
#define SEND_MESSAGE "SEND"
#define MAX_CNT 128

/**********Implement**************/
static int serverFd = -1;
//static int clientFds[MAX_CNT];
static ClientFds *clientFds = NULL;

static void initClientFds() {
    if (NULL == clientFds) {
        clientFds = (ClientFds*)malloc(sizeof(ClientFds));
        clientFds->next = NULL;
    }
}

static void addFd(ClientFds *fd) {
    if (NULL == fd) return;
    
    ClientFds *p = clientFds;
    while (1) {
        if (NULL == p->next) {
            p->next = fd;
            break;
        } else {
            p = p->next;
        }
    }
}

static void *createSocket(void *arg) {
    if (serverFd >= 0) {
        printf("Socket "SOCKET_NAME" has been createn!\n");
    } else {
        serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (serverFd < 0) {
            printf("Create socket error:%d\n", errno);
            return NULL;
        }

        unlink(SOCKET_NAME);

        printf("Create socket successfully!\n");

        struct sockaddr_un server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sun_family = AF_UNIX;
        strcpy(server_address.sun_path, SOCKET_NAME);
        
        long size = strlen(server_address.sun_path) + offsetof(struct sockaddr_un, sun_path);
        
        if (bind(serverFd, (struct sockaddr *)&server_address, (socklen_t)size) < 0) {
            printf("bind error:%d\n", errno);
            close(serverFd);
        }
        
        if (listen(serverFd, 5) < 0) {
            printf("listen error:%d\n", errno);
            close(serverFd);
        }
        
        printf("Listen successfully. Begin to accept.\n");
        
        int client_len = sizeof(server_address);
        //int cnt = 0;
        //fds->server_fd = fd;
        while(1) {
            int client_sockfd = accept(serverFd, (struct sockaddr *)&server_address, (socklen_t *)&client_len);
            printf("Accept socket client fd=%d\n", client_sockfd);
            ClientFds *clientFd = (ClientFds*)malloc(sizeof(ClientFds));
            clientFd->next = NULL;
            clientFd->fd = client_sockfd;

            char tag[64] = {0};
            read(client_sockfd, tag, 64);
            printf("CLient tag = %s\n", tag);
            strcpy(clientFd->tag, tag);
            addFd(clientFd);
        }
    }
    
    return NULL;
}

static int createClient(int fd, char *tag) {
    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy (address.sun_path, SOCKET_NAME);
    long len = sizeof(address);
    
    //printf("Client fd = %d\n", fd);
    int result = connect(fd, (struct sockaddr *)&address, (socklen_t)len);
    if (result < 0) {
        printf("Error result = %d\n", result);
    } else {
        write(fd, tag, strlen(tag));
    }
    
    return result;
}

//static void disposeSocket() {
    //close(fd);
    //fd = -1;
//}

/**********InterFace**************/
void initServer() {
    pthread_t pid;
    pthread_attr_t attr;

    initClientFds();

    int err = pthread_attr_init(&attr);
    if (err != 0) return;
    
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err != 0) return;
    
    err = pthread_create(&pid, &attr, createSocket, NULL);
    if (err != 0) printf("Create Thread fail!\n");
}

int createClientSocket() {
    return socket(AF_UNIX, SOCK_STREAM, 0);
}

void connectServer(int fd, char *tag) {
    printf("Begin to connect to Server\n");
    while(createClient(fd, tag) < 0) {
        printf("Need retry!\n");
        sleep(1);
    }
}

struct fd_set fds;
void test(const int fd) {
    //struct timeval timeout={0,0};
    int read_fd = fd;
    FD_ZERO(&fds);
    FD_SET(read_fd, &fds);
    
    select(read_fd+1, &fds, NULL, NULL, NULL);
    printf("select end!\n");
    char buf[32] = {0};
    recv(fd, buf,32, 0);
    printf("buf=%s\n", buf);
}

void pending() {
    
}

void notify(char *tag) {
    if (NULL == tag) return;

    ssize_t result = -1;
    for (ClientFds *p = clientFds->next; p != NULL; p = p->next) {
        if (strcmp(tag, p->tag) == 0) {
            result = write(p->fd, SEND_MESSAGE, strlen(SEND_MESSAGE));
            printf("notify tag = %s, fd=%d\n", p->tag, p->fd);
            break;
        }
    }
    
    if (result < 0) {
        printf("Error notify result = %d\n", (int)result);
    }
}

void disconnect() {
    
}
//
//  IPC.h
//  QueueTest
//
//  Created by yuzhou on 16/6/12.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#ifndef IPC_h
#define IPC_h

#include <stdio.h>

typedef struct CLIENTFD {
    char tag[64];
    int fd;
    struct CLIENTFD *next;
} ClientFds;

void initServer();
void pending();
void notify(char *tag);
int createClientSocket();
void connectServer(int fd, char *tag);
void test(const int fd);
#endif /* IPC_h */

//
//  PulseThread.c
//  QueueTest
//
//  Created by yuzhou on 16/6/10.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include "PulseThread.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "IPC.h"

static void fire(msgHandlerCB *cb, void *arg) {
    cb(arg);
}

static void* messageHandler(void *arg) {
    MessageHeader *header = (MessageHeader*)arg;
    printf("Start Message Handler: %s!\n", header->tag);

    while (1) {
        pending();
        printf("waitting!\n");
        test(header->fd);
        Message *msg = popupMessage(header, 0);
        if (NULL != msg) {
            fire(header->cb, (void*)msg);
        }
        /*
        pthread_mutex_lock(&(header->mutex));
        if (p->next == NULL) {
            printf("Empty Message Queue...\n");
        } else {
            printf("yuzhou debug\n");
            p->next = q->next;
            //应该增加一个线程通知
            //popupMessage();
            
            fire(cb, (void*)q);
            //cb((void*)q);
        }
        pthread_mutex_unlock(&(header->mutex));
         */
    }

    return NULL;
}

static bool isInit = false;

bool init() {
    if (!isInit) {
        initServer();
        isInit = InitMessageHeader();
    }

    return isInit;
}

bool createMessgaeQueue(const char *tag, msgHandlerCB *cb) {
    bool result = true;

    if (init()) {
        MessageHeader *header = NewAndAddMessageHeader(tag, cb);
        if (NULL != header) {
            connectServer(header->fd, header->tag);
            printf("Begin to create Message Header Tag = %s\n", tag);
        } else {
            result = false;
        }
    }

    return result;
}

void addMessageIntoQueue(const char *tag, int what) {
    MessageHeader *header = getMessageHeaderByTag(tag);
    if (NULL != header) {
        Message *msg = createMessage(what);
        addMessage(tag, msg);
    }
}

void createHandlerThread(const char *tag) {
    MessageHeader *header = getMessageHeaderByTag(tag);
    if (header == NULL) {
        printf("Header is NULL!\n");
    } else {
        pthread_t id;
        int ret = pthread_create(&id, NULL, (void*)messageHandler, header);
        if(ret != 0) printf("Create pthread %s error!\n", tag);
    }
}

void sendMessage(const char *tag, int what) {
    if (tag == NULL) {
        return;
    }
    addMessageIntoQueue(tag, what);
}

void disposeMessage() {
    
}
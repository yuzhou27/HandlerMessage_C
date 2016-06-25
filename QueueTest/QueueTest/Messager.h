//
//  Messager.h
//  QueueTest
//
//  Created by yuzhou on 16/6/11.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#ifndef Messager_h
#define Messager_h

#include <stdio.h>
#include <stdbool.h>

#define HEADER_TAG "1stHeader"

typedef void *(msgHandlerCB)(void *arg);


/*
typedef struct MSGINFO {
    char tag[64];
    uint count;
    msgCB *cb;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} MsgInfo;
*/

typedef struct MESSAGE {
    void *context;
    //MsgInfo *info;
    int what;
    struct MESSAGE *next;
} Message;

typedef struct MESSAGEHEADER {
    char tag[64];           //第一个节点tag为：1stHeader
    unsigned int count;     //如果第一个节点count表示有多少message，否则表示每一个header后面有多少message
    msgHandlerCB *cb;
    Message *msgQueueHeader;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int fd;
    struct MESSAGEHEADER *next;
} MessageHeader;

bool InitMessageHeader();
MessageHeader *getMessageHeaderByTag(const char *tag);
//MessageHeader *createMessageHeader(const char *tag/*, msgHandlerCB *cb*/);
//void addMessageHeader(Message *header);
MessageHeader *NewAndAddMessageHeader(const char *tag, msgHandlerCB *cb);
void addMessage(const char *tag, Message *msg);
Message *createMessage(int what);
bool isNormalTag(const char *tag);
bool isExistedTag(const char *tag);
Message *initMessage(const char *tag, msgHandlerCB *cb);
Message *popupMessage(MessageHeader *header, int index);

#endif /* Messager_h */

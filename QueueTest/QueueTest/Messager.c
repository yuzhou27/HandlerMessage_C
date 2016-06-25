//
//  Messager.c
//  QueueTest
//
//  Created by yuzhou on 16/6/11.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include "Messager.h"
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>
#include <pthread.h>
#include "IPC.h"

//static Message *msgHeader = NULL;
static MessageHeader *msgHeader = NULL;

bool InitMessageHeader() {
    bool result = true;

    if (NULL == msgHeader) {
        msgHeader = (MessageHeader*)malloc(sizeof(MessageHeader));
        if (NULL == msgHeader) {
            result = false;
            printf("Can't create a Message Header!\n");
        } else {
            msgHeader->tag[0] = '\0';
            sprintf(msgHeader->tag, HEADER_TAG);
            msgHeader->count = 0;
            msgHeader->msgQueueHeader = NULL;
            pthread_mutex_init(&(msgHeader->mutex), NULL);
            pthread_cond_init(&(msgHeader->cond), NULL);
            msgHeader->fd = -1;
            msgHeader->next = NULL;
        }
    }
    
    return result;
}

bool isNormalTag(const char *tag) {
    bool result = true;
    if ((tag == NULL) || (msgHeader == NULL) || (strcmp(tag, HEADER_TAG) == 0)) {
        result = false;
    }
    
    return result;
}

bool isExistedTag(const char *tag) {
    bool result = true;
    for (MessageHeader *p = msgHeader; p != NULL; p = p->next) {
        if (strcmp(p->tag, tag) == 0) {
            result = false;
            break;
        }
    }
    
    return result;
}

MessageHeader *getMessageHeaderByTag(const char *tag) {
    MessageHeader *header = NULL;
    
    if ((NULL == tag) || (strcmp(tag, HEADER_TAG) == 0)) {
        printf("Warning: Can't get "HEADER_TAG"\n");
    } else if ((NULL != msgHeader) && (NULL != msgHeader->next)) {
        MessageHeader *p = msgHeader->next;
        for (; p != NULL; p = p->next) {
            if (strcmp(p->tag, tag) == 0) {
                header = p;
                //printf("Found Header Tag = %s\n", tag);
                break;
            }
        }
    }
    
    return header;
}

static MessageHeader *CreateMessageHeader(const char *tag/*, msgHandlerCB *cb*/) {
    MessageHeader *header = (MessageHeader*)malloc(sizeof(MessageHeader));
    if (NULL == header) {
        printf("Can't create Msg Header!\n");
    } else {
        header->msgQueueHeader = (Message*)malloc(sizeof(Message));
        if (NULL == header->msgQueueHeader) {
            printf("Can't create Msg Queue Header!\n");
        } else {
            header->next = NULL;
            header->tag[0] = '\0';
            sprintf(header->tag, tag, strlen(tag) + 1);
            header->cb = NULL;
            header->count = 0;
            header->fd = createClientSocket();
            header->msgQueueHeader->next = NULL;

            pthread_mutex_init(&(header->mutex), NULL);
            pthread_cond_init(&(header->cond), NULL);
        }
    }
    
    return header;
}

MessageHeader *NewAndAddMessageHeader(const char *tag, msgHandlerCB *cb) {
    bool result = isNormalTag(tag) && isExistedTag(tag);
    if (!result) {
        return NULL;
    }

    MessageHeader *header = CreateMessageHeader(tag);//(MessageHeader*)malloc(sizeof(MessageHeader));
    if (NULL == header) {
        return NULL;
    }

    header->cb = cb;
    
    MessageHeader *p = msgHeader;
    while (1) {
        if (p->next == NULL) {
            p->next = header;
            msgHeader->count++;
            break;
        } else {
            p = p->next;
        }
    }
    
    return header;
}

Message *createMessage(int what) {
    Message *msg = (Message*)malloc(sizeof(Message));
    if (NULL != msg) {
        msg->context = NULL;
        msg->next = NULL;
        msg->what = what;
        return msg;
    } else {
        return NULL;
    }
}

void addMessage(const char *tag, Message *msg) {
    bool result = isNormalTag(tag) && isExistedTag(tag);
    if (result) return;

    MessageHeader *header = getMessageHeaderByTag(tag);

    if (NULL == header) {
        printf("Can't add Message into NULL Header!\n");
    } else {
        pthread_mutex_lock(&(header->mutex));
        Message *p = header->msgQueueHeader;
        while (1) {
            if (NULL == p->next) {
                p->next = msg;
                header->count++;
                notify(header->tag);
                break;
            } else {
                p = p->next;
            }
        }
        pthread_mutex_unlock(&(header->mutex));
    }
}

//index from 0
Message *popupMessage(MessageHeader *header, int index) {
    //MessageHeader *header = getMessageHeaderByTag(tag);
    pthread_mutex_lock(&(header->mutex));
    if (header->count == 0) {
        pthread_mutex_unlock(&(header->mutex));
        return NULL;
    }

    if (index > header->count) {
        index = header->count - 1;
    }

    Message *p = header->msgQueueHeader;
    Message *q = NULL;
    if (NULL != p) {
        for (int i = 0; i <= index - 1; i++, p = p->next);
        q = p->next;
        p->next = p->next->next;
        header->count--;
    }

    pthread_mutex_unlock(&(header->mutex));
    
    return q;
}

void dumpMessage() {
    
}

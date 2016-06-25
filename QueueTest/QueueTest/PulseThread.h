//
//  PulseThread.h
//  QueueTest
//
//  Created by yuzhou on 16/6/10.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#ifndef PulseThread_h
#define PulseThread_h

#include <stdio.h>
#include <pthread.h>
#include "Messager.h"

bool createMessgaeQueue(const char *tag, msgHandlerCB *cb);
void createHandlerThread(const char *tag);
void sendMessage(const char *tag, int what);
void addMessageIntoQueue(const char *tag, int what);
void disposeMessage();

#endif /* PulseThread_h */

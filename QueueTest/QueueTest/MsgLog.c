//
//  MsgLog.c
//  QueueTest
//
//  Created by yuzhou on 16/6/18.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include "MsgLog.h"
#include <time.h>
#include <pthread.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <io.h>
static pthread_mutex_t LogMutex;
static char sprint_buf[1024];

void initLogPrint() {
    pthread_mutex_init(&LogMutex, NULL);
}

void Log(const char *format, ...) {
    pthread_mutex_lock(&LogMutex);
    time_t timer = time(NULL);
    struct tm *tblock;
    //timer = time(NULL);
    tblock = localtime(&timer);

    printf("%s    :", asctime(tblock));
    va_list args;
    int n;
    va_start(args, format);
    n = vsprintf(sprint_buf, format, args);
    va_end(args);

    //write(stdout, sprint_buf, n);
    pthread_mutex_unlock(&LogMutex);
}
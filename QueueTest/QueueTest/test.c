//
//  test.c
//  QueueTest
//
//  Created by yuzhou on 16/6/9.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "operate.h"
#include "test.h"
#include "PulseThread.h"
#include "Messager.h"
#include "MsgLog.h"

void test1() {
    initDataHeader();
    
    Data *d1 = (Data*)malloc(sizeof(Data));
    d1->next = NULL;
    addData(d1);
    
    Data *d2 = (Data*)malloc(sizeof(Data));
    d2->next = NULL;
    addData(d2);
    
    Data *d3 = (Data*)malloc(sizeof(Data));
    d3->next = NULL;
    addData(d3);
    
    Data *d4 = (Data*)malloc(sizeof(Data));
    d4->next = NULL;
    addData(d4);
    
    dumpData("After add 4 Data!");
    
    removeData(2);
    
    removeData(6);
    
    dumpData("Remove 2 Data!");
    
    Data *d = getData(20);
    printf("Get Data Num = %d\n", d->num);
    
    dispose();
}

void callback1(void *arg) {
    Message *msg = (Message*)arg;
    printf("callback, what = %d\n", msg->what);

    //should free mem
    free(msg);
    msg = NULL;
}

void test2() {
    Log("123");
    createMessgaeQueue(__FUNCTION__, (void*)callback1);
    createHandlerThread(__FUNCTION__);
    sendMessage(__FUNCTION__, 8);


}




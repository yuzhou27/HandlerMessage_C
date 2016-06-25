//
//  main.c
//  QueueTest
//
//  Created by yuzhou on 16/6/9.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "test.h"
#include "operate.h"

/*
void test() {
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
    
    //reverseData();
    
    dumpData("After reverse Data!");
    
    removeData(2);
    
    removeData(6);
    
    dumpData("Remove 2 Data!");
    
    Data *d = getData(20);
    printf("Get Data Num = %d\n", d->num);
    
    dispose();
}
*/

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    //char *out = (char*)malloc(sizeof(char) * 100);
    //subString("abcdefgeeafef", "ef", &out);
    //printf("Filter Out is %s\n", out);

    //test1();
    
    test2();

    while (1) {
        sleep(0xFFFF);
    }
    return 0;
}

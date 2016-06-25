//
//  operate.h
//  QueueTest
//
//  Created by yuzhou on 16/6/9.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#ifndef operate_h
#define operate_h
#include <stdio.h>

typedef struct DATA {
    char *context;
    int num;
    struct DATA *next;
}Data;

Data* getData(uint index);
void initDataHeader();
void addData(Data *data);
void dumpData(const char *Tag);
void dispose();
void removeData(uint index);
int subString(const char *source, const char *sub, char **out);

#endif /* operate_h */

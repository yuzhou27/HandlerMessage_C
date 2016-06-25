//
//  operate.c
//  QueueTest
//
//  Created by yuzhou on 16/6/9.
//  Copyright © 2016年 edu.self. All rights reserved.
//

#include "operate.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static Data *DataHeader;// = (Data*)malloc(sizeof(Data));
static uint count;

void initDataHeader() {
    DataHeader = (Data*)malloc(sizeof(Data));
    DataHeader->next = NULL;
}

void dispose() {
    if (NULL != DataHeader) {
        free(DataHeader);
        DataHeader = NULL;
    }
}

void addData(Data *data) {
    if (NULL == data) {
        //return;
    } else {
        Data *p = DataHeader;
        uint index = 1;
        while (1) {
            if (NULL == p->next) {
                data->num = index;
                p->next = data;
                //p->num = index;
                count++;
                break;
            } else {
                p = p->next;
                index++;
            }
        }
    }
}

void removeData(uint index) {
    if (index > count) {
        printf("Out Of Range!\n");
    } else {
        Data *p = DataHeader->next;
        Data *q = DataHeader;
        
        while(1) {
            if ((NULL != p) && (p->num == index)) {
                //remove
                q->next = p->next;
                count--;
                free(p);
                p = NULL;
                break;
            } else {
                if (NULL == p->next) {
                    printf("No Found!\n");
                    break;
                }
                q = p;//q->next;
                p = p->next;
            }
        }
    }
}

void insertData(uint index, Data *data) {
    if (index > count) {
        addData(data);
    } else if (index <= 0) {
        index = 1;
    }
    Data *p = DataHeader->next;
    Data *q = DataHeader;
    
    for (int i = 1; p != NULL; q = p, p = p->next, i++) {
        if (index <= i) {
            //insert
            data->next = p->next;
            q->next = data;
            count++;
        }
    }
}

Data* getData(uint index) {
    if (index >= count) {
        index = count;
    }
    
    Data *p = DataHeader;
    for (uint i = 0; (NULL != p->next) && (i < index); p = p->next, i++);
    
    return p;
}

void reverseData() {
    Data *newHeader = (Data*)malloc(sizeof(Data));
    newHeader->next = NULL;
    Data *p = DataHeader;
    Data *q = newHeader;

    for (; (p != NULL); p = p->next) {
        printf("Test p = %d\n", p->num);
        if ((p->next != NULL) && (p->next->next == NULL)) {
            for (q = newHeader; q != NULL; q = q->next) {
                if (q->next == NULL) {
                    q->next = p->next;
                    break;
                }
            }

        }
    }
    
    printf("Test p = %d\n", p->num);
}

void dumpData(const char *Tag) {
    Data *p = DataHeader->next;
    
    printf("Begin to Dump %s, Data Count = %d\n", Tag, count);
    for (uint index = 1; p != NULL; p = p->next, index++) {
        printf("Node%d num = %d\n", index, p->num);
    }
}

int subString(const char *source, const char *sub, char **out) {
    printf("Source = %s, sub = %s\n", source, sub);
    printf("source[0]=%c\n", source[0]);
    unsigned long subLength = strlen(sub);
    unsigned long srcLength = strlen(source);
    int count = 0;

    strcpy(*out, "TODO!");
    if ((0 == subLength) || (0 == srcLength)) {
        printf("Null string!\n");
    } else {
        for (uint i = 0; i < srcLength; i++) {
            const char *src = source + i;
            bool isMatch = false;
            for (uint j = 0; j < subLength; j++) {
                if (src[j] == sub[j]) {
                    isMatch = true;
                } else {
                    isMatch = false;
                    break;
                }
            }
            
            if (isMatch) {
                printf("Found sub String at %d\n", i);
                count++;
            }
            
            //strncpy(*out, );
        }
    }
    
    return count;
    //for ()
    ///for ()
}

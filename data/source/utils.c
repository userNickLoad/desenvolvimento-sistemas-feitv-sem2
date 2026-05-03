#include "data.h"
#include <windows.h>

int compare_str(char *str1, char *str2){
    int i = 0;
    while(1){
        if(str1[i] != str2[i] || (str1[i] == '\0' && str2[i] != '\0') || (str2[i] == '\0' && str1[i] != '\0')) return 0;

        if((str1[i] == '\0' && str2[i] == '\0')) return 1;

        i++;
    }
}

void copy_str(char *to, char *from){
    int i = 0;
    while(1){
        to[i] = from[i];
        if(from[i] == '\0') return;
        i++;
    }
}

void copy_struct(void *to, void *from, int size){
    char *to_ = to, *from_ = from;

    for (int i = 0; i < size; i++){
        to_[i] = from_[i];
    }
}


void ajust_info(char *stream, char *src, int size, char end_char){
    for (int i = 0; i < size-1; i++)
        stream[i] = ' ';
    copy_str(stream, src);
    stream[size-1] = ';';
}
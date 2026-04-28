#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>

#include <malloc.h>

#ifdef _WIN32
    #define CLEAR_TERMINAL system("cls"); // Windows
#else
    #define CLEAR_TERMINAL system("clear"); // Linux/Unix
#endif


#endif
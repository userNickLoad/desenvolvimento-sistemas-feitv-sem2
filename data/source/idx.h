#pragma once
#include "data.h"
#include "../../header.h"

#define search_recursive(name, fl_path, SCAN_MASK, type, search)\
int name##_recursive_search(FILE * fl, unsigned int idx)\
{\  
    if(idx == 0) return 0;\
    unsigned int prt, height, left, right;\
    type *value;\
    fscanf(fl, SCAN_MASK, &prt, value, &left, &right);\
    int comp = search();\
\
\
}\

#define search(name, fl_path, SCAN_MASK, type, search_method)\
int name##_bsearch(type val){\
    FILE *fl = fopen(fl_path, "r");\
    unsigned int amount, ln_size, head;\
\
    fscanf(fl, IDX_HEADER_MASK, amout, ln_size, head);\
\
    idx = \
\
    fclode(fl);\
}


#define idx(name, fl_path, SCAN_MASK, PRINT_MASK, type, search_method)\
    search_recursive\
    search(name, fl_path, SCAN_MASK, type, search_method)





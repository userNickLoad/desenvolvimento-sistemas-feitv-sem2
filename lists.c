#include "lists.h"
#include "header.h"
#include "data/source/data.h"

declara_list_impl(ptr, prt_l);

dinamic_list_impl(char, dina_char, 0);

dinamic_list_impl(void *, dina_prt, 0);

void add_title(char *title, prt_l_list *title_l)
{
    char *t = malloc(sizeof(char)*21);
    copy_str(t, title);

    add_prt_l(t, title_l);
}

void pop_title(prt_l_list *title_l)
{
    char *t = (char *)title_l->tail->value;
    free(t);
    pop_prt_l(title_l);
    
}

void free_title(prt_l_list *title_l)
{
    while (title_l->size > 0)
    {
        pop_title(title_l);
    }
    
    free_prt_l(title_l);
}
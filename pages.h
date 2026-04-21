#ifndef PAGED_H
#define PAGED_H
#include "header.h"
#include "lists.h"

typedef struct Page
{
    Str_list link;
    char *title;
    char *description;
    char **opcoes;
    struct Page **nxt;
    struct Page *lst;
    void *payload;
    void (*render_payload)(struct Page *);
    void (*action)(struct Page *, int);

} Page;

Page * page_inicio(void);

void render(Page * page);

Page *build_page(char title[20], char description[250], char **opcoes, Page **nxt, Page *lst, void * render_payload, void *action);

void live_page(Page * page);

#endif
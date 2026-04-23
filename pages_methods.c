#include "pages.h"
#include "lists.h"

void render(Page * page){
    node_char_ptr *crr_link = page->link.head;
    while(crr_link != NULL){
        printf("%s/", crr_link->value);
        crr_link = crr_link->nxt;
    }

    printf("\n\n");
    if(page->description != NULL){
        printf("%s", page->description);

        printf("\n\n");
    }

    for(int i = 0; i < dinamic_size(page->opcoes); i++){
        printf("\t\t- [%d]: %s;\n", i, page->opcoes[i]);
    }
}

Page *build_page(char title[20], char description[250], char **opcoes, Page **nxt, Page *lst, void * render_payload, void *action)
{   
    Page * this_p = (Page *) malloc(sizeof(Page));

    this_p->link = *Str_init_list();

    this_p->title = title;
    this_p->description = description;
    this_p->opcoes = opcoes;
    this_p->nxt = nxt;
    this_p->lst = lst;
    this_p->render_payload = render_payload;
    this_p->action = action;

    return this_p;
}

Page * live_page(Page * page){
    if(page->lst){
        page->link = page->lst->link;
    }

    add_Str(page->title, &page->link);

    render(page);



   
}
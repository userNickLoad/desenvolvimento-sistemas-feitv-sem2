#include "header.h" 
#include "pages.h"


int main(void)
{
    Page * crr_page = (Page *) malloc(sizeof(Page));

    if(crr_page == NULL){
        fprintf(stderr, "NÃO FOI POSSIVEL ALOCAR ESPACO NA HEAP PARA GUARDAR AS PAGINAS");
        return -1;
    }

    crr_page->link = NULL;

    page_inicio(crr_page);

    while (1) {
        live_page(crr_page);
        
        if (crr_page == NULL) return -1;

    }


    return 0;
}
#include "../pages.h"

Page * link_pages(void) {
    /*INVOCA PAGINAS E AS CONSTROI*/
    Page * pag_inicio__ = page_inicio();

    Page * pag_login__ = page_login();

    Page * pag_signup__ = page_signup();

    /*MONTA SEQUENCIA DE PAGINAS*/

    //pag_inicio
    pag_inicio__->nxt = dina_pags_init(2);

    dina_pags_add(pag_login__, pag_inicio__->nxt);

    dina_pags_add(pag_signup__, pag_inicio__->nxt);
    //pag_inicio

    //pag_login
    pag_login__->
    //pag_login


    /**/
    return pag_inicio__;
}
#include "../pages.h"
#include "../lists.h"

Page * link_pages(void) {
    Page * pag_inicio__ = page_inicio();

    Page * pag_login__ = page_login();

    Page * pag_signup__ = page_signup();

    pag_inicio__->nxt =

}
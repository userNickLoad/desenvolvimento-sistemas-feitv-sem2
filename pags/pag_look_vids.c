#include "../pages.h"
#include "../header.h"

void page_look_vids(Page *this_p) {
    ChangePage *lst = malloc(sizeof(ChangePage));
    lst->free_all = NULL;
    lst->build = page_home;

    build_page(
        "videos",
        NULL,
        NULL,
        NULL,
        NULL,
        lst,
        NULL,
        NULL,
        NULL,
        this_p
    );
}
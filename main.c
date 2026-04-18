#include "header.h"
#include "lists.h"

declara_list(int, linker)


int main(void) {
    linker_list * list = linker_init_list();

    add_linker(10, list);
    add_linker(20, list);
    add_linker(30, list);
    node_int * crr = list->head;

    while (1) {
        printf("%d", crr->value);

        if (crr->nxt == NULL) {
            break;
        }
        crr = crr->nxt;
    }
    return 0;
}
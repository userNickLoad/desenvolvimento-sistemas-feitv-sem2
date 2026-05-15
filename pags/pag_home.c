#include "../pages.h"
#include "../header.h"

void page_home(Page *this_p) {

    char **ops =  (char **) dina_prt_init(2);
    ChangePage *nxt =  dina_chPage_init(2);

    add_opcao("Buscar por videos", ops, 20);
    add_nxt_pag(page_look_vids, NULL, nxt);

    add_opcao("Ver suas playlists", ops, 20);
    add_nxt_pag(page_look_playlists, NULL, nxt);


    build_page(
        "home",
        "\tSeja bem vindo!\n\n\tEscolha uma das opcoes a baixo para comecar sua experiencia: ",
        NULL,
        ops,
        nxt,
        NULL,
        NULL,
        NULL,
        NULL,
        this_p
    );
}
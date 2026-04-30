#include "../schema.h"
#include  "../pages.h"
#include "../header.h"

void name_action(Page *this_p) {
    User *user = (User *) this_p->data.payload;

    insert_terminal("Crie um nome para o seu usuario: ", user->name, sizeof(user->name));
}
void name(void *_, Page *this_p) {
    build_page(
        "name", 
        NULL, 
        NULL, 
        NULL,
        NULL,
        page_inicio, 
        NULL, 
        NULL,
        NULL, 
        name_action, 
        this_p
    );
}

void page_signup(Page *this_p) {
    User *user;
    if (this_p->data.payload == NULL) {
        user = (User *) malloc(sizeof(User));
        user->id = 0;
        user->name[0] = '\0';
        user->password[0] = '\0';
        this_p->data.payload = user;
    }

    user = this_p->data.payload;

    char **ops = (char **) dina_prt_init(2);
    void **nxt = dina_prt_init(2);

    ops = add_opcao("Crie um nome para o seu usuario:", ops, 85);
    for (int i = 0; i < dinamic_size(user->name); i++) {
        ops[0][i + 42] = user->name[i];
        if (user->name[i] == '\0') break;
    }

    ops = add_opcao("Crie uma senha a senha:", ops, 50);
    for (int i = 0; i < dinamic_size(user->password); i++) {
        ops[0][i + 24] = user->password[i];
        if (user->password[i] == '\0') break;
    }

    add_nxt_pag(name, 0, nxt)

    build_page("signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;", NULL, ops,
                    nxt, page_inicio, NULL, NULL, NULL, NULL, this_p);
}

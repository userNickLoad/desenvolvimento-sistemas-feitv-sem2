#include "../schema.h"
#include  "../pages.h"
#include "../header.h"

void name_action(Page *this_p) {
    User *user = (User *) this_p->data.payload;

    insert_terminal("Crie um nome para o seu usuario", user->name, sizeof(user->name));

    printf("Isso foi o que foi: %s de enter", user->name);
    getchar();

    this_p->selected = GO_BACK;
}
void name(Page *this_p) {

    ChangePage *lst = malloc(sizeof(lst));

    lst->free_all = 0;
    lst->build = page_signup;

    build_page(
        "name", 
        NULL, 
        NULL, 
        NULL,
        NULL,
        lst, 
        NULL, 
        NULL,
        NULL, 
        name_action, 
        this_p
    );
}

void clear(Page * this_p){
    clearFn_defualt(this_p);
    printf("TA LIMPANDO");
    getchar();
    User *user = this_p->data.payload;
    free(user);
    this_p->data.payload = NULL;
}

void page_signup(Page *this_p) {

    User *user;
    if (this_p->data.payload == NULL) {
        printf("TA LIMPANDO");
        getchar();
        user = (User *) malloc(sizeof(User));
        user->id = 0;
        user->name[0] = '\0';
        user->password[0] = '\0';
        this_p->data.payload = user;
    }

    user = this_p->data.payload;

    char **ops = (char **) dina_prt_init(2);
    ChangePage *nxt = dina_chPage_init(2);
    ChangePage *lst = malloc(sizeof(lst));

    ops = add_opcao("Crie um nome para o seu usuario:", ops, 85);
    for (int i = 0; i < dinamic_size(user->name); i++) {
        ops[0][i + 32] = user->name[i];
        if (user->name[i] == '\0') break;
    }

    ops = add_opcao("Crie uma senha a senha:", ops, 50);
    for (int i = 0; i < dinamic_size(user->password); i++) {
        ops[0][i + 23] = user->password[i];
        if (user->password[i] == '\0') break;
    }

    add_nxt_pag(name, 0, nxt)

    lst->build = page_inicio;
    lst->free_all = 1;

    build_page("signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;", NULL, ops,
                    nxt, lst, clear, NULL, NULL, NULL, this_p);
}

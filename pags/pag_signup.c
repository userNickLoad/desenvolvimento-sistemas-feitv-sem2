
#include "../schema.h"
#include  "../pages.h"
#include "../header.h"

// void action(Page * this_p){
//     User *user = (User *) this_p->data.payload;
//     char confirm_password[20];

//     insert_terminal("Crie um nome para o seu usuario", user->name, 50);
//     insert_terminal("Crie uma senha a senha", user->password, 20);
//     insert_terminal("Confirme a sua senha", confirm_password, 20);
// }

void page_signup(void *_, Page *this_p) {
    User * user;
    if(this_p->data.payload == NULL){
        user = (User *) malloc(sizeof(User));
        user->id = 0;
        user->name[0] = '\0';
        user->password[0] = '\0';

        this_p->data.payload = user;
    }

    user = this_p->data.payload;

    char ** ops = (char **)dina_prt_init(2);
    void ** nxt = dina_prt_init(2);

    ops = add_opcao("Crie um nome para o seu usuario :", ops, 85);
    for (int i = 0; i < dinamic_size(user->name); i++){
        ops[0][i+42] = user->name[i];
        if(user->name[i] == '\0') break;
    }

    ops = add_opcao("Crie uma senha a senha :", ops, 50);
    for (int i = 0; i < dinamic_size(user->password); i++){
        ops[0][i+24] = user->name[i];
        if(user->name[i] == '\0') break;
    }

    

    return build_page("signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;", NULL, ops, nxt, page_inicio, NULL, NULL, this_p);
}
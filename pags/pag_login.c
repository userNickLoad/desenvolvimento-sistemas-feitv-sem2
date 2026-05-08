#include "../data/source/data.h"
#include "../schema.h"
#include "../pages.h"
#include "../header.h"
#include "utils.h"

generate_gettxt_page(lg_name, "Coloque o seu usuario", User, name, validateCh_login, page_login)

generate_gettxt_page(lg_password, "Coloque a sua senha", User, password, validateCh_login, page_login)

void clear_login(Page *this_p)
{
    if (this_p->data.payload != NULL)
    {
        User *user = this_p->data.payload;
        free(user);
        this_p->data.payload = NULL;
    }
    if (this_p->data.response != NULL)
    {
        free_response(this_p->data.response, User)
            this_p->data.response = NULL;
    }
}

void login_request(Page *this_p)
{
    pop_Str(this_p->link);

    Response *res = this_p->data.response;
    if (res == NULL)
    {
        res = malloc(sizeof(Response));
        this_p->data.response = res;
        res->data = NULL;
    }

    User *user = this_p->data.payload;
    if (user == NULL)
    {
        res->code = 400;
        res->data = NULL;
        copy_str(res->msg, "Payload mal formulado");
        page_login(this_p);
        return;
    }

    Response res_login = login(user->name, user->password);
    if (res_login.code != 200)
    {
        copy_struct(res, &res_login, sizeof(Response));
        page_login(this_p);
        return;
    }

    clear_login(this_p);

    user = (User *) res_login.data;
    this_p->data.user.id = user->id;
    copy_str(this_p->data.user.name, user->name);
    free(user);
    page_home(this_p);
}

void page_login(Page *this_p)
{
    User *user;
    if (this_p->data.payload == NULL)
    {
        user = (User *)malloc(sizeof(User));
        user->id = 0;
        user->name[0] = '\0';
        user->password[0] = '\0';
        this_p->data.payload = user;
    }

    user = this_p->data.payload;

    char **ops = (char **)dina_prt_init(3);
    ChangePage *nxt = dina_chPage_init(3);
    ChangePage *lst = malloc(sizeof(ChangePage));

    char ops1[85];
    sprintf(ops1, "Coloque o seu usuario: %s", user->name);
    ops = add_opcao(ops1, ops, sizeof(ops1));

    char ops2[50];
    sprintf(ops2, "Coloque a sua senha: %s", user->password);
    ops = add_opcao(ops2, ops, sizeof(ops2));

    add_nxt_pag(lg_name, NULL, nxt)
    add_nxt_pag(lg_password, NULL, nxt)

    if (user->name[0] != '\0' && user->password[0] != '\0')
    {
        ops = add_opcao("Entrar", ops, 11);
        add_nxt_pag(login_request, NULL, nxt)
    }

    lst->build = page_inicio;
    lst->free_all = clear_login;

    char *question = NULL;

    if ((user->name[0] == '\0' || user->password[0] == '\0') && this_p->data.response == NULL)
        question = "\n\tNome ou senha nao preenchidos! Preencha os campos para dar continuidade.";
    if (this_p->data.response != NULL)
    {
        Response *res = (Response *)this_p->data.response;
        question = res->msg;
    }

    build_page(
        "login",
        "Area de login: \n\t\t+ Coloque o seu usuario e senha;\n\t\t + Valem apenas letras de [A-z], numeros de [0-9] e anderline [_]",
        question,
        ops,
        nxt,
        lst,
        NULL,
        NULL,
        NULL,
        this_p);
}
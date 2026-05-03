#include "../data/source/data.h"
#include "../schema.h"
#include "../pages.h"
#include "../header.h"


void password_action(Page *this_p)
{
    User *user = (User *)this_p->data.payload;

    insert_terminal("Crie uma senha a senha", user->password, sizeof(user->password));

    this_p->selected = GO_BACK;
}
void password(Page *this_p)
{

    ChangePage *lst = malloc(sizeof(lst));

    lst->free_all = 0;
    lst->build = page_signup;

    build_page(
        "password",
        NULL,
        NULL,
        NULL,
        NULL,
        lst,
        NULL,
        NULL,
        password_action,
        this_p);
}

void name_action(Page *this_p)
{
    User *user = (User *)this_p->data.payload;

    insert_terminal("Crie um nome para o seu usuario", user->name, sizeof(user->name));

    this_p->selected = GO_BACK;
}
void name(Page *this_p)
{

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
        name_action,
        this_p);
}

void clear(Page *this_p)
{   

    if (this_p->data.payload != NULL)
    {
        User *user = this_p->data.payload;
        free(user);
        this_p->data.payload = NULL;
    }
    if (this_p->data.response != NULL){
        free_response(this_p->data.response)
    }
}

void signup_request(Page *this_p)
{   
    Response *res = malloc(sizeof(Response));
    res->data = NULL;

    if (this_p->data.response != NULL){
        free_response(this_p->data.response)
    }

    if (this_p->data.payload == NULL)
    {   
        res->code = 400;
        copy_str(res->msg, "Payload mal formulado");
        this_p->data.response = res;
        page_signup(this_p);
        return;
    }

    User *user = (User *) this_p->data.payload;

    Response res_signup = signup(user->name, user->password);

    if(res_signup.code != 200){
        copy_struct(res, &res_signup, sizeof(User));
        this_p->data.response = res;
        page_signup(this_p);
        return;
    }

    clear(this_p);
    page_login(this_p);

}

void page_signup(Page *this_p)
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
    ChangePage *lst = malloc(sizeof(lst));

    char ops1[85];
    sprintf(ops1, "Crie um nome para o seu usuario: %s", user->name);
    ops = add_opcao(ops1, ops, sizeof(ops1));

    char ops2[50];
    sprintf(ops2, "Crie uma senha a senha: %s", user->password);
    ops = add_opcao(ops2, ops, sizeof(ops2));

    add_nxt_pag(name, NULL, nxt)
    add_nxt_pag(password, NULL, nxt)

    if (user->name[0] != '\0' && user->password[0] != '\0')
    {
        ops = add_opcao("Cadastrar.", ops, 11);
        add_nxt_pag(NULL, NULL, nxt)
    }

    lst->build = page_inicio;
    lst->free_all = clear;

    char *question = NULL;

    if ((user->name[0] == '\0' || user->password[0] == '\0') && this_p->data.response == NULL)
        question = "\n\tNome ou senha nao preenchidos! Preencha os campos para dar continuidade.";

    if (this_p->data.response != NULL)
    {
        Response *res = (Response *)this_p->data.response;
        question = res->msg;
    }

    build_page("signup", "Area de cadastro: \n\t\t+ Coloque o seu usuario, senha e confirme a senha;", question, ops,
               nxt, lst, NULL, NULL, NULL, this_p);
}

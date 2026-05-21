#include "data.h"
#include "../../lists.h"

void user_by_name(char * line, void * data_filter, void ** list_save)
{
    //Le linha pega
    User temp, *df = data_filter;
    USER_SCAN(line, temp)

    //se o nome não bater, não salva
    if(!compare_str(temp.name, df->name)) return;

    User *user = malloc(sizeof(User));
    copy_struct(user, &temp, sizeof(User));

    dina_prt_add(user, list_save);
}

Response *login(char *name, char *password)
{
    Response *res = malloc(sizeof(Response));
    User user_filter;

    copy_str(user_filter.name, name);

    User **query =  (User **)read_fl("User", user_by_name, 1, &user_filter);

    User *user = query[0];

    dinamic_free(void *, query)

    // se não achou usuario, é pq o nome está incorreto
    if (user == NULL)
    {
        res->code = 401;
        copy_str(res->msg, "\n\tNome ou senha devem estar incorretos.");
        return res;
    }

    // se não achou usuario, mas a senha está incorreta
    if (!compare_str(user->password, password))
    {   
        free(user);
        res->code = 401;
        copy_str(res->msg, "\n\tNome ou senha devem estar incorretos.");
        return res;
    }

    res->code = 200;
    res->data = user;
    copy_str(res->msg, "\n\tUsuario logado.");
    return res;
}

Response *signup(char *name, char *password)
{
    Response *res = malloc(sizeof(Response));
    User user_filter;

    copy_str(user_filter.name, name);

    User **query_user = (User **)read_fl("User", user_by_name, 1, &user_filter);

    if (dinamic_size(query_user) > 0)
    {   
        res->code = 400;
        res->data = NULL;
        sprintf(res->msg, "\n\tUsuario %s ja existe", name);
        free(query_user[0]);
        dinamic_free(void *, query_user)
        return res;
    }

    append_line("User", 1, USER_PRINT_MASK, 0, 0, name, password);

    res->code = 200;
    copy_str(res->msg, "\n\tUsuario criado");

    return res;
}
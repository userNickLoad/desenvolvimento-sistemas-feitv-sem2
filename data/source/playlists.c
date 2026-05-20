#include "data.h"
#include "../../lists.h"

void save_playlists_query(char *line, void *data_filer, void **data_selected)
{
    Playlist playlist;
    Playlist *playlist_filter = (Playlist *)data_filer;

    sscanf(line, PLAYLIST_SCAN_MASK, &playlist.id, &playlist.user_id, playlist.name, playlist.description);

    if (playlist.user_id != playlist_filter->user_id)
        return;

    if (!compare_titles(playlist.name, playlist_filter->name, sizeof(playlist.name)))
        return;

    Playlist *playlist_selected = malloc(sizeof(Playlist));

    dina_prt_add(playlist_selected, data_selected);
}

Response *handle_get_playlists(unsigned int user_id, char *title)
{
    Response *res = malloc(sizeof(Response));
    res->msg[0] = '\0';

    Playlist *playlist = malloc(sizeof(Playlist));
    playlist->user_id = user_id;
    copy_str(&playlist->name, title);

    res->data = read_fl("Playlist", save_playlists_query, playlist);

    free(playlist);

    if (dinamic_size(res->data) < 1)
    {
        dinamic_free(Playlist, res->data)
            res->code = 404;
        copy_str(res->msg, "\tNao foi possivel localizar playlists.");

        return res;
    }

    res->code = 200;

    return res;
}

Response *handle_create_playlists(unsigned int user_id, char *title)
{
    // Inicia respose
    Response *res = malloc(sizeof(Response));
    res->msg[0] = '\0';

    // Cria playlist usada como filtro para dados
    Playlist *playlist = malloc(sizeof(Playlist));
    playlist->user_id = user_id;
    copy_str(&playlist->name, title);

    // Verifica se o usuario possui alguma outra playlist de mesmo titulo
    Response *playlists = read_fl("Playlist", save_playlists_query, playlist);

    // libera filtro pois não vai ser usado mais
    free(playlist);

    // se a resposta da query for maior que, então o usuario possui outra playlist de mesmo titulo
    if (dinamic_size(res->data) > 0)
    {   
        //limpa lista com os resultados
        Playlist **clear = res->data
        free_prt_list(clear, dinamic_size(res->data))
        dinamic_free(Playlist, res->data);

        //retorna erro
        res->code = 403;
        copy_str(res->msg, "\tJá existe outra playlist com o nome.");

        return res;
    }

    append_line("Playlist", 1, PLAYLIST_PRINT_MASK, user_id)
    

    res->code = 200;
}
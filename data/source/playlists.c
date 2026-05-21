#include "data.h"
#include "../../lists.h"

void filter_playlists_title(char *line, void *data_filter, void **list_save)
{
    Playlist temp, *playlist_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    if(!(compare_str(playlist_filter->name, temp.name, sizeof(playlist_filter->name) && playlist_filter->user_id == temp.user_id))) return;

    Playlist *save_playlist = malloc(sizeof(Playlist));
    copy_struct(save_playlist, &temp, sizeof(Playlist));
    dina_prt_add(save_playlist, list_save);
}

void playlist_by_id(char *line, void *data_filter, void ** list_save)
{
    Playlist temp, *playlist_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    if(playlist_filter->id != temp.id) return;

    Playlist *save_playlist = malloc(sizeof(Playlist));
    copy_struct(save_playlist, &temp, sizeof(Playlist));
    dina_prt_add(save_playlist, list_save);
}

int erase_playlist_by_id(char *line, void *data_filter)
{
    Playlist temp, *playlist_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    return playlist_filter->id == temp.id;
}

int erase_playlist_video_by_playlist_id(char *line, void *data_filter)
{
    Playlist_Video temp, *playlist_filter = data_filter;

    PLAYLIST_VID_SCAN(line, temp)

    return playlist_filter->playlist_id == temp.playlist_id;
}

int erase_playlist_video_by_playlist_ids(char *line, void *data_filter)
{
    Playlist_Video temp, *playlist_filter = data_filter;

    PLAYLIST_VID_SCAN(line, temp)

    return playlist_filter->playlist_id == temp.playlist_id && playlist_filter->video_id == temp.video_id;
}

void filter_playlists_title_starts_with(char *line, void *data_filter, void **list_save)
{
    Playlist temp, *playlist_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    if(!(compare_titles(playlist_filter->name, temp.name, sizeof(playlist_filter->name) && playlist_filter->user_id == temp.user_id))) return;

    Playlist *save_playlist = malloc(sizeof(Playlist));
    copy_struct(save_playlist, &temp, sizeof(Playlist));
    dina_prt_add(save_playlist, list_save);
}

int update_increase_vids(char *line, void *data_filter)
{
    Playlist temp, *pv_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    if(!(temp.id == pv_filter->id)) return 0;

    temp.videos++;

    PLAYLIST_PRINT(line, temp);

    return 1;
}

int update_decrease_vids(char *line, void *data_filter)
{
    Playlist temp, *pv_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    if(!(temp.id == pv_filter->id)) return 0;

    temp.videos--;

    PLAYLIST_PRINT(line, temp);

    return 1;
}

void playlist_video_by_ids(char *line, void *data_filter, void **list_save)
{
    Playlist_Video temp, *pv_filter = data_filter;

    if(!(temp.video_id == pv_filter->video_id && temp.playlist_id == pv_filter->playlist_id)) return;

    Playlist_Video *save_pv = malloc(sizeof(Playlist_Video));
    copy_struct(save_pv, &temp, sizeof(Playlist_Video));
    dina_prt_add(save_pv, list_save);
}

void playlist_videos_by_playlist_id(char *line, void *data_filter, void **list_save){
    Playlist_Video temp, *pv_filter = data_filter;

    PLAYLIST_VID_SCAN(line, temp);

    if(!(temp.playlist_id == pv_filter->playlist_id)) return;

    Playlist_Video *pv_save = malloc(sizeof(Playlist_Video));
    copy_struct(pv_save, &temp, sizeof(Playlist_Video));
    dina_prt_add(pv_save, list_save);
}

int update_videos_order_deleted(char *line, void *data_filter)
{
    Playlist_Video temp, *pv_filter = data_filter;

    PLAYLIST_VID_SCAN(line, temp)

    if(!(pv_filter->playlist_id == temp.playlist_id && pv_filter->place < temp.place)) return 0;

    temp.place--;

    PLAYLIST_VID_SCAN(line, temp)
    return 1;
}

void join_playlist_videos(char *line, void *data_filter, void **list_save)
{
    Video temp;
    Playlist_Video **videos_filter = data_filter;

    VIDEO_SCAN(line, temp)

    int fund = 0;
    int place = 0;
    for (int i = 0; i < dinamic_size(data_filter); i++){
        if(temp.id == videos_filter[i][0].video_id)
        {
            fund = 1;
            place = videos_filter[i][0].place;
            break;
        }
    }

    if(!fund) return;
    Video *save_video = malocc(sizeof(Video));
    copy_struct(save_video, &temp, sizeof(Video));

    Videos_PV_DTO *save_dto = malocc(sizeof(Videos_PV_DTO));
    save_dto->videos = save_video;
    save_dto->place = place;
  
    dina_prt_add(save_dto, list_save);
}

Response *create_playlist(unsigned int user_id, char * title, char * description)
{   
    Response *res = malloc(sizeof(Response));

    Playlist playlist_filter;
    playlist_filter.user_id = user_id;
    copy_str(playlist_filter.name, title);

    Playlist **same_title_query = read_fl("Playlist", filter_playlists_title, 1, &playlist_filter);

    if (same_title_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query same_title_query");
        res->data = NULL;
        return res;
    }

    if(dinamic_size(same_title_query) > 0)
    {      
        free(same_title_query[0]);
        dinamic_free(void *, same_title_query)

        res->code = 403;
        snprintf(res->msg, 50, "voce ja possui a pl %20s", title);
        res->data = NULL;
        return res;
    }

    dinamic_free(void *, same_title_query)

    append_line("Playlist", 1, PLAYLIST_PRINT_MASK, 0, 0, title, description);

    res->code = 200;
    res->data = NULL;
    snprintf(res->msg, 50, "pl %20s criada", title);

    return res;
}

Response *delete_playlist(unsigned int user_id, unsigned int playlist_id)
{
    Response *res = malloc(sizeof(Response));

    Playlist playlist_filter;
    playlist_filter.user_id = user_id;
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = read_fl("Playlist", filter_playlists_title, 1, &playlist_filter);

    if (playlist_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query playlist_query");
        res->data = NULL;
        return res;
    }

    if(dinamic_size(playlist_query) == 0)
    {      
        dinamic_free(void *, playlist_query)

        res->code = 404;
        copy_str(res->msg, "Esta playlist nao foi encontrada");
        res->data = NULL;
        return res;
    }

    Playlist *playlist = playlist_query[0];
    dinamic_free(void *, playlist_query)

    Playlist_Video playlist_video_filter;
    playlist_video_filter.playlist_id = playlist_id;

    erase_line("Playlist_video", erase_playlist_video_by_playlist_id, playlist->videos, &playlist_video_filter);

    erase_line("Playlist", erase_playlist_by_id, 1, playlist);

    res->code = 200;
    snprintf(res->msg, 50, "playlist %20s deletada com sucesso.", playlist->name);
    res->data = NULL;

    free(playlist);

    return res;
}

Response *search_for_playlists(unsigned int user_id, char * title)
{
    Response *res = malloc(sizeof(Response));

    Playlist playlist_filter;
    playlist_filter.user_id = user_id;
    copy_str(playlist_filter.name, title);

    Playlist **playlists = read_fl("Playlist", filter_playlists_title_starts_with, 0, &playlist_filter);

    if (playlists == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query playlists");
        res->data = NULL;
        return res;
    }

    if(dinamic_size(playlists) == 0)
    {      
        dinamic_free(void *, playlists)

        res->code = 404;
        copy_str(res->msg, "Voce nao possui playlists, crie alguma");
        res->data = NULL;
        return res;
    }

    res->code = 200;
    res->data = playlists;
    copy_str(res->msg, "playlists achadas");

    return res;
}

Response *add_video_playlist(unsigned int playlist_id, unsigned int video_id)
{
    Response *res = malloc(sizeof(Response));

    //Pegar quantidade de videos
    ////////////
    Playlist playlist_filter = {0};
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = read_fl("Playlist", playlist_by_id, 1, &playlist_filter);
    
    if (playlist_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query plalist_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(playlist_query) == 0)
    {   
        dinamic_free(void *, playlist_query)
        res->code = 404;
        copy_str(res->msg, "playlist nao encontrada");
        res->data = NULL;
        return res;
    }

    Playlist *playlist = playlist_query[0];
    ////////////
    //

    //verificar duplicidade
    ////////////
    if(playlist->videos > 0){
        Playlist_Video playlist_video_filter = {0};
        playlist_video_filter.playlist_id = playlist_id;
        playlist_video_filter.video_id = video_id;

        Playlist_Video **video_already_in_query = read_fl("Playlist_video", playlist_video_by_ids, 1, &playlist_video_filter);

        if (video_already_in_query == NULL)
        {   
            res->code = 500;
            copy_str(res->msg, "erro ao fazer query video_already_in_query");
            res->data = NULL;
            return res;
        }

        if (dinamic_size(video_already_in_query) > 0)
        {      
            free_prt_list(video_already_in_query, dinamic_size(video_already_in_query))
            dinamic_free(void *, video_already_in_query)

            res->code = 403;
            copy_str(res->msg, "Proibido colocar o mesmo video mais de uma vez");
            res->data = NULL;
            return res;
        }

        if (dinamic_size(video_already_in_query) > 0)
        {      
            free(video_already_in_query[0]);
            dinamic_free(void *, video_already_in_query)
            
            res->code = 403;
            copy_str(res->msg, "Proibido colocar o mesmo video mais de uma vez");
            res->data = NULL;
            return res;
        }

        dinamic_free(void *, video_already_in_query)
    }    
    ////////////
    //


    append_line("Playlist_video", 0, PLAYLIST_VID_PRINT_MASK, playlist_id, video_id, playlist->videos);

    update_fl("Playlist", update_increase_vids, 1, playlist);
}

Response *remove_video_playlist(unsigned int playlist_id, unsigned int video_id)
{
    Response *res = malloc(sizeof(Response));

    //Quntidade de videos na playlist
    Playlist playlist_filter = {0};
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = read_fl("Playlist", playlist_by_id, 1, &playlist_filter);

    if (playlist_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query playlist_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(playlist_query) != 1)
    {   
        dinamic_free(void *, playlist_query)
        res->code = 404;
        copy_str(res->msg, "playlist nao encontrada");
        res->data = NULL;
        return res;
    }

    Playlist *playlist = playlist_query[0];
    dinamic_free(void *, playlist_query)

    Playlist_Video pv_filter = {0};
    pv_filter.playlist_id = playlist_id;
    pv_filter.video_id = video_id;

    Playlist **pv_query = read_fl("Playlist", playlist_video_by_ids, 1, &pv_filter);

    if (pv_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query pv_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(pv_query) != 1)
    {   
        dinamic_free(void *, playlist_query)
        res->code = 404;
        copy_str(res->msg, "playlist nao encontrada");
        res->data = NULL;
        return res;
    }

    Playlist_Video *playlist_video = pv_query[0];
    dinamic_free(void *, playlist_query)    

    erase_line("Playlist_video", erase_playlist_video_by_playlist_ids, 1, playlist_video);

    update_fl("Playlist_video", update_videos_order_deleted, (playlist->videos - (1 + playlist_video->place)), playlist_video);

    update_fl("Playlist", playlist_by_id, update_decrease_vids, playlist);

    free(playlist_video);
    free(playlist_video);

    res->code = 200;
    copy_str(res->msg, "Video removido");
    res->data = NULL;

    return res;
}

Response *videos_from_playlist(unsigned int playlist_id)
{
    Response *res = malloc(sizeof(Response));

    Playlist playlist_filter = {0};
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = read_fl("Playlist", playlist_by_id, 1, &playlist_filter);

    if (playlist_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query playlist_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(playlist_query) != 1)
    {   
        dinamic_free(void *, playlist_query)
        res->code = 404;
        copy_str(res->msg, "playlist nao encontrada");
        res->data = NULL;
        return res;
    }

    Playlist *playlist = playlist_query[0];
    dinamic_free(void *, playlist_query)

    if (playlist->videos == 0)
    {
        free(playlist);
      
        res->code = 404;
        copy_str(res->msg, "Playlist vazia, ensira filmes aqui.");
        res->data = NULL;
        return res;
    }

    Playlist_Video *pv_filter = {0};
    pv_filter->playlist_id = playlist_id;

    Playlist_Video **pv_query = read_fl("Playlist_video", playlist_videos_by_playlist_id, playlist->videos, &pv_filter);
    
    if (pv_query == NULL)
    {   
        free(playlist);

        res->code = 500;
        copy_str(res->msg, "erro ao fazer query pv_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(pv_query) == 0)
    {   
        free(playlist);

        res->code = 404;
        copy_str(res->msg, "erro ao tentar achar relacao");
        res->data = NULL;
        return res;
    }

    Videos_PV_DTO **video_query = read_fl("Video", join_playlist_videos, playlist->videos, pv_query);

    if (video_query == NULL)
    {   
        free(playlist);

        free_prt_list(pv_query, dinamic_size(pv_query))
        dinamic_free(void *, pv_query);

        res->code = 500;
        copy_str(res->msg, "erro ao fazer query video_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(video_query) != playlist->videos)
    {
        free(playlist);

        free_prt_list(pv_query, dinamic_size(pv_query))
        dinamic_free(void *, pv_query);

        free_prt_list(video_query, dinamic_size(video_query))
        dinamic_free(void *,video_query);

        res->code = 500;
        copy_str(res->msg, "erro ao fazer query video_query 2");
        res->data = NULL;
        return res;
    }

    Playlis_Videos_PV_DTO *play_vid_pv_dto = malloc(sizeof(Playlis_Videos_PV_DTO));
    play_vid_pv_dto->videos = video_query;
    play_vid_pv_dto->playlist = playlist;

    res->code = 200;
    res->data = play_vid_pv_dto;
    copy_str(res->msg, "consulta pronta");

    return res;
}
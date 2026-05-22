#include "../../header.h"
#include "data.h"
#include "../../lists.h"

void filter_playlists_title(char *line, void *data_filter, void **list_save)
{
    Playlist temp, *playlist_filter = data_filter;

    PLAYLIST_SCAN(line, temp)

    trim_str(playlist_filter->name);

    if(!(compare_str(playlist_filter->name, temp.name) && playlist_filter->user_id == temp.user_id)) return;

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

    trim_str(playlist_filter->name);

    if(!(compare_titles(playlist_filter->name, temp.name, 50) && playlist_filter->user_id == temp.user_id)) return;

    Playlist *save_playlist = malloc(sizeof(Playlist));
    copy_struct(save_playlist, &temp, sizeof(Playlist));
    dina_prt_add(save_playlist, list_save);
}

typedef struct Playlist_Video_DTO
{
    Playlist playlist;
    Playlist_Video **pv;
} Playlist_Video_DTO;

void filter_playlists_title_starts_with_and_add(char *line, void *data_filter, void **list_save)
{   
    Playlist_Video_DTO *dto_filter = data_filter;
    Playlist temp, playlist_filter = dto_filter->playlist;
    Playlist_Video **blocked_playlists = dto_filter->pv;


    PLAYLIST_SCAN(line, temp)

    trim_str(playlist_filter.name);

    if(!(compare_titles(playlist_filter.name, temp.name, 50) && playlist_filter.user_id == temp.user_id)) return;

    for (int i = 0; i < dinamic_size(blocked_playlists); i++)
    {
        if(blocked_playlists[i]->playlist_id == temp.id) return;
    }

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

int update_decrease_playlists(char *line, void *data_filter)
{
    User temp, *pv_filter = data_filter;

    USER_SCAN(line, temp)

    if(!(temp.id == pv_filter->id)) return 0;

    temp.playlists--;

    USER_PRINT(line, temp);

    return 1;
}

int update_increase_playlists(char *line, void *data_filter)
{
    User temp, *pv_filter = data_filter;

    USER_SCAN(line, temp)

    if(!(temp.id == pv_filter->id)) return 0;

    temp.playlists++;

    USER_PRINT(line, temp);

    return 1;
}

void playlist_video_by_ids(char *line, void *data_filter, void **list_save)
{
    Playlist_Video temp, *pv_filter = data_filter;

    PLAYLIST_VID_SCAN(line, temp)

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

void playlist_videos_by_video_id(char *line, void *data_filter, void **list_save){
    Playlist_Video temp, *pv_filter = data_filter;

    PLAYLIST_VID_SCAN(line, temp);

    if(!(temp.video_id == pv_filter->video_id)) return;

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

    PLAYLIST_VID_PRINT(line, temp)
    return 1;
}

typedef struct Videos_PV_SEARCH_DTO
{
    Video video;
    Playlist_Video **playlist_video;
} Videos_PV_SEARCH_DTO;

void join_playlist_videos(char *line, void *data_filter, void **list_save)
{   
    Videos_PV_SEARCH_DTO *search_filter = data_filter;
    Video temp, video = search_filter->video;
    Playlist_Video **videos_filter = search_filter->playlist_video;


    VIDEO_SCAN(line, temp)

    int fund = 0;
    int place = 0;
    for (int i = 0; i < dinamic_size(videos_filter); i++){
        if(temp.id == videos_filter[i][0].video_id)
        {
            fund = 1;
            place = videos_filter[i][0].place;
            break;
        }
    }

    if(!fund) return;

    if(!(compare_titles(video.name, temp.name, 50))) return;


    Video *save_video = malloc(sizeof(Video));
    copy_struct(save_video, &temp, sizeof(Video));

    Videos_PV_DTO *save_dto = malloc(sizeof(Videos_PV_DTO));
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

    Playlist **same_title_query = (Playlist **)read_fl("Playlist", filter_playlists_title, 1, &playlist_filter);

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
        snprintf(res->msg, 50, "\n\n\tvoce ja possui a pl %-50s", title);
        res->data = NULL;
        return res;
    }

    dinamic_free(void *, same_title_query)

    append_line("Playlist", 1, PLAYLIST_PRINT_MASK, 0, user_id, 0, title, description);

    User user_filter;
    user_filter.id = user_id;

    update_fl("User", update_increase_playlists, 1, &user_filter);

    res->code = 200;
    res->data = NULL;
    snprintf(res->msg, 50, "pl %20s criada", title);

    return res;
}

Response *delete_playlist(unsigned int playlist_id, unsigned int user_id)
{
    Response *res = malloc(sizeof(Response));

    Playlist playlist_filter;
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = (Playlist **)read_fl("Playlist", playlist_by_id, 1, &playlist_filter);

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

    if (playlist->user_id != user_id)
    {   
        free(playlist);
        dinamic_free(void *, playlist_query)
        res->code = 404;
        copy_str(res->msg, "Esta playlist nao te pertence");
        res->data = NULL;
        return res;
    }

    Playlist_Video playlist_video_filter;
    playlist_video_filter.playlist_id = playlist_id;

    if (playlist->videos > 0) erase_line("Playlist_video", erase_playlist_video_by_playlist_id, playlist->videos, &playlist_video_filter);

    playlist_filter.id = playlist_id;

    erase_line("Playlist", erase_playlist_by_id, 1, &playlist_filter);

    User user_filter;
    user_filter.id = user_id;

    update_fl("User", update_decrease_playlists, 1, &user_filter);

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

    Playlist **playlists = (Playlist **)read_fl("Playlist", filter_playlists_title_starts_with, 0, &playlist_filter);

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
        copy_str(res->msg, "\n\tVoce nao possui playlists, crie alguma");
        res->data = NULL;
        return res;
    }

    res->code = 200;
    res->data = playlists;
    copy_str(res->msg, "playlists achadas");

    return res;
}

Response *search_for_playlists_to_add(unsigned int user_id, unsigned int video_id, char * title)
{
    Response *res = malloc(sizeof(Response));

    Playlist_Video pv_filter = {0};
    pv_filter.video_id = video_id;

    Playlist_Video **pv_query = (Playlist_Video **)read_fl("Playlist_video", playlist_videos_by_video_id, 0, &pv_filter);

    if (pv_query == NULL)
    {   
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query pv_query");
        res->data = NULL;
        return res;
    }

    Playlist_Video_DTO playlist_filter;
    playlist_filter.playlist.user_id = user_id;
    copy_str(playlist_filter.playlist.name, title);
    playlist_filter.pv = pv_query;

    Playlist **playlists = (Playlist **)read_fl("Playlist", filter_playlists_title_starts_with_and_add, 0, &playlist_filter);

    free_prt_list(pv_query, dinamic_size(pv_query))
    dinamic_free(void *, pv_query)

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
        copy_str(res->msg, "\n\tVoce nao possui playlists, crie alguma");
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

    Playlist **playlist_query = (Playlist **)read_fl("Playlist", playlist_by_id, 1, &playlist_filter);
     
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
    dinamic_free(void *, playlist_query)
    ////////////
    //

    //verificar duplicidade
    ////////////
    if(playlist->videos > 0){
        Playlist_Video playlist_video_filter = {0};
        playlist_video_filter.playlist_id = playlist_id;
        playlist_video_filter.video_id = video_id;

        Playlist_Video **video_already_in_query = (Playlist_Video **)read_fl("Playlist_video", playlist_video_by_ids, 1, &playlist_video_filter);

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

        dinamic_free(void *, video_already_in_query)
    }    
    ////////////
    //


    append_line("Playlist_video", 0, PLAYLIST_VID_PRINT_MASK, playlist_id, video_id, playlist->videos);
    
    update_fl("Playlist", update_increase_vids, 1, playlist);

    free(playlist);

    res->code = 200;
    copy_str(res->msg, "video adicionado");
    res->data = NULL;
    
    return res;
}

Response *remove_video_playlist(unsigned int playlist_id, unsigned int video_id)
{
    Response *res = malloc(sizeof(Response));

    //Quntidade de videos na playlist
    Playlist playlist_filter = {0};
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = (Playlist **)read_fl("Playlist", playlist_by_id, 1, &playlist_filter);

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

    Playlist_Video **pv_query = (Playlist_Video **)read_fl("Playlist_video", playlist_video_by_ids, 1, &pv_filter);

    if (pv_query == NULL)
    {   
        free(playlist);
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query pv_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(pv_query) != 1)
    {   
        free(playlist);
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

    update_fl("Playlist", update_decrease_vids, 1, playlist);

    free(playlist_video);
    free(playlist_video);

    res->code = 200;
    copy_str(res->msg, "Video removido");
    res->data = NULL;

    return res;
}

Response *videos_from_playlist(unsigned int playlist_id, char *title)
{
    Response *res = malloc(sizeof(Response));

    Playlist playlist_filter = {0};
    playlist_filter.id = playlist_id;

    Playlist **playlist_query = (Playlist **)read_fl("Playlist", playlist_by_id, 1, &playlist_filter);

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

    Playlis_Videos_PV_DTO *play_vid_pv_dto = malloc(sizeof(Playlis_Videos_PV_DTO));
    play_vid_pv_dto->playlist = playlist;
    play_vid_pv_dto->videos = NULL;

    if (playlist->videos == 0)
    {      
        res->code = 206;
        copy_str(res->msg, "\n\n\tPlaylist vazia, ensira filmes aqui.");
        res->data = play_vid_pv_dto;
        return res;
    }

    Playlist_Video pv_filter = {0};
    pv_filter.playlist_id = playlist_id;

    Playlist_Video **pv_query = (Playlist_Video **)read_fl("Playlist_video", playlist_videos_by_playlist_id, playlist->videos, &pv_filter);
    
    if (pv_query == NULL)
    {   
        free(play_vid_pv_dto);
        free(playlist);
        res->code = 500;
        copy_str(res->msg, "erro ao fazer query pv_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(pv_query) == 0)
    {   
        dinamic_free(void *, pv_query)
        res->code = 206;
        copy_str(res->msg, "\n\n\tPlaylist vazia, ensira filmes aqui. 2");
        res->data = play_vid_pv_dto;
        return res;
    }

    Videos_PV_SEARCH_DTO search_filter = {0};
    copy_str(search_filter.video.name, title);
    search_filter.playlist_video = pv_query;

    Videos_PV_DTO **video_query = (Videos_PV_DTO **) read_fl("Video", join_playlist_videos, playlist->videos, &search_filter);

    if (video_query == NULL)
    {   
        free(playlist);
        free(play_vid_pv_dto);
        free_prt_list(pv_query, dinamic_size(pv_query))
        dinamic_free(void *, pv_query);

        res->code = 500;
        copy_str(res->msg, "erro ao fazer query video_query");
        res->data = NULL;
        return res;
    }

    if (dinamic_size(video_query) == 0)
    {
        free_prt_list(video_query, dinamic_size(video_query))
        dinamic_free(void *,video_query);

        res->code = 206;
        copy_str(res->msg, "\n\tPlaylist vazia, insira ou melhore a procura.");
        res->data = play_vid_pv_dto;
        return res;
    }

    if (dinamic_size(video_query) > playlist->videos)
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


    play_vid_pv_dto->videos = video_query;

    res->code = 200;
    res->data = play_vid_pv_dto;
    copy_str(res->msg, "consulta pronta");

    return res;
}
#include "data.h"
#include "../../lists.h"

int video_increase_like(char *line, void *data_filter)
{
    Video original;
    Video *new = (Video *)data_filter;

    VIDEO_SCAN(line, original)

    if(original.id != new->id) return 0;

    sprintf(line, VIDEO_PRINT_MASK, original.id, original.name, original.desc, original.duration,  original.likes + 1,  original.dislikes);

    return 1;
}

int video_decrease_like(char *line, void *data_filter)
{
    Video original;
    Video *new = (Video *)data_filter;

    VIDEO_SCAN(line, original)

    if(original.id != new->id) return 0;

    sprintf(line, VIDEO_PRINT_MASK, original.id, original.name, original.desc, original.duration,  original.likes - 1,  original.dislikes);

    return 1;
}

int video_increase_dislike(char *line, void *data_filter)
{
    Video original;
    Video *new = (Video *)data_filter;

    VIDEO_SCAN(line, original)

    if(original.id != new->id) return 0;

    sprintf(line, VIDEO_PRINT_MASK, original.id, original.name, original.desc, original.duration,  original.likes,  original.dislikes + 1);

    return 1;
}

int video_decrease_dislike(char *line, void *data_filter)
{
    Video original;
    Video *new = (Video *)data_filter;

    VIDEO_SCAN(line, original)

    if(original.id != new->id) return 0;

    sprintf(line, VIDEO_PRINT_MASK, original.id, original.name, original.desc, original.duration,  original.likes,  original.dislikes - 1);

    return 1;
}

void user_liked(char *line, void *data_filter, void **list_save)
{
    Like temp;
    Like *like_filter = (Like *)data_filter;

    LIKE_SCAN(line, temp)

    if (temp.user_id != like_filter->user_id || temp.video_id != like_filter->video_id) return;
    
    Like *save_like = malloc(sizeof(Like));
    copy_struct(save_like, &temp, sizeof(Like));
    dina_prt_add(save_like, list_save);
    
}

int verify_like(char * line, void *data)
{   
    Like *like = (Like *) data;
    int user_id, vid_id;
    sscanf(line, LIKE_SCAN_MASK, &user_id, &vid_id);

    return (like->user_id == user_id && like->video_id == vid_id);
}

Response *handle_like(unsigned int user_id, unsigned int video_id)
{   
    Like like_filter;
    like_filter.user_id = user_id;
    like_filter.video_id = video_id;

    Video video_filter;
    video_filter.id = video_id;

    Like **liked_query = (Like **) read_fl("Like", user_liked, 1, &like_filter);
    Like *liked = NULL;
    if(liked_query != NULL)
    {
        liked = (dinamic_size(liked_query) > 0)? liked_query[0]: NULL;
        dinamic_free(void *, liked_query)
    }

    Dislike ** disliked_query = (liked == NULL) ? (Dislike **) read_fl("Dislike", user_liked, 1, &like_filter) : NULL;
    Dislike *disliked = NULL;
    if(disliked_query != NULL)
    {
        disliked = (dinamic_size(disliked_query) > 0)? disliked_query[0]: NULL;
        dinamic_free(void *, disliked_query)
    }


    // Response *res = malloc(sizeof(Response));

    if (liked != NULL)
    {   
        erase_line("Like", verify_like, 1, liked);
        update_fl("Video", video_decrease_like, 1, &video_filter);
        free(liked);
        return NULL;
    }

    if (disliked != NULL)
    {   
        erase_line("Dislike", verify_like, 1, disliked);
        update_fl("Video", video_decrease_dislike, 1, &video_filter);
        free(disliked);
    }

    append_line("Like", 0, LIKE_PRINT_MASK, user_id, video_id);
    update_fl("Video", video_increase_like, 1, &video_filter);

    free(liked);
    free(disliked);

    return NULL;
}

Response *handle_dislike(unsigned int user_id, unsigned int video_id)
{   
    Like like_filter;
    like_filter.user_id = user_id;
    like_filter.video_id = video_id;

    Video video_filter;
    video_filter.id = video_id;

    Like **liked_query = (Like **) read_fl("Like", user_liked, 1, &like_filter);
    Like *liked = NULL;
    if(liked_query != NULL)
    {
        liked = (dinamic_size(liked_query) > 0)? liked_query[0]: NULL;
        dinamic_free(void *, liked_query)
    }

    Dislike ** disliked_query = (liked == NULL) ? (Dislike **) read_fl("Dislike", user_liked, 1, &like_filter) : NULL;
    Dislike *disliked = NULL;
    if(disliked_query != NULL)
    {
        disliked = (dinamic_size(disliked_query) > 0)? disliked_query[0]: NULL;
        dinamic_free(void *, disliked_query)
    }

    if (disliked != NULL)
    {   
        erase_line("Dislike", verify_like, 1, disliked);
        update_fl("Video", video_decrease_dislike, 1, &video_filter);
        free(disliked);
        
        return NULL;
    }

    if (liked != NULL)
    {   

        erase_line("Like", verify_like, 1, liked);
        update_fl("Video", video_decrease_like, 1, &video_filter);
        free(liked);
    }

    append_line("Dislike", 0, LIKE_PRINT_MASK, user_id, video_id);
    update_fl("Video", video_increase_dislike, 1, &video_filter);

    free(liked);
    free(disliked);

    return NULL;
}

void vid_by_id(char *line, void *data_filter, void **list_save)
{
    Video temp;
    Video *video_filter = data_filter;

    sscanf(line, VIDEO_SCAN_MASK, &temp.id, temp.name, temp.desc, &temp.duration, &temp.likes, &temp.dislikes);

    if(temp.id != video_filter->id) return;

    Video *save = malloc(sizeof(Video));
    copy_struct(save, &temp, sizeof(Video));

    dina_prt_add(save, list_save);
}

Response *video_user_search(unsigned int user_id, unsigned int video_id)
{
    Response *res = malloc(sizeof(Response));
    res->msg[0] = '\0';

    Video video_filter;
    video_filter.id = video_id;

    Like like_filter;
    like_filter.user_id = user_id;
    like_filter.video_id = video_id;

    Video **vid = (Video **) read_fl("Video", vid_by_id, 1, &video_filter);

    if (dinamic_size(vid) == 0)
    {   
        dinamic_free(void *, vid);
        res->code = 404;
        res->data = NULL;
        sprintf(res->msg, "Nao foi possivel localizar o video id: %d", video_id);
        return res;
    }

    Like **liked = (Like **) read_fl("Like", user_liked, 1, &like_filter);
    Dislike ** disliked = (dinamic_size(liked) == 0) ? (Dislike **) read_fl("Dislike", user_liked, 1, &like_filter) : NULL;

    Video_user *video_user = (Video_user *)malloc(sizeof(Video_user));
    video_user->video.id = vid[0]->id;
    video_user->video.likes = vid[0]->likes;
    video_user->video.duration = vid[0]->duration;
    video_user->video.dislikes = vid[0]->dislikes;
    copy_str(video_user->video.name, vid[0]->name);
    copy_str(video_user->video.desc, vid[0]->desc);

    free(vid[0]);
    dinamic_free(void *, vid);

    video_user->like = 0;
    video_user->dislike = 0;

    if(liked != NULL)
    {
        video_user->like = dinamic_size(liked) > 0;
        if (dinamic_size(liked) > 0) free(liked[0]);
    }

    if(disliked != NULL)
    {
        video_user->dislike = dinamic_size(disliked) > 0;
        if (dinamic_size(disliked) > 0) free(disliked[0]);
    }
    
    dinamic_free(void *, liked);

    dinamic_free(void *, disliked);

    free(vid[0]);
   
    res->code = 200;
    res->data = video_user;
    return res;
}

void search_vids(char *line, void *data_filter, void **list_save)
{   
    Video temp, *video_filter = data_filter;
    
    VIDEO_SCAN(line, temp)

    if (!compare_titles(video_filter->name, temp.name, sizeof(temp.name))) return;

    Video *save = malloc(sizeof(Video));
    copy_struct(save, &temp, sizeof(Video));
    dina_prt_add(save, list_save);
}

Response *search_for_videos(char *title)
{
    Response *res = malloc(sizeof(Response));

    Video video_filter;
    copy_str(video_filter.name, title);

    Video **vids = (Video **) read_fl("Video", search_vids, 0, &video_filter);

    if (dinamic_size(vids) == 0)
    {
        res->code = 400;
        res->data = NULL;
        sprintf(res->msg, "Nenhum resultado: %s", title);
        dinamic_free(void *, vids) 
        return res;
    }
    res->code = 200;
    res->data = vids;

    return res;
}
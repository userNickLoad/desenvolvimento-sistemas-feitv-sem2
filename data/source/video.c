#include "data.h"
#include "../../lists.h"

Video *apend_video(Video *vid)
{
    db_file(Video, "r+")

    vid->id = Video_amount + 1;
    vid->likes = 0;
    vid->dislikes = 0;

    fseek(fl_Video, 0, SEEK_END);

    fprintf(fl_Video, "%010u;", vid->id);
    char name_b[51];
    ajust_info(name_b, vid->name, sizeof(name_b), ';');
    fwrite(name_b, sizeof(char), 51, fl_Video);

    char dec_b[251];
    ajust_info(dec_b, vid->desc, sizeof(dec_b), ';');
    fwrite(dec_b, sizeof(char), 251, fl_Video);

    fprintf(fl_Video, "%010u;%010u;%010u\n", vid->duration, 0, 0);

    fseek(fl_Video, 0, SEEK_SET);
    fprintf(fl_Video, HEADER_MASK, (Video_amount + 30), Video_line_size);

    fclose(fl_Video);
    return vid;
}

int compare_titles(char *title1, char *title2, int size)
{   
    if(title1[0] == '\0') return 1;
    for (int i = 0; i < size; i++)
    {
        if(title1[i] != title2[i]) return 0;
        if(title1[i+1] == '\0') return 1;
    }
    return 0;
}

void *erase_like(unsigned int user_id, unsigned int video_id)
{
    db_file(Like, "r+")
    
    for(int i = 0; i < Like_amount; i++)
    {   
        Like like;
        fscanf(fl_Like, LIKE_SCAN_MASK, &like.user_id, &like.video_id);
        
        if(like.user_id == user_id && like.video_id == video_id){
            for (int j = i+1; j < Like_amount; j++)
            {   
                fscanf(fl_Like, LIKE_SCAN_MASK, &like.user_id, &like.video_id);
                fseek(fl_Like, (-Like_line_size), SEEK_CUR);
                fprintf(fl_Like, LIKE_PRINT_MASK, like.user_id, like.video_id);
            }
            break;
        }
    }

    fseek(fl_Like, 0, SEEK_SET);
    fprintf(fl_Like, LIKE_PRINT_MASK, Like_amount-1, Like_line_size);

    return NULL;
}

void *erase_dislike(unsigned int user_id, unsigned int video_id)
{
    db_file(Dislike, "r+")
    
    for(int i = 0; i < Dislike_amount; i++)
    {   
        Dislike Dislike;
        fscanf(fl_Dislike, LIKE_SCAN_MASK, &Dislike.user_id, &Dislike.video_id);
        
        if(Dislike.user_id == user_id && Dislike.video_id == video_id){
            for (int j = i+1; j < Dislike_amount; j++)
            {   
                fscanf(fl_Dislike, LIKE_SCAN_MASK, &Dislike.user_id, &Dislike.video_id);
                fseek(fl_Dislike, (-Dislike_line_size), SEEK_CUR);
                fprintf(fl_Dislike, LIKE_PRINT_MASK, Dislike.user_id, Dislike.video_id);
            }
            break;
        }
    }

    fseek(fl_Dislike, 0, SEEK_SET);
    fprintf(fl_Dislike, LIKE_PRINT_MASK, Dislike_amount-1, Dislike_line_size);

    return NULL;
}

void *append_like(unsigned int user_id, unsigned int video_id)
{
    db_file(Like, "r+")
    fseek(fl_Like, 0, SEEK_END);

    fprintf(fl_Like, LIKE_PRINT_MASK, user_id, video_id);

    fseek(fl_Like, 0, SEEK_SET);
    fprintf(fl_Like, LIKE_PRINT_MASK, Like_amount+1, Like_line_size);

    fclose(fl_Like);

    return NULL;
}

void *append_dislike(unsigned int user_id, unsigned int video_id)
{
    db_file(Dislike, "r+")
    fseek(fl_Dislike, 0, SEEK_END);

    fprintf(fl_Dislike, LIKE_PRINT_MASK, user_id, video_id);

    fseek(fl_Dislike, 0, SEEK_SET);
    fprintf(fl_Dislike, LIKE_PRINT_MASK, Dislike_amount+1, Dislike_line_size);

    fclose(fl_Dislike);

    return NULL;
}

void *video_increase_like(unsigned int video_id)
{
    db_file(Video, "r+")
    fseek(fl_Video, 0, SEEK_END);

    for(int i = 0; i < Video_amount; i++)
    {   
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if(vid.id == video_id){
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fprintf(fl_Video, VIDEO_PRINT_MASK, vid.id, vid.name, vid.desc, vid.duration, vid.likes + 1, vid.dislikes);
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}

void *video_decrease_like(unsigned int video_id)
{
    db_file(Video, "r+")
    fseek(fl_Video, 0, SEEK_END);

    for(int i = 0; i < Video_amount; i++)
    {   
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if(vid.id == video_id){
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fprintf(fl_Video, VIDEO_PRINT_MASK, vid.id, vid.name, vid.desc, vid.duration, vid.likes - 1, vid.dislikes);
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}

void *video_increase_dislike(unsigned int video_id)
{
    db_file(Video, "r+")
    fseek(fl_Video, 0, SEEK_END);

    for(int i = 0; i < Video_amount; i++)
    {   
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if(vid.id == video_id){
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fprintf(fl_Video, VIDEO_PRINT_MASK, vid.id, vid.name, vid.desc, vid.duration, vid.likes, vid.dislikes + 1);
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}

void *video_decrease_dislike(unsigned int video_id)
{
    db_file(Video, "r+")
    fseek(fl_Video, 0, SEEK_END);

    for(int i = 0; i < Video_amount; i++)
    {   
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if(vid.id == video_id){
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fprintf(fl_Video, VIDEO_PRINT_MASK, vid.id, vid.name, vid.desc, vid.duration, vid.likes, vid.dislikes - 1);
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}


Like *user_liked(unsigned int user_id, unsigned int video_id)
{
    db_file(Like, "r")
    Like *liked = NULL;

    for(int i = 0; i < Like_amount; i++)
    {   
        Like like;
        fscanf(fl_Like, LIKE_PRINT_MASK, &like.user_id, &like.video_id);
        
        if(like.user_id == user_id && like.video_id == video_id){
            liked = malloc(sizeof(Like));
            copy_struct(liked, &like, sizeof(Like));
            break;
        }
    }

    return liked;
}

Dislike *user_disliked(unsigned int user_id, unsigned int video_id)
{
    db_file(Dislike, "r")
    Dislike * disliked = NULL;

    for(int i = 0; i < Dislike_amount; i++)
    {   
        Dislike dislike;
        fscanf(fl_Dislike, LIKE_PRINT_MASK, &dislike.user_id, &dislike.video_id);
        
        if(dislike.user_id == user_id && dislike.video_id == video_id){
            disliked = malloc(sizeof(Dislike));
            copy_struct(disliked, &dislike, sizeof(Dislike));
            break;
        }
    }

    return disliked;
}

Response *handle_like(unsigned int user_id, unsigned int video_id)
{
    Like *liked = user_liked(user_id, video_id);
    Dislike *disliked = (liked != NULL)? user_disliked(user_id, video_id): NULL;

    // Response *res = malloc(sizeof(Response));

    if (liked != NULL)
    {   
        erase_like(user_id, video_id);
        video_decrease_like(video_id);
        return NULL;
    }

    if(disliked != NULL)
    {
        erase_dislike(user_id, video_id);
        video_decrease_dislike(video_id);
    }

    append_like(user_id, video_id);
    video_increase_like(video_id);
    return NULL;

}

Response *handle_dislike(unsigned int user_id, unsigned int video_id)
{
    Like *liked = user_liked(user_id, video_id);
    Dislike *disliked = (liked != NULL)? user_disliked(user_id, video_id): NULL;

    // Response *res = malloc(sizeof(Response));

    if(disliked != NULL)
    {
        erase_dislike(user_id, video_id);
        video_decrease_dislike(video_id);
        return NULL;
    }

    if (liked != NULL)
    {   
        erase_like(user_id, video_id);
        video_decrease_like(video_id);
    }

    append_like(user_id, video_id);
    video_increase_like(video_id);
    return NULL;
}

Video *vid_by_id(unsigned int video_id)
{
    db_file(Video, "r")

    Video * vid_selec = NULL;

    for(int i = 0; i < Video_amount; i++)
    {   
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if(vid.id == video_id){
            vid_selec = (Video *)malloc(sizeof(Video));
            copy_struct(vid_selec, &vid, sizeof(Video));
            break;
        }
    }
    fclose(fl_Video);

    return vid_selec;
}

Response *video_user_search(unsigned int user_id, unsigned int video_id)
{
    Response * res = malloc(sizeof(Response));

    Video *vid = vid_by_id(video_id);

    if(vid == NULL)
    {
        res->code = 404;
        res->data = NULL;
        sprintf(res->msg, "Nao foi possivel localizar o video id: %d", video_id);
        return res;
    }

    Like *liked = user_liked(user_id, video_id);
    Dislike *dislike = (liked == NULL)? user_disliked(user_id, video_id): NULL;

    res->msg[0] = '\0';

    Video_user * video_user = (Video_user *)malloc(sizeof(Video_user));
    copy_struct(&video_user->video, vid, sizeof(Video));
    free(vid);

    video_user->like = liked != NULL;
    video_user->dislike = dislike != NULL;

    res->code = 200;
    res->data = video_user;
    return res;
}

Video *search_vids(char *title)
{
    db_file(Video, "r")

    Video * vids = (Video *)dina_prt_init(Video_amount);

    for(int i = 0; i < Video_amount; i++)
    {   
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if(compare_titles(title, vid.name, sizeof(vid.name))){
            Video * vid_selec = malloc(sizeof(Video));
            copy_struct(vid_selec, &vid, sizeof(vid));
            dina_prt_add((void *)vid_selec, (void *)vids);
        }
    }
    fclose(fl_Video);

    return vids;
}

Response search_for_videos(char *title)
{   
    Response res;

    Video *vids = search_vids(title);

    if (dinamic_size(vids) < 0)
    {
        res.code = 400;
        res.data = NULL;
        printf(res.msg, "Nenhum resultado: %s", title);
        dinamic_free(void *, vids)
        return res;
    }
    res.code = 200;
    res.data = vids;

    return res;

}
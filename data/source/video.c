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

void *video_increase_like(unsigned int video_id)
{
    db_file(Video, "r+")
        fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < Video_amount; i++)
    {
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if (vid.id == video_id)
        {
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fseek(fl_Video, 324, SEEK_CUR);
            fprintf(fl_Video, "%010u", (vid.likes + 1));
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}

void *video_decrease_like(unsigned int video_id)
{
    db_file(Video, "r+")
        fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < Video_amount; i++)
    {
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if (vid.id == video_id)
        {
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fseek(fl_Video, 324, SEEK_CUR);
            fprintf(fl_Video, "%010u", (vid.likes - 1));
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}

void *video_increase_dislike(unsigned int video_id)
{
    db_file(Video, "r+")
        fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < Video_amount; i++)
    {
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if (vid.id == video_id)
        {
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fseek(fl_Video, 335, SEEK_CUR);
            fprintf(fl_Video, "%010u", (vid.dislikes + 1));
            break;
        }
    }

    fclose(fl_Video);

    return NULL;
}

void *video_decrease_dislike(unsigned int video_id)
{
    db_file(Video, "r+")
        fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for (int i = 0; i < Video_amount; i++)
    {
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if (vid.id == video_id)
        {
            fseek(fl_Video, HEARDER_SIZE + (i * Video_line_size), SEEK_SET);
            fseek(fl_Video, 335, SEEK_CUR);
            fprintf(fl_Video, "%010u", (vid.dislikes - 1));
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

    for (int i = 0; i < Like_amount; i++)
    {
        Like like;
        fscanf(fl_Like, LIKE_PRINT_MASK, &like.user_id, &like.video_id);

        if (like.user_id == user_id && like.video_id == video_id)
        {
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
        Dislike *disliked = NULL;

    for (int i = 0; i < Dislike_amount; i++)
    {
        Dislike dislike;
        fscanf(fl_Dislike, LIKE_PRINT_MASK, &dislike.user_id, &dislike.video_id);

        if (dislike.user_id == user_id && dislike.video_id == video_id)
        {
            disliked = malloc(sizeof(Dislike));
            copy_struct(disliked, &dislike, sizeof(Dislike));
            break;
        }
    }

    return disliked;
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
    Like *liked = user_liked(user_id, video_id);
    Dislike *disliked = (liked == NULL) ? user_disliked(user_id, video_id) : NULL;

    // Response *res = malloc(sizeof(Response));

    if (liked != NULL)
    {
        erase_line("Like", verify_like, liked);
        video_decrease_like(video_id);
        return NULL;
    }

    if (disliked != NULL)
    {
        erase_line("Dislike", verify_like, disliked);
        video_decrease_dislike(video_id);
    }

    append_line("Like", 0, LIKE_PRINT_MASK, user_id, video_id);
    video_increase_like(video_id);

    free(liked);
    free(disliked);

    return NULL;
}

Response *handle_dislike(unsigned int user_id, unsigned int video_id)
{   
    Dislike *disliked =  user_disliked(user_id, video_id);
    Like *liked = (disliked == NULL) ? user_liked(user_id, video_id): NULL;

    // Response *res = malloc(sizeof(Response));

    if (disliked != NULL)
    {
        erase_line("Dislike", verify_like, disliked);
        video_decrease_dislike(video_id);
        return NULL;
    }

    if (liked != NULL)
    {
        erase_line("Like", verify_like, liked);
        video_decrease_like(video_id);
    }

    append_line("Dislike", 0, LIKE_PRINT_MASK, user_id, video_id);
    video_increase_dislike(video_id);

    free(liked);
    free(disliked);

    return NULL;
}

Video *vid_by_id(unsigned int video_id)
{
    db_file(Video, "r")

        Video *vid_selec = NULL;

    for (int i = 0; i < Video_amount; i++)
    {
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if (vid.id == video_id)
        {
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
    Response *res = malloc(sizeof(Response));

    Video *vid = vid_by_id(video_id);

    if (vid == NULL)
    {
        res->code = 404;
        res->data = NULL;
        sprintf(res->msg, "Nao foi possivel localizar o video id: %d", video_id);
        return res;
    }

    Like *liked = user_liked(user_id, video_id);
    Dislike *dislike = (liked == NULL) ? user_disliked(user_id, video_id) : NULL;

    res->msg[0] = '\0';

    Video_user *video_user = (Video_user *)malloc(sizeof(Video_user));
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

        Video *vids = (Video *)dina_prt_init(Video_amount);

    for (int i = 0; i < Video_amount; i++)
    {
        Video vid;
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vid.id, vid.name, vid.desc, &vid.duration, &vid.likes, &vid.dislikes);

        if (compare_titles(title, vid.name, sizeof(vid.name)))
        {
            Video *vid_selec = malloc(sizeof(Video));
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
        dinamic_free(void *, vids) return res;
    }
    res.code = 200;
    res.data = vids;

    return res;
}
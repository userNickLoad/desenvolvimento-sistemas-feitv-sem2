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


Video *get_vids()
{
    db_file(Video, "r")

    Video * vids = malloc(sizeof(Video) * Video_amount);

    for(int i = 0; i < Video_amount; i++)
    {   
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vids[i].id, vids[i].name, vids[i].desc, &vids[i].duration, &vids[i].likes, &vids[i].dislikes);
    }
    fclose(fl_Video);
    
    return vids;
}

int compare_titles(char *title1, char *title2, int size)
{
    for (int i = 0; i < size; i++)
    {
        if(title1[i] != title2[i]) return 0;
        if(title1 == '\0') return 1;
    }
    return 0;
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
    if (title == NULL)
    {
        res.code = 200;
        res.data = get_vids();
        return res;
    }

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
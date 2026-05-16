#include "data.h"

Video* apend_video_idx_name(Video *vid){
    db_file(Video, "r+")
}

Video *apend_video(Video *vid)
{   
    db_file(Video, "r+")

    vid->id = Video_amount+1;
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


int main()
{   
    db_file(Video, "r")
    db_file(Video_idx_name, "r+")

    Video *vids = malloc(sizeof(vids)*Video_amount);

    fseek(fl_Video, HEARDER_SIZE, SEEK_SET);

    for(int i = 0; i < Video_amount; i++)
    {
        fscanf(fl_Video, VIDEO_SCAN_MASK, &vids[i].id, vids[i].name, vids[i].desc, &vids[i].duration, &vids[i].likes, &vids[i].dislikes);
    }

    //0000000000;                                                  ;0000000000;0000000000;0000000000

    

    fclose(fl_Video);
    fclose(fl_Video_idx_name);
    return 0;
}

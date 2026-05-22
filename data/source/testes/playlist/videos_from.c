#include "../../../../header.h"
#include "../../data.h"
#include "../../../../schema.h"
#include "../../../../lists.h"

int main(void)
{

    Response *res = videos_from_playlist(6);
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    Playlis_Videos_PV_DTO *res_ = res->data;
    Playlist *pl = res_->playlist;
    printf("Playlist %0.50s\n", pl->name);

    Videos_PV_DTO ** vids = res_->videos;
    for(int i = 0; i < pl->videos; i++)
    {   
        
        printf("Video %0.50s\n", vids[i]->videos->name);
        printf("place: %u\n", vids[i]->place);
    }

    getchar();

}
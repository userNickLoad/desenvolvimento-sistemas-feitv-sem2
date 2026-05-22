#include "../../../../header.h"
#include "../../data.h"
#include "../../../../schema.h"
#include "../../../../lists.h"

int main(void)
{

    Response *res = add_video_playlist(6, 7);
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    
    res = add_video_playlist(6, 9);
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);


    getchar();

}
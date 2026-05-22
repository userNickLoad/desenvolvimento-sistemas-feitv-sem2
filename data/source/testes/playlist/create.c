#include "../../../../header.h"
#include "../../data.h"
#include "../../../../schema.h"

int main(void)
{
    Response *res = create_playlist(0, "videos animais", "muito ilegais");
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    res = create_playlist(0, "videos joia", "muito ilegais");
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    res = create_playlist(0, "abacate", "muito ilegais");
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

    res = create_playlist(0, "bobrinha", "muito ilegais");
    printf("res->code: %u\n", res->code);
    printf("res->msg %s\n", res->msg);
    printf("res->data: %p\n", res->data);

}
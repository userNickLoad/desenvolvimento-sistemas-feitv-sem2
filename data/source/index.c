#include "data.h"

int create_index(char *target_col_f, int target_init, char *path){
    db_file(path, "r");
    
    fseek(fl_path, target_init, SEEK_CUR);

    for (int i = 0; i < path_amount; i++)
    {
        fscanf(fl_path, table_f, )
    }
}

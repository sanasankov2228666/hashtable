#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_dump.h"
#include "../str_funcs/str_func.h"
#include "rewrite_aling_file.h"

int main (void)
{
    init_crc32_table ();

    hash_func funcs  [7] = 
    
    {   hash_func_1,    hash_func_2,    hash_func_3,
        hash_func_4,    hash_func_5,    hash_func_6 ,
        hash_func_crc32                              };

    HashTable tables [7] = {};

    for (int i = 0; i < 7; i++)
    {
        tables [i] = hash_table_constructor (funcs [i]);
    }

    // =============================================================

    FILE* fp_add = fopen ("textfile.txt", "r");
    if (!fp_add) return 1;
    
    for (int i = 0; i < 7; i++)
    {
        hash_table_add_file (&tables [i], fp_add);
    }

    // =============== ДАМПЫ ===================
 
    for (int i = 0; i < 7; i++)
    {
        char str [30] = {};
        snprintf (str, 30, "hash_table_%d", i + 1);

        hashtable_dump (&tables [i], str);
    }

    // =========== УДАЛЕНИЕ ТАБЛИЦ =============

    for (int i = 0; i < 7; i++)
    {
        hash_table_deleter (&tables [i]);
    }

    fclose (fp_add);

    return 0;
}
#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_dump.h"
#include "../str_funcs/str_func.h"
#include "rewrite_aling_file.h"

#define HASH_FUNC(func, size) {#func "_" #size, func, size}

struct hash_func_info
{
    const char* name;
    const hash_func func_ptr;
    size_t size_table;
}

funcs [] =
{
    HASH_FUNC(hash_func_1or0,     4001),
    HASH_FUNC(hash_func_frst_ltr, 4001),
    HASH_FUNC(hash_func_strlen,   4001),
    HASH_FUNC(hash_func_asci_sum, 4001),
    HASH_FUNC(hash_func_asci_sum, 517 ),
    HASH_FUNC(hash_func_rol,      4001),
    HASH_FUNC(hash_func_ror,      4001),
    HASH_FUNC(hash_func_crc32,    4001)
};

int main (void)
{
    init_crc32_table ();

    HashTable tables [8] = {};

    size_t len = sizeof (funcs) / sizeof (hash_func_info);

    for (int i = 0; i < len; i++)
    {
        tables [i] = hash_table_constructor (funcs[i].func_ptr, funcs[i].size_table);
    }

    // =============================================================

    FILE* fp_add = fopen ("textfile.txt", "r");
    if (!fp_add) return 1;
    
    for (int i = 0; i < len; i++)
    {
        hash_table_add_file (&tables [i], fp_add);
    }

    // =============== ДАМПЫ ===================
 
    for (int i = 0; i < len; i++)
    {
        char str [30] = {};
        snprintf (str, 30, "table_%s", funcs[i].name);

        hashtable_dump (&tables [i], str);
    }

    // =========== УДАЛЕНИЕ ТАБЛИЦ =============

    for (int i = 0; i < len; i++)
    {
        hash_table_deleter (&tables [i]);
    }

    fclose (fp_add);

    return 0;
}
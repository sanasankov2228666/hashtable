#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_dump.h"
#include "../str_funcs/str_func.h"
#include "rewrite_aling_file.h"

int main (void)
{

    #ifdef CRC_32_OPT
        HashTable hashtable_crc32 = hash_table_constructor (my_crc32, 4001);
    #else
        init_crc32_table ();
        HashTable hashtable_crc32 = hash_table_constructor (hash_func_crc32, 4001);
    #endif

    // =============================================================

    FILE* fp_add = fopen ("textfile.txt", "r");
    if (!fp_add) return 1;

    FILE* fp_search = fopen ("test.txt", "r");
    if (!fp_search) return 1;
    
    hash_table_add_file (&hashtable_crc32, fp_add);

    rewrite_to_algn_bin (fp_search);

    FILE* fp_bin = fopen ("test.bin", "rb");
    if (!fp_bin) return 1;
    
    char* test_buf = NULL;
    size_t size = file_read (fp_bin, &test_buf);

    // =============== ТЕСТЫ ===================

    for (int i = 0; i < 500; i++)
    {
        char* begin_word = test_buf;
        size_t size_copy = size;

        while (size_copy >= 32)
        {
            hash_table_search (&hashtable_crc32, begin_word);
            begin_word += 32;
            size_copy  -= 32;
        }
    }

    // =========== УДАЛЕНИЕ ТАБЛИЦ =============

    hash_table_deleter (&hashtable_crc32);

    fclose (fp_add);
    fclose (fp_search);
    fclose (fp_bin);

    free (test_buf);

    return 0;
}
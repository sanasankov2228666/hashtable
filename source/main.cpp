#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_dump.h"
#include "../str_funcs/str_func.h"

int a = 0;

int main (void)
{

    init_crc32_table ();

    // ====== ОБЪЯВЛЕНИЕ ХЭШ ТАБЛИЦ С РАЗНЫМИ ХЭШ ФУЦНКЦИЯМИ ======

    HashTable hashtable_crc32 =  hash_table_constructor (hash_func_crc32);

    // =============================================================

    FILE* fp_add = fopen ("textfile.txt", "r");
    if (!fp_add) return 1;

    FILE* fp_search = fopen ("test.txt", "r");
    if (!fp_search) return 1;
    
    hash_table_add_file (&hashtable_crc32, fp_add);

    char* test_buf = NULL;
    file_read (fp_search, &test_buf);

    // =============== ТЕСТЫ ===================

    char* begin_word = test_buf;
    char* end_word   = test_buf;

    for (int i = 0 ; i < 50; i++)
    {
        begin_word = test_buf;

        while (*begin_word != '\0')
        {
            begin_word = find_word_begin (begin_word);
            end_word   = find_word_end   (begin_word);
    
            char old_letter = *end_word;
            *end_word = '\0';
    
            hash_table_search (&hashtable_crc32, begin_word);

            *end_word = old_letter;
            begin_word = end_word;

            a++;
        }
    }

    // =========== УДАЛЕНИЕ ТАБЛИЦ =============

    hash_table_deleter (&hashtable_crc32);

    fclose (fp_add);
    fclose (fp_search);

    free (test_buf);

    return 0;
}
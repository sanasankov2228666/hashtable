#include "hashtable.h"
#include "hashtable_dump.h"

int main (void)
{

    init_crc32_table ();

    // ====== ОБЪЯВЛЕНИЕ ХЭШ ТАБЛИЦ С РАЗНЫМИ ХЭШ ФУЦНКЦИЯМИ =======

    HashTable hashtable1      =  hash_table_constructor (hash_func_1);
    HashTable hashtable2      =  hash_table_constructor (hash_func_2);
    HashTable hashtable3      =  hash_table_constructor (hash_func_3);
    HashTable hashtable4      =  hash_table_constructor (hash_func_4);
    HashTable hashtable5      =  hash_table_constructor (hash_func_5);
    HashTable hashtable6      =  hash_table_constructor (hash_func_6);
    HashTable hashtable_crc32 =  hash_table_constructor (hash_func_crc32);

    // =============================================================

    FILE* fp = fopen ("textfile.txt", "r");
    if (!fp) return 1;
    
    hash_table_add_file (&hashtable_crc32, fp);
    hash_table_add_file (&hashtable1, fp);
    hash_table_add_file (&hashtable2, fp);
    hash_table_add_file (&hashtable3, fp);
    hash_table_add_file (&hashtable4, fp);
    hash_table_add_file (&hashtable5, fp);
    hash_table_add_file (&hashtable6, fp);

    hashtable_dump (&hashtable_crc32, "hash_crc32");
    hashtable_dump (&hashtable1, "hash1");
    hashtable_dump (&hashtable2, "hash2");
    hashtable_dump (&hashtable3, "hash3");
    hashtable_dump (&hashtable4, "hash4");
    hashtable_dump (&hashtable5, "hash5");
    hashtable_dump (&hashtable6, "hash6");

    search hash1 = hash_table_search (&hashtable1, "baseness");
    search hash2 = hash_table_search (&hashtable2, "baseness");
    search hash3 = hash_table_search (&hashtable3, "baseness");
    search hash4 = hash_table_search (&hashtable4, "baseness");
    search hash5 = hash_table_search (&hashtable5, "baseness");
    search hash6 = hash_table_search (&hashtable6, "baseness");
    search hash_crc = hash_table_search (&hashtable_crc32, "baseness");
    search hash_crc2 = hash_table_search (&hashtable_crc32, "asdadasd");

    printf ("search 1 (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash1.table_i, hash2.list_i);
    printf ("search 2 (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash2.table_i, hash2.list_i);
    printf ("search 3 (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash3.table_i, hash3.list_i);
    printf ("search 4 (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash4.table_i, hash4.list_i);
    printf ("search 5 (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash5.table_i, hash5.list_i);
    printf ("search 6 (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash6.table_i, hash6.list_i);
    printf ("search crc (baseness) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash_crc.table_i, hash_crc.list_i);
    printf ("search crc (asdadasd) - adres\ntable i = %zu\nlist_i = %zu\n\n", hash_crc2.table_i, hash_crc2.list_i);

    // =========== УДАЛЕНИЕ ТАБЛИЦ =============

    hash_table_deleter (&hashtable1);
    hash_table_deleter (&hashtable2);
    hash_table_deleter (&hashtable3);
    hash_table_deleter (&hashtable4);
    hash_table_deleter (&hashtable5);
    hash_table_deleter (&hashtable6);
    hash_table_deleter (&hashtable_crc32);

    return 0;
}
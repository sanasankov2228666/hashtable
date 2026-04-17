#ifndef HASH_TABLE_H
#define HASH_TABLE_H

error_t hashtable_dump (HashTable* hashtable, const char* dump_name);

void hash_table_text_dump (const struct HashTable* ht);

void hashtable_png_gen (HashTable* hashtable, FILE* html_file, char* path);

error_t hashtable_histogram (HashTable* hashtable, FILE* html_file);

error_t create_table_dump_folder (const char* dump_name);

#endif 
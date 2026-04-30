#ifndef HASH_TABLE_H
#define HASH_TABLE_H

error_t hashtable_dump (HashTable* hashtable, const char* dump_name);

void hash_table_text_dump (const struct HashTable* ht);

void hashtable_png_gen (HashTable* hashtable, FILE* html_file, char* path);

error_t hashtable_get_dump_data (HashTable* hashtable, FILE* data_file);

error_t make_plot_script (FILE* plot_file, const char* data_filename, const char* path);

error_t create_table_dump_folder (const char* dump_name);

#endif 
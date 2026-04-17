#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../list/list.h"

#define  SIZE_TABLE 20
#define  DFLT_SIZE_CLSN 3

typedef size_t (*hash_func) (const char*); 

struct search
{
    size_t table_i = 0;
    size_t list_i  = 0;
};


struct HashTable
{
    list_s* hash = NULL;
    size_t numb_words = 0;

    hash_func func = NULL;
    int errors;
};

// ============================================================ ПРОТОТИПЫ ===========================================================

HashTable hash_table_constructor (hash_func func);
error_t   hash_table_deleter     (HashTable* hashtable);

// =========== ФУНКЦИИ ВЗАИМОДЕЙСТВИЯ ============

error_t hash_table_add      (HashTable* hashtable, char* word);
search  hash_table_search   (HashTable* hashtable, char* word);
error_t hash_table_add_file (HashTable* hashtable, FILE* fp);



// ================ ХЭШ ФУНКЦИИ ================

size_t hash_func_1 (const char* word);
size_t hash_func_2 (const char* word);
size_t hash_func_3 (const char* word);
size_t hash_func_4 (const char* word);
size_t hash_func_5 (const char* word);
size_t hash_func_6 (const char* word);

size_t hash_func_crc32 (const char* word);

// ========== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===========

unsigned char rol (unsigned char val, int i);
unsigned char ror (unsigned char val, int i);

search not_found      (void);
void init_crc32_table (void);

char* find_word_begin (char* buffer);
char* find_word_end   (char* buffer);

#endif
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../list/list.h"
#include "config.h"

// #define  SIZE_TABLE 4001
#define  DFLT_SIZE_CLSN 3


#ifdef CRC_32_OPT
    typedef size_t (*hash_func) (const char*, size_t len); 
#else
    typedef size_t (*hash_func) (const char*);
#endif 

struct search
{
    size_t table_i = 0;
    size_t list_i  = 0;
};

struct HashTable
{
    list_s* hash = NULL;
    size_t size  = 0;
    size_t numb_words = 0;

    hash_func func = NULL;
    int errors;
};

// ============================================================ ПРОТОТИПЫ ===========================================================

HashTable hash_table_constructor (hash_func func, size_t size);
error_t   hash_table_deleter     (HashTable* hashtable);

// =========== ФУНКЦИИ ВЗАИМОДЕЙСТВИЯ ============

error_t hash_table_add      (HashTable* hashtable, char* word);
search  hash_table_search   (HashTable* hashtable, char* word);
error_t hash_table_add_file (HashTable* hashtable, FILE* fp);

// ================ ХЭШ ФУНКЦИИ ================

size_t hash_func_1or0     (const char* word);
size_t hash_func_frst_ltr (const char* word);
size_t hash_func_strlen   (const char* word);
size_t hash_func_asci_sum (const char* word);
size_t hash_func_rol      (const char* word);
size_t hash_func_ror      (const char* word);

#ifdef CRC_32_OPT
    extern "C" size_t my_crc32(const char* word, size_t len);
#else
    size_t hash_func_crc32 (const char* word);
    void init_crc32_table (void);
#endif

// ========== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===========

unsigned char rol (unsigned char val, int i);
unsigned char ror (unsigned char val, int i);

search not_found  (void);

#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>

#include "config.h"
#include "hashtable.h"
#include "../str_funcs/str_func.h"

uint32_t crc32_table[256];

// ===================================================== КОНСТРУКТОР И ДЕЛИТЕР ========================================================

//! функция инцилизации структуры таблицы
HashTable hash_table_constructor (hash_func func)
{
    HashTable table = {};
    table.func = func;

    table.hash = (list_s*) calloc (SIZE_TABLE, sizeof (list_s));
    if (!table.hash)
    {
        D_PRINT ("ERROR in creating table\n");
        table.errors = ERROR;
    }

    return table;
}

//! функция удаления таблицы
error_t hash_table_deleter (HashTable* hashtable)
{
    for (int i = 0; i < SIZE_TABLE; i++)
    {
        if (hashtable->hash[i].is_active)
            list_deleter (&hashtable->hash[i]);
    }

    free (hashtable->hash);

    return SUCCSES;
}

// ============================================= ФУНКЦИИ ВЗАИМОДЕЙСТВИЯ С ТАБЛИЦЕЙ =================================================

//! фукнция добавления элемента в таблицу
error_t hash_table_add (HashTable* hashtable, char* word)
{
    size_t index = hashtable->func (word) % SIZE_TABLE;
    list_s* cur_list = &hashtable->hash[index];

    if (!cur_list->is_active)
    {
        if (list_creator (cur_list, DFLT_SIZE_CLSN))
            return ERROR;

        if (insert_begin (cur_list, word))
            return ERROR;

        hashtable->numb_words++;
        return SUCCSES;
    }

    size_t search = list_search (cur_list, word);

    if (search == PZN)
        insert_begin (cur_list, word);

    else cur_list->data[search].count++;

    hashtable->numb_words++;
    return SUCCSES;
}

//! функция добавления в таблицу слов из файла 
error_t hash_table_add_file (HashTable* hashtable, FILE* fp)
{
    char* text = NULL;
    size_t len = file_read (fp, &text);
    
    if (!text) 
    {
        D_PRINT ("ERROR file_read return NULL\n");
        return ERROR;
    }
    
    char* begin_word = text;
    char* end_word   = text;

    while (*begin_word != '\0')
    {
        begin_word = find_word_begin (begin_word);
        end_word   = find_word_end   (begin_word);

        char old_letter = *end_word;
        *end_word = '\0';

        hash_table_add (hashtable, begin_word);

        *end_word = old_letter;
        begin_word = end_word;
    }
    
    free (text);

    return SUCCSES;
}

//! функция поиска в хэш таблице
search hash_table_search (HashTable* hashtable, char* word)
{
    size_t index = hashtable->func (word) % SIZE_TABLE;
    search elem = {};
    
    if (!hashtable->hash[index].data)
        return not_found ();
    
    elem.table_i  = index;
    list* cur_list = &hashtable->hash[index];

    size_t search = list_search (cur_list, word);
    if (search == PZN) return not_found ();

    elem.list_i = search;
    return elem;
}

// ======================================================= ХЭШ ФУНКЦИИ ==============================================================

size_t hash_func_1 (const char* word)
{
    return *word > 90;
}

size_t hash_func_2 (const char* word)
{
    if (!word) return 0;
    return (size_t) *word;
}

size_t hash_func_3 (const char* word)
{
    return strlen (word);
}

size_t hash_func_4 (const char* word)
{
    size_t let_sum = 0;
    
    for (int i = 0; word[i] != '\0' && i < 64; i++)
        let_sum += word[i];

    return let_sum;
}

size_t hash_func_5 (const char* word)
{
    size_t hash = word[0];
    if (hash == 0) return hash;

    hash = rol (hash, 3);
    hash = hash ^ word[1];

    return hash;
}

size_t hash_func_6 (const char* word)
{
    size_t hash = word[0];
    if (hash == 0) return hash;

    hash = ror (hash, 3);
    hash = hash ^ word[1];

    return hash;
}

size_t hash_func_crc32 (const char* word)
{
    uint32_t crc = 0xFFFFFFFF;
    
    size_t length = strlen (word);

    for (size_t i = 0; i < length; i++)
    {
        unsigned char table_index = (crc ^ word[i]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[table_index];
    }

    return (size_t) crc ^ 0xFFFFFFFF;
}

// =================================================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ========================================================

unsigned char rol (unsigned char val, int i)
{
    return (val << i) | (val >> (8 - i));
}

unsigned char ror (unsigned char val, int i)
{
    return (val >> i) | (val << (8 - i));
}

search not_found (void)
{
    search not_found = {PZN, PZN};
    return not_found;
}

void init_crc32_table (void)
{
    uint32_t polynomial = 0xEDB88320;

    for (uint32_t i = 0; i < 256; i++)
    {
        uint32_t crc = i;

        for (int j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;

            else crc >>= 1;
        }

        crc32_table[i] = crc;
    }
}

char* find_word_begin(char* buffer)
{
    while (*buffer != '\0' && !isalnum(*buffer))
        buffer++;

    return buffer;
}

char* find_word_end(char* buffer)
{
    while (*buffer != '\0' && isalnum(*buffer))
        buffer++;

    return buffer;
}
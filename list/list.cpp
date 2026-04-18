#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <immintrin.h>

#include "list.h"
#include "checker.h"
#include "../source/config.h"

#define MAX_WORD_SIZE

// =====================================================================================================================================
//                                                   ФУНЦИИ СПИСКА
// =====================================================================================================================================

//!функция создания списка
error_t list_creator(list_s* list, size_t capacity)
{
    if (capacity < 1)
    {
        D_PRINT ("bad capacity\n");
        return ERROR;
    }

    list->data = (list_t*) calloc (capacity + 2, sizeof(list_t));
    PTR_CLC_CHECK(list->data);

    list->next = (size_t*) calloc (capacity + 2, sizeof(size_t));
    PTR_CLC_CHECK(list->next);

    list->prev = (size_t*) calloc (capacity + 2, sizeof(size_t));
    PTR_CLC_CHECK(list->prev);

    list->capacity = capacity;
    list->size = 0;

    list->next[0] = 0;
    list->next[capacity + 1] = 0;

    list->prev[0] = 0;
    list->prev[capacity + 1] = PZN;

    list->free_i = 1;

    for (size_t i = 1; i <= capacity; i++)
    {
        list->next[i] = i + 1;
        list->prev[i] = PZN;
    }

    list->is_active = true;

    VERIFY;

    return SUCCSES;
}

//!удаление списка
error_t list_deleter(list_s* list)
{
    size_t curr = list->next[0];

    while (curr != 0)
    {
        free (list->data[curr].word);
        list->data[curr].word = NULL;
        curr = list->next[curr];
    }

    free (list->data);
    free (list->next);
    free (list->prev);
    
    list->capacity = 0;
    list->size = 0;
    list->free_i = 0;

    D_PRINT ("list was deleted\n");

    return SUCCSES;
}

//!функция удлинения
error_t list_realloc(list_s* list)
{
    list->data = (list_t*) realloc ( list->data, sizeof(list_t) * (list->capacity + 1) * 2);
    PTR_RLC_CHECK(list->data)

    list->next = (size_t*) realloc ( list->next, sizeof(size_t) * (list->capacity + 1) * 2);
    PTR_RLC_CHECK(list->next)

    list->prev = (size_t*) realloc ( list->prev, sizeof(size_t) * (list->capacity + 1) * 2);
    PTR_RLC_CHECK(list->prev)

    for (size_t i = list->capacity + 1; i < (list->capacity + 1) * 2; i++ )
    {
        list->data[i].word = NULL;
        list->data->word   = 0;
        list->next[i] = i + 1;
        list->prev[i] = PZN;
    }

    list->free_i = list->capacity + 1;
    list->capacity = list->capacity * 2;
    list->next[list->capacity + 1] = 0;

    return SUCCSES;
}

// ============================================ ФУНКЦИИ ВЗАИМОДЕЙСТВИЯ СО СПИСКОИ ===================================================

error_t insert_begin(list_s* list, const char* word)
{   
    VERIFY;

    if ( list->size == list->capacity )
    {
        if (list_realloc(list)) return ERROR;
    }

    size_t search = list_search (list, word);
    if (search != PZN)
    {
        list->data[search].count++;
        return SUCCSES;
    }

    size_t index = list->free_i;
    list->free_i = list->next[list->free_i];

    char* word32 = (char*) calloc (32, sizeof (char));
    if (!word32)
    {
        D_PRINT ("ERROR in calloc returned adres\n");
        return ERROR;
    }

    list->data[index].word  = strncpy (word32, word, 31);
    list->data[index].count = 1;

    size_t old_head = get_head (list);
    list->next[0] = index;
    list->next[get_head (list)] = old_head;
    list->prev[old_head] = get_head (list);
    list->prev[get_head (list)] = 0;

    list->size++;

    VERIFY;

    return SUCCSES;
}

//!функция вставки в конец
error_t insert_end(list_s* list, const char* word)
{   
    VERIFY;

    if ( list->size == list->capacity )
    {
        if (list_realloc (list)) return ERROR;
    }

    size_t search = list_search (list, word);
    if (search != PZN)
    {
        list->data[search].count++;
        return SUCCSES;
    }
    
    size_t index = list->free_i;

    list->free_i = list->next[list->free_i];
    list->data[index].word  = strdup (word);
    list->data[index].count = 1;

    size_t old_tail = get_tail (list);
    list->prev[0] = index;

    list->next[old_tail] = get_tail (list);
    list->prev[get_tail (list)] = old_tail;
    list->next[get_tail (list)] = 0;

    list->size++;

    VERIFY;

    return SUCCSES;
}

//!функция удаления начала
error_t delete_begin(list_s* list)
{
    VERIFY;
    
    if (list->size == 0) return ERROR;

    size_t index = list->free_i;

    size_t old_head = get_head (list);
    list->next[0] = list->next[get_head (list)];
    list->prev[get_head (list)] = 0;

    list->prev[old_head] = PZN;
    list->free_i = old_head;
    list->next[old_head] = index;

    list->size--;

    VERIFY;

    return SUCCSES;
}

//!функция удаления конца
error_t delete_end(list_s* list)
{
    VERIFY;

    if (list->size == 0) return ERROR;

    size_t index = list->free_i;

    size_t old_tail = get_tail (list);
    list->prev[0] = list->prev[get_tail (list)];

    list->next[get_tail (list)] = 0;
    list->prev[old_tail] = PZN;
    list->free_i = old_tail;
    list->next[old_tail] = index;

    list->size--;

    VERIFY;

    return SUCCSES;
}


// ====================================================================================================================================

//!обращение к head
size_t get_head (list_s* list)
{
    return list->next[0];
}

//!обращение к tail
size_t get_tail (list_s* list)
{
    return list->prev[0];
}

size_t list_search (list_s* list, const char* word)
{
    size_t curr = get_head (list);

    char word_buf[32] = {};
    strncpy (word_buf, word, 31);

    __m256i v_word = _mm256_loadu_si256 ( (__m256i*) word_buf);
    
    while (curr != 0)
    {
        __m256i v_list_word = _mm256_loadu_si256 ( (__m256i*) list->data[curr].word);
        __m256i cmp = _mm256_cmpeq_epi8 (v_word, v_list_word);

        if (_mm256_movemask_epi8 (cmp) == 0xFFFFFFFF)
            return curr;

        curr = list->next[curr];
    }

    return PZN;
}
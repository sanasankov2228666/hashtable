#ifndef LIST_H
#define LIST_H

#include <stdio.h>

// === Значение для незанятых ячеек ===
#define PZN (size_t)-1

// === тип данных хранящийся в списке ===
struct list_t
{
   char* word = NULL;
   size_t count  = 0;
};


// === тип данных для ошибок ===
typedef int error_t;

// ====== структура списка ======
struct list
{
   list_t* data = NULL;
   size_t* next = NULL;
   size_t* prev = NULL; 

   size_t capacity = 0;
   size_t size     = 0;

   size_t free_i = 0; 

   int is_active = false;

   size_t error  = 00000000;
};

typedef struct list list_s;

enum
{
   ERROR   = 1,
   SUCCSES = 0,
};

// === коды ошибок ===
enum
{
    correct = 0,
    bad_size = 1,
    bad_capacity = 2,
    bad_pointer = 4,
    kanareika_error = 8,
    poison_error = 16,
    hash_error = 32,
};

// ===== проверки указателей =====

#define PTR_CLC_CHECK(ptr)\
         if (ptr == NULL) \
         {\
            printf("NULL pointer after CALLOC\n");\
            return ERROR;\
         }\

#define PTR_RLC_CHECK(ptr)\
         if (ptr == NULL) \
         {\
            printf("NULL pointer after REALLOC\n");\
            free (ptr);\
            return ERROR;\
         }\

// ====================================================== ДОП ФУНКЦИИ ===========================================================

size_t get_head (list_s* list);
size_t get_tail (list_s* list);

// ==================================================== ФУНКЦИИ СПИСКА =============================================================

error_t list_creator  (list_s* list, size_t capacity);
error_t list_deleter  (list_s* list);
error_t list_realloc  (list_s* list);

error_t insert_begin  (list_s* list, const char* val);
error_t insert_end    (list_s* list, const char* val);
error_t delete_begin  (list_s* list);
error_t delete_end    (list_s* list);

size_t list_search (list_s* list, const char* val);

#endif
#ifndef CHECKER_H_
#define CHECKER_H_

#include "list.h"

#define INF 1000000000

// === ENUM для ошибок ===

enum
{   
    CORRECT       = 0,
    BAD_POINTER   = 1,
    BAD_CAPACITY  = 2,
    BAD_SIZE      = 4,
    BAD_FREE      = 8,
    CANARY_CHANGE = 16,
    BAD_HEAD      = 32,
    BAD_TAIL      = 64,
    BAD_ORDER     = 128,
};

// === define для проверок в конце и начале функций ===

// ================================================= ФУНКЦИИ ДАМПА ==========================================================

error_t generate_dot_dump (list_s* list, const char* filename, FILE* html_file, const char* reason,  const char* path);

error_t html_generator (const char* filename, FILE* html_file);

void list_dump (list_s list, const char* reason, FILE* html_file, const char* path);

void counter_reset (void);

// ================================================ ФУКНКЦИИ ПРОВЕРКИ ======================================================

error_t checker (list_s* list);

error_t verify  (list_s* list, const char* func, const char* file, int LINE);

#endif
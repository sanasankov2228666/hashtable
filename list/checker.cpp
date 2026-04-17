#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "list.h"
#include "../source/config.h"
#include "checker.h"

#define DOT_FILENAME_LEN 200

static int dump_count = 0;

// =====================================================================================================================================
//                                                    ФУНКЦИИ ГЕНЕРАЦИИ ДАМПА
// =====================================================================================================================================

error_t generate_dot_dump (list_s* list, const char* filename, FILE* html_file, const char* reason,  const char* path)
{
    char dot_filename[DOT_FILENAME_LEN] = {};
    snprintf(dot_filename, sizeof(dot_filename), "%sdot/%s.dot", path, filename);

    FILE* dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        D_PRINT ("Error creating DOT file\n");
        return ERROR;
    }
    
    fprintf (dot_file, "digraph List {\n"
                      "\trankdir=LR;\n"
                      "\tnodesep=0.8;\n"
                      "\tranksep=1;\n"
                      "\tnode [shape=record, style=filled];\n"
                      "\tedge [sep=10];\n"
                      "\tfontsize = 24;\n"
                      "\tlabel=\"%s\";\n"
                      "\tlabelloc=t;\n\n", reason);
    
    fprintf (dot_file, "\n");

    for (size_t i = 0; i < list->capacity + 2; i++)
    {
        fprintf (dot_file, "\tnode%zu [label = \"index %zu|word = %s|count = %zu |next = %zu|prev = %zu\"];\n",
        i, i, list->data[i].word, list->data[i].count, list->next[i], list->prev[i]);
    }
    
    fprintf (dot_file, "\n");

    for (size_t i = list->next[get_head(list)]; i != get_tail(list); i = list->next[i])
    {
        if ( list->next[i] > list->size) break;

        if (i != 0)
        fprintf (dot_file, "\tnode%zu [fillcolor=\"lightblue\", color=\"darkblue\", style=\"filled\"];\n", i);
    }

    fprintf (dot_file, "\n");

    for (size_t i = 0; i < list->capacity + 1; i++)
    {
        fprintf (dot_file, "\tnode%zu -> node%zu [style=invis, weight=1000];\n", i, i + 1);
    }

    fprintf(dot_file, "\n");

    for( size_t i = 1; i < list->capacity + 1; i++)
    {   
        if (list->next[i] > (list->capacity + 1) && list->prev[i] != PZN)
        {
            fprintf (dot_file, "\tERROR_NEXT%zu [shape=octagon, color=\"firebrick2\", style=\"filled\"];\n"
                              "\tnode%zu->ERROR_NEXT%zu [color = deeppink, constraint=false];\n", list->next[i], i, list->next[i]);
        }

        else
        {
            if (list->prev[i] != PZN && list->next[i] != 0)
            fprintf (dot_file, "\tnode%zu->node%zu [color = blue, constraint=false];\n", i, list->next[i]);
        }
    }

    fprintf (dot_file, "\n");

    for( size_t i = 1; i < list->capacity + 1; i++)
    {
        if (list->prev[i] != 0 && list->prev[i] != PZN)
        {   
            if (list->prev[i] > list->capacity + 1)
            {
                fprintf (dot_file, "\tERROR_PREV%zu [shape=octagon, color=\"firebrick2\", style=\"filled\"];"
                                  "\tnode%zu->ERROR_PREV%zu [color = deeppink, constraint=false];\n", list->prev[i], i, list->prev[i]);
            }

            else
            {
                if (list->prev[i] != PZN)
                fprintf (dot_file, "\tnode%zu->node%zu [color = red, constraint=false];\n", i, list->prev[i]);
            }
        }
    }

    fprintf (dot_file, "\n");

    for(size_t i = list->free_i; list->next[i] != 0; i = list->next[i])
    {
        fprintf (dot_file, "\tnode%zu->node%zu [color = green, constraint=false];\n", i, list->next[i]);
    }

    fprintf (dot_file, "\n"
                      "\tHEAD [shape=plaintext, fillcolor=\"royalblue\", color=\"darkblue\", style=\"filled\"];\n"
                      "\tTAIL [shape=plaintext, fillcolor=\"tomato\", color=\"darkred\", style=\"filled\"];\n"
                      "\tFREE [shape=plaintext, fillcolor=\"lawngreen\", color=\"darkgreen\", style=\"filled\"];\n"
                      "\tZERO [shape=plaintext, fillcolor=\"coral\", color=\"darkred\", style=\"filled\"];\n");
    
    fprintf (dot_file, "\t{rank = same ; HEAD; node%zu [fillcolor=\"royalblue\", color=\"darkblue\", style=\"filled\"]};\n"
                      "\t{rank = same ; TAIL; node%zu [fillcolor=\"tomato\", color=\"darkred\", style=\"filled\"]};\n"
                      "\t{rank = same ; FREE; node%zu [fillcolor=\"lawngreen\", color=\"darkgreen\", style=\"filled\"]};\n"
                      "\t{rank = same ; ZERO; node0   [fillcolor=\"coral\", color=\"darkred\", style=\"filled\"]};\n",
                       get_head(list), get_tail(list), list->free_i);

    fprintf (dot_file, "\n\n");

    fprintf (dot_file, "}\n");
    fclose  (dot_file);
    
    char command[400];
    snprintf(command, sizeof (command), "dot -Tpng %sdot/%s.dot -o %spng/%s.png",path, filename, path, filename);
    
    int result = system (command);
    if (result)
    {
        D_PRINT ("ERROR in generating list png\n");
        return ERROR;
    }
    
    D_PRINT ("PNG was generated\n");

    html_generator (filename, html_file);

    return SUCCSES;
}

error_t html_generator(const char* filename, FILE* html_file)
{
    fprintf(html_file, "<figure>\n"
                            "\t<img src=\"png/%s.png\"/>\n"
                            "</figure>\n", filename);

    return 0;
}

//! распечатка списка
void list_dump(list_s list, const char* reason, FILE* html_file, const char* path)
{   
    fprintf(html_file,     "\n-------------------------------------------list-dump------------------------------------------------\n\n"
                           "Reason - %s\n"
                           "head = %zu\n"
                           "tail = %zu\n"
                           "size = %zu\n"
                           "capacity = %zu\n"
                           "free_index = %zu\n",
                           reason, get_head(&list), get_tail(&list), list.size, list.capacity, list.free_i);

    fprintf(html_file, "\n--------------------------------------------------END------------------------------------------------\n");

    char filename[40] = {};
    snprintf(filename, sizeof(filename), "list_dump_%d", dump_count++);

    if (generate_dot_dump(&list, filename, html_file, reason, path) ) printf("PNG generate failed\n");
}

void counter_reset (void)
{
    dump_count = 0;
}


// =====================================================================================================================================
//                                                    ФУНКЦИИ ПРОВЕРКИ СПИСКА 
// =====================================================================================================================================

// === ФУНКЦИЯ ПРОВЕРКИ ОШИБОК ===
error_t checker(list_s* list)
{   
    if (list->data == NULL || list->next == NULL || list->prev == NULL)
        list->error |= BAD_POINTER;

    if (list->capacity < 1 || list->capacity > INF)
        list->error |= BAD_CAPACITY;

    if (list->size > list->capacity || list->size > INF) 
        list->error |= BAD_SIZE;

    if (list->prev[list->free_i] != PZN && list->prev[list->free_i] != 0)
        list->error |= BAD_FREE;

    size_t head = get_head (list);
    size_t tail = get_tail (list);

    if (list->size > 0)
    {
        if (list->prev[head] != 0) list->error |= BAD_HEAD;
        if (list->next[tail] != 0) list->error |= BAD_TAIL;
    }

    size_t current = 0;
    for (size_t step = 0; step <= list->size; step++)
    {
        size_t next_i = list->next[current];

        if (next_i > list->capacity + 1)
        {
            list->error |= BAD_ORDER;
            break;
        }

        if (list->prev[next_i] != current)
        {   
            list->error |= BAD_ORDER;
            break;
        }

        current = next_i;
        
        if (current == 0 && step < list->size)
        {
            list->error |= BAD_ORDER;
            break;
        }
    }

    return (int) list->error;
}

//  === ФУНКЦИЯ ОБРАБОТКИ ОШИБКИ ===
error_t verify (list_s* list, const char* func, const char* file, int LINE)
{
    int error = checker(list);

    if (list->error & 1)
    {
        printf("bad pointer!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 2)
    {
        printf("bad capacity!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 4)
    {
        printf("bad size!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 8)
    {
        printf("bad free!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 16)
    {
        printf("canary change!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_deleter(list);
    }

    if (list->error & 32)
    {
        printf("bad head!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 64)
    {
        printf("bad tail!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 128)
    {
        printf("bad order!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (error != 0)
    {
        list_deleter(list);
        return ERROR;
    }

    return SUCCSES;
}
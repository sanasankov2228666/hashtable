
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_dump.h"
#include "config.h"
#include "../list/list.h"
#include "../list/checker.h"

#define COMMAND_LEN 100
#define HTML_FILENAME_LEN 100
#define PATH_LEN 100

// ====================================================== ДАМП ХЭШ ТАБЛИЦЫ ===============================================================

//! Основная функция дампа
error_t hashtable_dump (HashTable* hashtable, const char* dump_name)
{
    if (!hashtable || !dump_name)
    {
        D_PRINT ("ERROR in hash\n");
        return ERROR;
    }

    char path[PATH_LEN] = {};
    char html_filename[HTML_FILENAME_LEN] = {};
    
    snprintf (path, PATH_LEN, "dump/%s/", dump_name);
    snprintf (html_filename, HTML_FILENAME_LEN, "%stable.html", path);
    
    create_table_dump_folder (path);
    
    FILE* html_file = fopen (html_filename, "w");
    if (!html_file)
    {
        D_PRINT ("ERROR in file openning\n");
        return ERROR;
    }
    
    hashtable_histogram (hashtable, html_file);

    if (SIZE_TABLE < 30)
    {
        counter_reset ();
        hashtable_png_gen (hashtable, html_file, path );
    }

    return SUCCSES;
}

// ==================================================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ==========================================================

//! Генерация дампа для одного из хэшей
void hashtable_png_gen (HashTable* hashtable, FILE* html_file, char* path)
{
    for (int i = 0; i < SIZE_TABLE; i++)
    {
        fprintf (html_file, "<pre>\n<font size=\"6\">\n"
                            "============================================ HASHTALBE [%d] ======================================================\n\n", i);

        if (hashtable->hash[i].is_active)
            list_dump (hashtable->hash[i], "check list in table", html_file, path);

        else fprintf (html_file, "\n\n[    ZERO LIST     ]\n\n");

        fprintf (html_file, "\n\n===================================================================================================================\n\n"
                            "</font>\n");
    }
}

//! Создание папок для дампа
error_t create_table_dump_folder (const char* path)
{
    char command_mkdir_dump [COMMAND_LEN] = {};
    char command_mkdir_png  [COMMAND_LEN] = {};
    char command_mkdir_dot  [COMMAND_LEN] = {};

    snprintf (command_mkdir_dump, COMMAND_LEN, "mkdir %s",     path);
    snprintf (command_mkdir_dot,  COMMAND_LEN, "mkdir %sdot/", path);
    snprintf (command_mkdir_png,  COMMAND_LEN, "mkdir %spng/", path);

    if ( system (command_mkdir_dump) ||
         system (command_mkdir_dot)  ||
         system (command_mkdir_png)   )
    {
        D_PRINT ("ERROR in creating dump dir\n");
        return ERROR;
    }

    return SUCCSES;
}


// ===================================================== ГЕНЕРАЦИЯ ГИСТОГРАММЫ =========================================================

//! Генерация гистограммы
error_t hashtable_histogram (HashTable* hashtable, FILE* html_file)
{
    if (!hashtable || !html_file)
    {
        D_PRINT ("ERROR hashtable = NULL\n");
        return ERROR;
    }

    size_t max_size = 0;
    for (int i = 0; i < SIZE_TABLE; i++)
    {
        if (hashtable->hash[i].size > max_size)
            max_size = hashtable->hash[i].size;
    }

    if (max_size == 0) max_size = 1;

    fprintf (html_file, "<!DOCTYPE html>\n"
                        "<html>\n"
                        "<head>\n"
                        "\t<style>\n"
                        "\t\tbody {\n"
                        "\t\t\tfont-family: Arial;\n"
                        "\t\t\tmargin: 20px;\n"
                        "\t\t}\n"
                        "\t\t.histogram-wrapper {\n"
                        "\t\t\toverflow-x: auto;\n"
                        "\t\t\twidth: 100%%;\n"
                        "\t\t\tborder: 1px solid #ccc;\n"
                        "\t\t\tpadding: 15px;\n"
                        "\t\t\tbackground: #f9f9f9;\n"
                        "\t\t}\n"
                        "\t\t.histogram {\n"
                        "\t\t\tdisplay: flex;\n"
                        "\t\t\talign-items: flex-end;\n"
                        "\t\t\tgap: 2px;\n"
                        "\t\t\tmin-width: max-content;\n"
                        "\t\t\theight: 500px;\n"
                        "\t\t\tborder-bottom: 2px solid #333;\n"
                        "\t\t}\n"
                        "\t\t.bar {\n"
                        "\t\t\twidth: 12px;\n"
                        "\t\t\tbackground: #4682b4;\n"
                        "\t\t\tborder-radius: 3px 3px 0 0;\n"
                        "\t\t\ttransition: all 0.2s;\n"
                        "\t\t}\n"
                        "\t\t.bar:hover {\n"
                        "\t\t\tbackground: #ff6347;\n"
                        "\t\t}\n"
                        "\t\t.label {\n"
                        "\t\t\twidth: 12px;\n"
                        "\t\t\ttext-align: center;\n"
                        "\t\t\tfont-size: 8px;\n"
                        "\t\t\tcolor: #666;\n"
                        "\t\t}\n"
                        "\t\t.labels {\n"
                        "\t\t\tdisplay: flex;\n"
                        "\t\t\tgap: 2px;\n"
                        "\t\t\tmargin-top: 5px;\n"
                        "\t\t}\n"
                        "\t\t.stats {\n"
                        "\t\t\tmargin-bottom: 15px;\n"
                        "\t\t\tpadding: 10px;\n"
                        "\t\t\tbackground: #e8f0fe;\n"
                        "\t\t\tborder-radius: 5px;\n"
                        "\t\t}\n"
                        "\t</style>\n"
                        "</head>\n");

    fprintf (html_file, "<body>\n"
                        "\t<div class=\"stats\">\n"
                        "\t\t<strong> Хеш-таблица:</strong> %d списков. "
                        "Макс %zu элементов\n"
                        "\t</div>\n",
                        SIZE_TABLE, max_size);

    fprintf (html_file, "\t<div class=\"histogram-wrapper\">\n"
                        "\t\t<div class=\"histogram\">\n");
    
    for (int i = 0; i < SIZE_TABLE; i++)
    {
        int height = (max_size > 0) ? 
                     (hashtable->hash[i].size * 280) / max_size : 0;

        if (height == 0 && hashtable->hash[i].size > 0) height = 2;
        
        fprintf (html_file, "\t\t\t<div class=\"bar\" style=\"height: %dpx;\" "
                          "title=\"Список %d: %zu элементов\"></div>\n", 
                 height, i, hashtable->hash[i].size);
    }                 
    
    fprintf (html_file, "\t\t</div>\n"
                        "\t\t<div class=\"labels\">\n");

    for (int i = 0; i < SIZE_TABLE; i++)
    {
        if (SIZE_TABLE > 100)
        {
            if (i % 50 == 0 || i == SIZE_TABLE - 1)
                fprintf (html_file, "\t\t\t<div class=\"label\">%d</div>\n", i);
            
            else fprintf (html_file, "\t\t\t<div class=\"label\"></div>\n");
        } 
        
        else fprintf (html_file, "\t\t\t<div class=\"label\">%d</div>\n", i);
    }

    fprintf (html_file, "\t\t</div>\n"
                        "\t</div>\n"
                        "</body>\n"
                        "</html>\n");
    
    return SUCCSES;
}

// ===================================================== ТЕКСТОВЫЙ ДАМП =================================================================

//! Текстовый дамп
void hash_table_text_dump (const struct HashTable* ht)
{
    if (!ht)
    {
        D_PRINT ("ERROR pointer is NULL\n");
        return;
    }

    size_t active_lists_count = 0;
    size_t total_elements = 0;

    if (ht->hash)
    {
        for (size_t i = 0; i < ht->numb_words; ++i)
        {
            if (ht->hash[i].is_active)
            {
                active_lists_count++;
                total_elements += ht->hash[i].size;
            }
        }
    }

    printf ("\n============================================================\n");
    printf ("                        HASH TABLE DUMP                       \n");
    printf ("============================================================\n");

    printf ("  [SUMMARY]\n");
    printf ("  > Total (Capacity): %zu\n", ht->numb_words);
    
    double load_factor = (ht->numb_words > 0) ? (double)active_lists_count / ht->numb_words : 0;

    printf ("  > Active list:           %zu (%.2f%%)\n", active_lists_count, load_factor * 100);
    printf ("  > Total Words Stored:       %zu\n", total_elements);
    printf ("  > Table Errors:             %s (Code: 0x%X)\n", 
           (ht->errors == 0 ? "NONE" : "DETECTED"), ht->errors);

    printf ("------------------------------------------------------------\n");

    printf ("  [ACTIVE LISTS DETAILS]\n");
    
    if (active_lists_count == 0)
        printf ("  *** NO ACTIVE LISTS FOUND ***\n");

    else
    {
        printf("  %-10s %-12s %-12s %-10s\n", "INDEX", "SIZE", "CAPACITY", "ERROR_CODE");
        
        for (size_t i = 0; i < ht->numb_words; ++i)
        {
            list_s* lst = &(ht->hash[i]);

            if (lst->is_active)
            {
                printf ("  #%-9zu %-12zu %-12zu 0x%08zX\n", 
                       i, 
                       lst->size, 
                       lst->capacity, 
                       lst->error);
            }
        }
    }

    printf ("============================================================\n\n");
}
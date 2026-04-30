
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_dump.h"
#include "config.h"
#include "../list/list.h"
#include "../list/checker.h"

#define COMMAND_LEN 100
#define FILENAME_LEN 100
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
    char plot_filename[FILENAME_LEN] = {};
    char data_filename[FILENAME_LEN] = {};

    snprintf (path, PATH_LEN, "dump/%s/", dump_name);
    snprintf (plot_filename, FILENAME_LEN, "%shist.gp", path);
    snprintf (data_filename, FILENAME_LEN, "%sdata.txt",   path);

    create_table_dump_folder (path);
    
    FILE* data_file = fopen (data_filename, "w");
    if (!data_file)
    {
        D_PRINT ("ERROR in file openning\n");
        return ERROR;
    }

    FILE* plot_file = fopen (plot_filename, "w");
    if (!plot_file)
    {
        D_PRINT ("ERROR in file openning\n");
        fclose (data_file);
        return ERROR;
    }

    hashtable_get_dump_data (hashtable, data_file);
    make_plot_script (plot_file, data_filename, path);

    char plt_command [COMMAND_LEN] = {};

    snprintf (plt_command, COMMAND_LEN, "gnuplot %s", plot_filename);
    
    fclose (plot_file);
    fclose (data_file);

    if (system (plt_command))
    {
        D_PRINT ("ERROR in generating histogram (dump name %s)\n", dump_name);
        printf("erorr");
    }

    // if (SIZE_TABLE < 30)
    // {
    //     counter_reset ();
    //     hashtable_png_gen (hashtable, plot_file, path );
    // }

    return SUCCSES;
}

// ==================================================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ==========================================================

//! Генерация дампа для одного из хэшей
void hashtable_png_gen (HashTable* hashtable, FILE* html_file, char* path)
{
    for (int i = 0; i < hashtable->size; i++)
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

error_t make_plot_script (FILE* plot_file, const char* data_filename, const char* path)
{
    if (!plot_file || !data_filename || !path)
    {
        D_PRINT ("ERROR plot_file or data_filename or path == NULL\n");
        return ERROR; 
    }

    fprintf (plot_file, "set terminal pngcairo size 2000,600\n"
                        "set output '%shistogram.png'\n"
                        "set style fill solid 0.7\n"
                        "set boxwidth 1.0 relative\n"
                        "set xlabel \"Списки\"\n"
                        "set ylabel \"Количество слов\"\n"
                        "set title \"Гистограмма распределения слов\"\n"
                        "set grid ytics\n"
                        "set xtics rotate by -45\n"
                        "set xrange [0:*]\n"
                        "set yrange [0:*]\n"
                        "plot '%s' using 1:2 with impulses lc rgb \"blue\" title \"Хэш-таблица\"\n", 
                        path, data_filename);

    return SUCCSES;
}

//! Генерация гистограммы
error_t hashtable_get_dump_data (HashTable* hashtable, FILE* data_file)
{
    if (!hashtable || !data_file)
    {
        D_PRINT ("ERROR hashtable = NULL\n");
        return ERROR;
    }

    for (int i = 0; i < hashtable->size; i++)
        fprintf (data_file, "%d %zu\n",i , hashtable->hash[i].size);

    char plt_command [COMMAND_LEN] = {};

    

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
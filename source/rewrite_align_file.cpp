#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> 

#include "../str_funcs/str_func.h"
#include "rewrite_aling_file.h"
#include "config.h"

void rewrite_to_algn_bin (FILE* fp_words)
{
    FILE* file_out = fopen ("test.bin", "wb");
    if (!file_out)
    {
        D_PRINT ("ERROR in file opening\n");
        return;
    }

    char* test_buf = NULL;
    file_read (fp_words, &test_buf);

    char* begin_word = test_buf;
    char* end_word   = test_buf;

    char word32[32] = {};

    while (*begin_word != '\0')
    {
        begin_word = find_word_begin (begin_word);
        end_word   = find_word_end   (begin_word);

        char old_letter = *end_word;
        *end_word = '\0';

        strncpy (word32, begin_word, 31);
        fwrite  (word32, 1, 32, file_out);

        *end_word = old_letter;
        begin_word = end_word;
    }

    free (test_buf);
    fclose (file_out);
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
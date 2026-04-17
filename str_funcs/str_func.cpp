#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "str_func.h"
#include "../source/config.h"

//!чтение в один большой массив
size_t file_read (FILE* stream, char** buffer)
{
    if (!stream || !buffer)
    {
        D_PRINT ("ERROR in func params file_read");
        return 0;
    }

    size_t number = size_file (stream);
    *buffer = (char*) calloc ( (number + 2), sizeof (**buffer) );
    if (*buffer == NULL)
    {
        D_PRINT ("error in file read\n");
        return 0;
    }

    size_t check = fread (*buffer, sizeof (char), (size_t) number, stream);

    fseek (stream, 0, SEEK_SET);
    return (size_t) number;
}

size_t size_file (FILE* stream)
{
    if (stream == NULL)
    {
        D_PRINT ("error in size_file stream adres\n");
        return 0;
    }

    struct stat file = {};
    int descriptor = fileno (stream);
    fstat (descriptor, &file);

    return (size_t) file.st_size;
}
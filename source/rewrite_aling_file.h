#ifndef REWRITE_TO_ALGN_BIN_H
#define REWRITE_TO_ALGN_BIN_H

#include <stdio.h>

void rewrite_to_algn_bin (FILE* fp_words);

char* find_word_begin (char* buffer);

char* find_word_end   (char* buffer);

#endif

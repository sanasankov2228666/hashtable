#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include <time.h>

// #define DEBUG

#ifdef DEBUG
    #define D_PRINT(...) printf("In function %s, %s:%d\n", __func__, __FILE__, __LINE__);\
                         printf(__VA_ARGS__)
    #define VERIFY if (verify(list,  __FUNCTION__, __FILE__, __LINE__)) return ERROR;\

#else
    #define D_PRINT(...) //
    #define VERIFY //
#endif

#ifndef OPT
    #define OPT 1
#endif

#if OPT == 0
    #define NO_OPT
#elif OPT == 1
    #define STRCMP_OPT
#elif OPT == 2
    #define CRC_32_OPT
    #define STRCMP_OPT
#elif OPT == 3
    #define ASM_INLINE
    #define CRC_32_OPT
    #define STRCMP_OPT
#endif

#endif
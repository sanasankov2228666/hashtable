OPT ?= 0

LIST_SRC = list/list.cpp list/checker.cpp
STR_FUNC = str_funcs/str_func.cpp
HASH_TABLE = source/hashtable.cpp source/hashtable_dump.cpp source/rewrite_align_file.cpp

SOURCES = $(LIST_SRC) $(STR_FUNC) $(HASH_TABLE)
ASM_SRC = source/my_crc32.asm
OBJ = source/my_crc32.o

CFLAGS = -g -O3 -mavx2 -fno-omit-frame-pointer -DOPT=$(OPT)

ifeq ($(OPT), 2)
	ASM_CRC32 = $(OBJ)
else ifeq ($(OPT), 3)
	ASM_CRC32 = $(OBJ)
else
	ASM_CRC32 = 
endif

all: table

$(OBJ): $(ASM_SRC)
	@echo "=== Assembling CRC32 ==="
	nasm -f elf64 $(ASM_SRC) -o $(OBJ)

table: $(ASM_CRC32) source/main.cpp
	@echo "=== Compiling front ==="
	g++ $(CFLAGS) source/main.cpp $(SOURCES) $(ASM_CRC32) -o hashtable
	@echo "=== Compilation complete ==="

clean:
	rm -f hashtable $(OBJ)
section .text

global my_crc32

;________________________________________________________________________________________________________________________________________

my_crc32:
    mov eax, 0xFFFFFFFF

    cmp rsi, 8
    jb .loop_bytes

.loop_qwords:
    crc32 rax, qword [rdi]
    add rdi, 8
    sub rsi, 8
    cmp rsi, 8
    jae .loop_qwords

.loop_bytes:
    test rsi, rsi
    jz .done

.next_byte:
    crc32 eax, byte [rdi]
    inc rdi
    dec rsi
    jnz .next_byte

.done:
    not eax
    ret

section .note.GNU-stack noalloc noexec nowrite progbits

;________________________________________________________________________________________________________________________________________

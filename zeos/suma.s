# 1 "suma.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "suma.S"
# 1 "include/asm.h" 1
# 2 "suma.S" 2

.globl add2; .type add2, @function; .align 0; add2:
 push %ebp
    movl %esp,%ebp
 movl 0x8(%ebp),%edx
    movl 0xc(%ebp),%eax
    add %edx,%eax
    pop %ebp
    ret

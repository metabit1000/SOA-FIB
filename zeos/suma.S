#include <asm.h>

ENTRY(add2) 
	push   %ebp
    movl    %esp,%ebp
	movl   0x8(%ebp),%edx
    movl    0xc(%ebp),%eax
    add    %edx,%eax
    pop    %ebp
    ret

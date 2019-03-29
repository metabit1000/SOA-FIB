# 1 "switch_process.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "switch_process.S"
# 1 "include/asm.h" 1
# 2 "switch_process.S" 2

.globl task_switch; .type task_switch, @function; .align 0; task_switch:
 pushl %ebp
 movl %esp,%ebp

 pushl %esi
 pushl %edi
 pushl %ebx

 pushl 8(%ebp)
 call inner_task_switch

 popl %ebx
 popl %edi
 popl %esi

 movl %ebp,%esp
 popl %ebp
 ret

.globl inner_task_switch; .type inner_task_switch, @function; .align 0; inner_task_switch:
 pushl %ebp
 movl %esp,%ebp

 call current
 movl %eax,%edi
 movl 8(%ebp),%esi

 pushl %esi
 call extra

 movl %ebp, 4(%edi)
 movl 4(%esi), %esp
 popl %ebp
 ret

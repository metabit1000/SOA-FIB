# 1 "wrappers.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "wrappers.S"
# 1 "include/asm.h" 1
# 2 "wrappers.S" 2

.globl write; .type write, @function; .align 0; write:
 pushl %ebp
 movl %esp, %ebp

 pushl %edx
 pushl %ecx
 pushl %ebx

 movl 8(%ebp), %ebx
 movl 12(%ebp), %ecx
 movl 16(%ebp), %edx

 movl $4, %eax

 pushl $write_ret

 pushl %ebp
 movl %esp, %ebp

 sysenter
write_ret:

 popl %ebx
 popl %ecx
 popl %edx

 movl %ebp,%esp
 popl %ebp
 ret

.globl gettime; .type gettime, @function; .align 0; gettime:
 pushl %ebp
 movl %esp, %ebp

 movl $10,%eax

 pushl $time_ret

 pushl %ebp
 movl %esp, %ebp

 sysenter
time_ret:
 popl %ebp
 ret

.globl getpid; .type getpid, @function; .align 0; getpid:
 pushl %ebp
 movl %esp,%ebp

 movl $20,%eax

 pushl $pid_ret

 pushl %ebp
 movl %esp,%ebp

 sysenter
pid_ret:
 popl %ebp
 ret

.globl fork; .type fork, @function; .align 0; fork:
 pushl %ebp
 movl %esp,%ebp

 movl $2,%eax

 pushl $fork_ret

 pushl %ebp
 movl %esp,%ebp

 sysenter
fork_ret:
 popl %ebp
 ret

.globl exit; .type exit, @function; .align 0; exit:
 pushl %ebp
 movl %esp,%ebp

 movl $1,%eax

 pushl $exit_ret

 pushl %ebp
 movl %esp,%ebp

 sysenter
exit_ret:
 popl %ebp
 ret

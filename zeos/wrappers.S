#include <asm.h>

ENTRY(write)   //Implementación de write con sysenter
	pushl %ebp
	movl %esp, %ebp

	pushl %edx
	pushl %ecx  //guardamos valores en la stack
	pushl %ebx

	movl 8(%ebp), %ebx
	movl 12(%ebp), %ecx
	movl 16(%ebp), %edx  //aqui no hay problema en usarlos

	movl $4, %eax  //identifier of the system call

	pushl $write_ret           //guardo la direccion de retorno al codigo de usuario
	
	pushl %ebp
	movl %esp, %ebp   //fake dynamic link

	sysenter  //the trap
write_ret:
	//Faltaria aqui tratar el resultado con eax
	popl %ebx
	popl %ecx
	popl %edx
	
	movl %ebp,%esp
	popl %ebp
	ret
	
ENTRY(gettime)
	pushl %ebp
	movl %esp, %ebp
		
	movl $10,%eax     //identifier of the system call
	
	pushl $time_ret           //guardo la direccion de retorno al codigo de usuario
	
	pushl %ebp
	movl %esp, %ebp   //fake dynamic link

	sysenter  //the trap
time_ret:		 
	popl %ebp
	ret
	
ENTRY(getpid)
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

ENTRY(fork)
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

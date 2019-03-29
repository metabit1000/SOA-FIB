#include <asm.h>

ENTRY(task_switch)
	pushl %ebp
	movl %esp,%ebp
	
	pushl %esi
	pushl %edi
	pushl %ebx
	
	pushl 8(%ebp) //pushl @new
	call inner_task_switch
	
	popl %ebx
	popl %edi
	popl %esi
	
	movl %ebp,%esp
	popl %ebp
	ret
	
ENTRY(inner_task_switch)
	pushl %ebp
	movl %esp,%ebp
	
	call current
	movl %eax,%edi //direccion del task_union current
	movl 8(%ebp),%esi //@new
	
	pushl %esi
	call extra
	
	movl %ebp, 4(%edi)  // %ebp --> current()->kernel_esp
	movl 4(%esi), %esp  // new->task.kernel_esp --> %esp
	popl %ebp
	ret

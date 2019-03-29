/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");  //sti activa interrupciones, ya que en kernel mode estan desactivadas por default

	while(1)
	{
	;
	}
}

/* Cambio de contexto entre procesos; new = puntero al proceso que se ejecutará*/

void task_switch(union task_union * new); 
void inner_task_switch(union task_union * new); //codigo en switch_process.S

void extra(union task_union * new) {
	tss.esp0 = (unsigned long)&(new->stack[KERNEL_STACK_SIZE]);
	writeMSR(0x175,tss.esp0);
	
	set_cr3(get_DIR(&new->task));
}

void init_idle (void)
{
  /* PID = 0 y siempre en kernel mode; hace que la CPU este siempre ocupada */
  
  struct list_head *pointer = list_first(&freequeue); //primer elemento de la freequeue
  list_del(pointer); //lo elimino de la freequeue
  idle_task= list_entry(pointer, struct task_struct, list); //Get the container of this list element
  union task_union *idle_union = (union task_union*)idle_task;
  
  idle_task->PID = 0; 
  allocate_DIR(idle_task); //inicializo con nuevo directory
  
  idle_union->stack[KERNEL_STACK_SIZE - 1] = (DWord)&cpu_idle; 
  idle_union->stack[KERNEL_STACK_SIZE - 2] = 0;
  idle_union->task.kernel_esp = (unsigned long)&(idle_union->stack[KERNEL_STACK_SIZE - 2]); 
}

void init_task1(void)
{
   /* It is the first user process (ejecuta user.c) to be executed after booting the OS . PID = 1 */
	
  struct list_head *pointer = list_first(&freequeue); 
  list_del(pointer); 
  struct task_struct *task1= list_entry(pointer, struct task_struct, list); 
  union task_union *task1_union = (union task_union *)task1; 
  
  task1->PID = 1; 
  allocate_DIR(task1);
  set_user_pages(task1);
  
  tss.esp0 = &task1_union->stack[KERNEL_STACK_SIZE]; //actualizo la tss para que apunte a la pila del proceso
  writeMSR(0x175,tss.esp0); //tambien para el sysenter
  
  set_cr3(task1->dir_pages_baseAddr);
}


void init_sched()
{
	int i;
	INIT_LIST_HEAD(&freequeue);
	for (i = 0; i < 10; ++i) {
		list_add_tail(&task[i].task.list,&freequeue); //relleno la freequeue con todos los PCB's
	}
	INIT_LIST_HEAD(&readyqueue); //inicializo la readyqueue como vacia
}

struct task_struct* current()  //me devuelve la PCB del proceso que se esta actualmente ejecutando (4.2.2)
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}


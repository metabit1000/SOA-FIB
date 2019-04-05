/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#include <system.h>

#define LECTURA 0
#define ESCRIPTURA 1

int PID_ = 100;

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF; 
  if (permissions!=ESCRIPTURA) return -EACCES; 
  return 0;
}

int ret_from_fork()
{
	return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{ 
  int PID=-1;
  struct list_head *pointer;
  struct task_struct *child_task, *curr_task;
  union task_union *child_union, *curr_union;
  int frames[NUM_PAG_DATA], i; //20
  page_table_entry *child_pt, *curr_pt;

  /* En caso de que no hayan procesos libres; error */
  if (list_empty(&freequeue))
		return -EAGAIN;

  /* Obtengo un proceso nuevo de la freequeue */
  pointer = list_first(&freequeue); 
  list_del(pointer); 
  child_task = list_entry(pointer, struct task_struct, list); 
  child_union = (union task_union*)child_task;
  
  /* Obtengo el proceso actual (el padre) */
  curr_task = current();
  curr_union = (union task_union *)curr_task;

  /* Copy the parent’s task_union to the child + new directory */
  copy_data(curr_union,child_union,sizeof(union task_union));
  allocate_DIR(child_task);
  
  /* Busco paginas fisicas libres para data+stack del child process */
  for (i = 0; i < NUM_PAG_DATA; ++i) {
	  frames[i] = alloc_frame(); //Search a free physical page 
	  if (frames[i] < 0) {
		while (i > 0)
			free_frame(frames[--i]);
		return -EAGAIN;
	  }
  }
  
  child_pt = get_PT(child_task);
  curr_pt = get_PT(curr_task);
  int logical_page; //del hijo!
  
  /* Copiamos entradas de código + código del sistema y los datos (comparten) */  
  for (logical_page = 0; logical_page < NUM_PAG_KERNEL + NUM_PAG_CODE; ++logical_page) {
	  int frame = get_frame(curr_pt, logical_page); //del padre!
	  set_ss_pag(child_pt, logical_page, frame);
  }
  
  /* Copiamos entradas DATA+STACK (NO comparten) --> esto se ve genial en el power de teoria*/
  
  /* Hacemos que las entradas de la TP(child) apunten a los nuevos frames para el child_process(que tengo en frames[]) */
  while (logical_page < NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA) {
	 set_ss_pag(child_pt, logical_page, frames[logical_page - (NUM_PAG_KERNEL + NUM_PAG_CODE)]);
	 ++logical_page;
  }
  /* La magia: 
   * 1. Asigno entradas de la TP(current) a los nuevos frames (temporal free entries) 
   * 2. Copio las paginas data+stack del proceso current en estas temporal free entries (es decir, después de NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA)
   * 3. Libero el acceso que tenia el padre a los nuevos frames y como child_tp ya apunta al sitio correcto; he acabado*/
  for (i = 0; i <NUM_PAG_DATA; ++i) {
	  set_ss_pag(curr_pt,  NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA + i, frames[i]);
	  copy_data((void *)((NUM_PAG_KERNEL + NUM_PAG_CODE + i) * PAGE_SIZE), (void *)((NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA + i) * PAGE_SIZE), PAGE_SIZE);
	  del_ss_pag(curr_pt, NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA + i);
  }
  set_cr3(get_DIR(curr_task)); //flush TLB
  
  /* Assign a new PID to the process */  
  child_task->PID = ++PID_;
  PID = child_task->PID;
  
  /* Actualizar el task_switch */
  child_union->stack[KERNEL_STACK_SIZE-19] = 0;
  child_union->stack[KERNEL_STACK_SIZE-18] = (unsigned int)ret_from_fork;
  child_task->kernel_esp = (unsigned long *)&child_union->stack[KERNEL_STACK_SIZE-19];

  /* Insert the new process into the ready queue */
  list_add_tail(&child_task->list, &readyqueue);
  
  return PID;
} 

void sys_exit()
{  
	int i;
	page_table_entry *PT = get_PT(current());
	for (i = 0; i < NUM_PAG_DATA; ++i) {
			int frame = get_frame(PT,PAG_LOG_INIT_DATA+i); //consigo la traducción de mem fisica
			free_frame(frame); //Marco como free el frame
			del_ss_pag(PT,PAG_LOG_INIT_DATA+i); //Elimino la traduccion en la tabla de paginas
	}
	
	list_add_tail(&(current()->list), &freequeue); //Pasa el proceso a estar free
	current()->PID=-1;
	
	sched_next_rr();  //Select the next process to execute
}

int sys_write(int fd, char * buffer, int size) {
	int result = check_fd (fd, ESCRIPTURA);
	char des_buffer[128]; 
	int result2 = 0;
	
	/* Check de fd, buffer y size */
	if (result != 0) return result;
	if (buffer == NULL) return -EFAULT;
	if (size < 0) return -EINVAL;
	
    while (size > 128) {
		copy_from_user(buffer,des_buffer,128);
		result2 += sys_write_console(des_buffer, 128);
		size -= 128;
		buffer += 128;
	}

	copy_from_user(buffer,des_buffer,size);
	result2 += sys_write_console(des_buffer, size);
	
	return result2;
}
 
int sys_gettime() 
{
	return zeos_ticks;
}

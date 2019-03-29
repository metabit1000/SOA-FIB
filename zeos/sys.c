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

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF; 
  if (permissions!=ESCRIPTURA) return -EACCES; 
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
  int PID;
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

  /* Copy the parent’s task_union to the child // new directory */
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
  
  /* CODE */  
  for (i=0; pag < NUM_PAG_CODE; i++) 
    child_pt[PAG_LOG_INIT_CODE + i].entry = curr_pt[PAG_LOG_INIT_CODE + i].entry;
  
  /* DATA+STACK */
   
  return PID;
}

void sys_exit()
{  
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

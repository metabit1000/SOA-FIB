/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>

#define NR_TASKS      10
#define KERNEL_STACK_SIZE	1024

enum state_t { ST_RUN, ST_READY, ST_BLOCKED };

/* DECLARACION READY/FREE QUEUE */
struct list_head readyqueue;
struct list_head freequeue;

struct task_struct *idle_task; //pone que se declare como global

struct task_struct { //PCB
  int PID;			/* Process ID. This MUST be the first field of the struct. */
  unsigned long kernel_esp; //variable para guardar la posicion en la pila donde esta el valor inicial para ebp
  page_table_entry * dir_pages_baseAddr;  //direccion del directorio que contiene la tabla de paginas
  struct list_head list;  //necesario para encolar (a falta de memoria dinamica)
};

union task_union {
  struct task_struct task;  /* PCB, per procés */
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

extern union task_union task[NR_TASKS]; /* Vector de tasques, 10 procesos en zeOS max */


#define KERNEL_ESP(t)       	(DWord) &(t)->stack[KERNEL_STACK_SIZE]

#define INITIAL_ESP       	KERNEL_ESP(&task[1])

/* Inicialitza les dades del proces inicial */
void init_task1(void);

void init_idle(void);

void init_sched(void);

/* Me devuelve la PCB del proceso que se esta actualmente ejecutando */
struct task_struct * current();

void task_switch(union task_union*t);

struct task_struct *list_head_to_task_struct(struct list_head *l);

int allocate_DIR(struct task_struct *t);

page_table_entry * get_PT (struct task_struct *t) ;

page_table_entry * get_DIR (struct task_struct *t) ;

/* Headers for the scheduling policy */
void sched_next_rr();
void update_process_state_rr(struct task_struct *t, struct list_head *dest);
int needs_sched_rr();
void update_sched_data_rr();

#endif  /* __SCHED_H__ */

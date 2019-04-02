/*
 * libc.c 
 */

#include <libc.h>

#include <types.h>

int errno;

void itoa(int a, char *b) //convierte a String un int
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)  //retorna el numero de caracteres en un String
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

void perror(void) //Muestra errores al usuario
{
	char error[200];
	itoa(errno,error);
	write(1,"\n",1);
	if (errno == 0) write(1,"No hay errores por parte del perror\n",36);
	else write(1,error,strlen(error));  //escribo por pantalla el numero de error
}

/* WRAPPERS */

int write (int fd,char* buffer,int size); // version sysenter

/*int write (int fd,char* buffer,int size) //version int (ejemplo)
{
	/*eax = 4, ebx = fd, ecx = *buffer, edx = size 
	int result;
	
	__asm__ __volatile__ (
		"int $0x80\n\t"
		: "=a" (result)
		: "a" (4), "b" (fd), "c" (buffer), "d" (size));
		
	if (result < 0) { 
		errno = -result;
		result -1;
	}
	else return result;
} */

int gettime();
int getpid(void);
int fork(void);
void exit(void);


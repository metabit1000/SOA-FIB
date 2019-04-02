#include <libc.h>

char buff[24];

int pid;

int add2(int par1,int par2);

long inner(long n) {
	int i;
	long suma;
	suma = 0;
	for (i=0; i<n; i++) suma = suma + i;
	return suma;
}

long outer(long n){
	int i;
	long acum;
	acum = 0;
	for(i=0; i<n; i++) acum = acum + inner(i);
	return acum;
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
      /*__asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
     
    long count, acum, suma;
	count = 75;
	acum = 0;
	acum = outer(count);
	suma = add2(3,4);
	write(1,"\n",1);
	write(1,"FUNCIONA EL WRITE\n",18); 
	
	int PID = fork();
	if (PID == 0){
		char buffer[] = "Soy un hijo, con el PID: "; //Porque no me escribe estooo??
		int aux = getpid();
		char res[64];
		itoa(aux,res);
		write(1,buffer,strlen(buffer));
		write(1,res,strlen(res));
		exit();		
	}
	else {	
		char buffer[] = "\nSoy el padre, con el PID: ";
		int aux = getpid();
		char res[64];
		itoa(aux,res);
		write(1,buffer,strlen(buffer));
		write(1,res,strlen(res));
		exit();
	}
	
	perror();
	while(1) {
		//int aux = getpid();
		//char res[64];
		//itoa(aux,res);
		//write(1,res,strlen(res)); 
	}
	return 0;
}

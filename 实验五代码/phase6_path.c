#include<stdio.h>
void f();
extern void (*myprint)();

void (*myprint)() = f;

void f(){
	printf("U202215643\n");
	}

//void (*myprint)() ;

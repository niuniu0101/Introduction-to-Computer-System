#include<stdio.h>
extern void (*myprint)();
void f(){
	printf("U202215631");
}
void(*myprint)()=f;

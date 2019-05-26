#include<stdio.h>


typedef struct _table_entry{
	int x;
	float y;
} table_entry;

table_entry t;
table_entry *p = &t;



void main(void){
	
 	printf("please prodvide me with an INT and a FLOAT\n");
 	scanf("%x %f",&(t.x),&(t.y));
// 	scanf("%x %f",&(p->x),&(p->y)); //equivalent to previous line
// 	scanf("%x %f",&((&t)->x),&((&t)->y)); //equivalent to previous line
 	printf("x is %d - y is %f\n",t.x,t.y);
}

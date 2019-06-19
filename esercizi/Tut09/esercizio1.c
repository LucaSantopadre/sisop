#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


int DIM_POOL = 10;
int page_size;
int N;

struct allocator{
	int bitmap[10];
	void *pointer_first_page;
	void *pointer_first_continuous_free_pages;
	int num_continuous_free_pages;
}al;


struct allocator* init_allocator(void){
	page_size = getpagesize();

	for (int i = 0; i < DIM_POOL; ++i)
	{
		al.bitmap[i]=0;
	}
	al.pointer_first_page = mmap(NULL, DIM_POOL * page_size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
	al.pointer_first_continuous_free_pages = al.pointer_first_page;
	al.num_continuous_free_pages = DIM_POOL;

	return &al;
}


void alloca_pagine(struct allocator *al){
	int trovata=0, indice_bitmap_partenza;


	printf("Quante pagine vuoi allocare?\n");
	scanf("%d",&N);
	

	// cerco primo posto disponibile
	for (int i = 0; i < DIM_POOL; ++i)
	{
		if(al->bitmap[i] == 0 && trovata==0){
		indice_bitmap_partenza=i;
		trovata=1;

			for (int j = i; j< DIM_POOL; ++j)
			{
				if(al->bitmap[j] == 0){
					al->num_continuous_free_pages = al->num_continuous_free_pages + 1;
				}
				else break;
			}
		}
	}


	if(trovata){
		printf("TROVATA regione contigua di %d pagine\n",N);
		printf("inizio mappa: %d\n",indice_bitmap_partenza);

		for (int i = indice_bitmap_partenza; i < indice_bitmap_partenza + N; ++i)
		{
			al->bitmap[i] = 1;
		}
		printf("bitmap:");
		for (int i = 0; i < DIM_POOL; ++i)
		{
			printf("%d",al->bitmap[i]);
		}
		printf("\n");
	}


}


void dealloca_pagine(struct allocator *al){
	int trovata=0, indice_bitmap_partenza;

	printf("Quante pagine vuoi deallocare?\n");
	scanf("%d",&N);


}


void stampa_bitmap(struct allocator *al){
	printf("bitmap:");
	for (int i = 0; i < DIM_POOL; ++i)
	{
			printf("%d",al->bitmap[i]);
	}
	printf("\n");
}




int main(int argc, char* argv[]){

	struct allocator *pointer_al = init_allocator();
	printf("inizializzato allocatore\nbitmap: ");
	stampa_bitmap(pointer_al);
	printf("indirizzo pool: %p\n\n",pointer_al->pointer_first_page);

	int choice;




	while(1){
		printf("[0] per allocare nuove pagine\n");
		printf("[1] per deallocare pagine\n");
		printf("[2] stampa bitmap\n");
		scanf("%d",&choice);

		if(choice==0){
			alloca_pagine(pointer_al);

		}
		if(choice==1){
			dealloca_pagine(pointer_al);

		}
		if(choice==2){
			stampa_bitmap(pointer_al);
		}

		printf("\n\n\n\n");
	}
}

#include <stdio.h>
#include "mymalloc.h"
struct Node{
	int data;
	long long kek;
};
int main()
{
	struct Node *curr =(struct Node *) my_buddy_malloc(sizeof(struct Node));
//	dump_heap();
	curr->data = 1;
	curr->kek = 1000;
//	dump_heap();
	my_free(curr);
	dump_heap();
//	printf("\n%p\n", ptr);
/*	ptr = my_buddy_malloc(100);
	dump_heap();
	printf("\nthird alloc: \n");
	ptr = my_buddy_malloc(100);
	dump_heap();
	printf("\n\n");
	ptr = my_buddy_malloc(30);
	dump_heap();
	printf("\n\n");
	ptr = my_buddy_malloc(30);
	dump_heap();*/
	return 0;
}

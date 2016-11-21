#include <stdio.h>
#include "mymalloc.h"
struct Node{
	int data;
	long long data2;
};
int main()
{
	printf("\nheap before my_buddy_malloc() -- should be empty\n");	
	dump_heap();	
	struct Node *curr =(struct Node *) my_buddy_malloc(sizeof(struct Node));
	printf("\nheap after my_buddy_malloc() - [30] should be empty and everything else have one node\n");
	dump_heap();
	curr->data = 1;
	curr->data2 = 2;
	my_free(curr);
	printf("\nheap should be empty\n");
	dump_heap();
	return 0;
}

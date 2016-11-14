#include <stdio.h>

int main()
{
	void *ptr = my_buddy_malloc(256);
	dump_heap();
	printf("\nsecond alloc: \n");
	ptr = my_buddy_malloc(256);
	dump_heap();
	return 0;
}

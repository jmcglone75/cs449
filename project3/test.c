#include <stdio.h>

int main()
{
	void *ptr = my_buddy_malloc(500);
	dump_heap();
	return 0;
}

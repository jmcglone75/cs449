#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#define MAX_MEM 1<<30
#include "mymalloc.h"

static void *base = NULL;
static struct Block *free_list[26];





void* my_buddy_malloc(int size)
{
	if(base == NULL)
	{
		base = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
		free_list[25] = (struct Block *) base;
		free_list[25]->header = 30 << 1; //stores a log2 of the usable size in the 7 leftmost bits, shifts right to make the 0th bit a 0 (because its empty).
	}
	assert(base != NULL);

	int closest_block_size = pow(2, (ceil(log(size+1)/log(2)))); //calculate closest power of 2 block size

	if (closest_block_size < 32)
	{
		closest_block_size = 32; //can't allocate a smaller block size than 32, (e.g. 16 or 8), so make it 32 even if its < 32.
	}


	int index_freelist = (log(closest_block_size)/log(2)) - 5; //calculate the index of our free list for the block size
	struct Block *return_block = NULL;
	int index = index_freelist;	
	while(index_freelist <= 25 && return_block == NULL)
	{
		if(free_list[index_freelist] != NULL)
		{
			//call split
			return_block = split(index, free_list[index_freelist]);
			return_block->header = return_block->header | 1;  //set occ bit to 1 -- meaning occupied	
		}
		else
		{
			index_freelist++;
		}
	}
	return ((char *)return_block) + 1;

}

void my_free(void *ptr)
{
	assert(base != NULL);

	struct Block *block = ((char *)ptr) - 1; //gives pointer to header of block
	struct Block *buddy;


	int size = (block->header >> 1);
	if(block->header & 1)
	{
		block->header = block->header & ~((char) 1);
		if (free_list[size-5] == NULL)
		{
			block->next = NULL;
			block->prev = NULL;
			free_list[size - 5] = block;
		}
		else
		{
			free_list[size - 5]->prev = block;
			block->next = free_list[size - 5];
			free_list[size - 5] = block;
			block->prev = NULL;
		}
	}
	//finds buddy using xor
	buddy = (((char *)block - (char *)base) ^ (1 << size)) + ((char *)base);

	//base case - buddy is occupied or the size is 30
	if (buddy->header & 1 || size == 30) 		
	{
		return;
	}
	else //not occupied -- coalesce
	{
		//remove block
		if(block == free_list[size-5])
		{
			free_list[size-5] = block->next;
			if(free_list[size - 5] != NULL)
			{
				free_list[size-5]->prev = NULL;
			}
		}
		else if(block->next == NULL)
		{
			block->prev->next = NULL;
		}
		else
		{
			block->prev->next = block->next;
			block->next->prev = block->prev;
		}
			
		if(buddy == free_list[size-5])
		{
			free_list[size-5] = buddy->next;
			if(free_list[size-5] != NULL)
			{
				free_list[size-5]->prev = NULL;
			}
		}
		else if(buddy->next == NULL && buddy->prev != NULL)
		{
			buddy->prev->next = NULL;
		}
		else
		{
			buddy->prev->next = buddy->next;
			buddy->next->prev = buddy->prev;
		}

		block->next = NULL;
		block->prev = NULL;
		buddy->next = NULL;
		buddy->prev = NULL;

		if (block < buddy)
		{
			block->header = ((size + 1) << 1);
		}
		else
		{
			buddy->header = ((size + 1) << 1);
			block = buddy;
		}		
		if (free_list[size-5+1] == NULL)
		{
			block->next == NULL;
			free_list[size-5+1] = block;
		}
		else
		{
			free_list[size-5+1]->prev = block;
			block->next = free_list[size-5+1];
			free_list[size-5+1] = block;
		}

		my_free(((char *)block) + 1);
		
	}

}

void *split(int target_size, struct Block *block)
{
	int block_size = (block->header >> 1) - 5;
	//remove current block from free_list
	if(free_list[block_size]->next == NULL) //only block in freelist
	{
		free_list[block_size] = NULL;
	}
	else //more than one block in free list.  remove first one and make head of list the next one
	{			
		free_list[block_size] = free_list[block_size]->next;
		free_list[block_size]->prev = NULL;
	}
	


	if (block_size == target_size) //checks to see if size in freelist is same as size needed for allocation
	{
		block->prev = NULL;
		block->next = NULL;
		return block;
	}
	else
	{
		
		struct Block *buddy = (struct Block *) (((char *)block) + (1 << (block_size + 5 - 1)));
		block->header = (block_size + 5 - 1) << 1;
		buddy->header = (block_size + 5 - 1) << 1;

		if(free_list[block_size - 1] == NULL) //add block and buddy to free list. block->buddy
		{
			block->next = buddy;
			buddy->prev = block;
			free_list[block_size - 1] = block;
		}
		else //add both block and buddy to the beggining of free list.  block->buddy->rest of free_list
		{
			block->next = buddy;
			buddy->prev = block;
			buddy->next = free_list[block_size - 1];
			free_list[block_size - 1]->prev = buddy;
			free_list[block_size - 1] = block;
		}
		return split(target_size, block);

	}

}

void dump_heap()
{
	int i = 0;
	for (i; i <= 25; i++)
	{
		struct Block *curr = free_list[i];
		printf("%d->", i+5);
		while(curr != NULL)
		{
			//print node
			printf("[%d : %d : %.0f]->", (curr->header & 1), ((char *)curr - (char *)base), pow(2, (curr->header >> 1)));
			curr = curr->next;
		}
		printf("NULL\n");
	}
}




















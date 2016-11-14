#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#define MAX_MEM 1<<30

static void *base = NULL;
static struct Block *free_list[26];

void *split(int size, struct Block *block);
void dump_heap();
void *my_buddy_mallloc(int size);

struct Block
{
	unsigned char header;
	struct Block *next;
	struct Block *prev;
};

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
	return ((char *) return_block) + 1;

 return NULL;

}

void my_free(void *ptr)
{
	assert(base != NULL);
}

void *split(int target_size, struct Block *block)
{
	int block_size = (block->header >> 1) - 5;
	//remove current block from free_list
	if(free_list[block_size]->next == NULL)
	{
		free_list[block_size] = NULL;
	}
	else
	{			
		free_list[block_size] = free_list[block_size]->next;
		free_list[block_size]->prev = NULL;
	}

	if (block_size == target_size) //checks to see if size in freelist is same as size needed for allocation
	{
		return block;
	}
	else
	{
		//probably not working 
     		//struct Block *buddy = (void *)((uintptr_t) block ^ (uintptr_t)pow(2, block_size+5)); //gives you pointer to buddy in middle
		
		struct Block *buddy = (struct Block *) (((char *)block) + (1 << (block_size + 5 - 1)));
		block->header = (block_size + 5 - 1) << 1;
		buddy->header = (block_size + 5 - 1) << 1;
		struct Block *front = free_list[block_size - 1];
		if(front == NULL) //add block and buddy to free list. block->buddy
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
			printf("[%d:%d:%f]->", (curr->header & 1), ((char *)curr - (char *)base), pow(2, (curr->header >> 1)));
			curr = curr->next;
		}
		printf("NULL\n");
	}
}




















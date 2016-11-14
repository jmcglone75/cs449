#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

struct Block {
  int occ;              // whether block is occupied
  int size;             // size of block (including header)
  struct Block *prev;   // pointer to previous block
  struct Block *next;   // pointer to next block
};

static struct Block *head = NULL;       // head of list

/** @brief Bump up program break by the allocation size
 *
 *  Monotonically increase the heap at every allocation.
 *  In this heap, there are no data structures to mark the
 *  used and unused portions of the heap.
 *
 *  @param size Allocation size in bytes.
 *  @return Pointer to the allocated block of memory.
 */
void *my_malloc1(int size)
{
  
  return (void*)sbrk(size);
}

/** @brief Bump up program break by the allocation size + block header size
 *
 *  Same as malloc1 except that now you also allocate space
 *  for the block header.  The structure of the block header
 *  is given in the struct Block type.  The function should
 *  return the location after the header, which is the usable
 *  part of the block.
 *
 *  @param size Allocation size in bytes.
 *  @return Pointer to the usable part of allocated memory.
 */
void *my_malloc2(int size)
{
  sbrk(sizeof(struct Block));
  return (void *) sbrk(size);
}

/** @brief Bump up program break by the allocation size + block header size
 *         and fill in the block header fields
 *
 *  Now blocks are added to the double-linked list which starts
 *  from 'head'.  When a new block is added, the occ bit needs to
 *  be set to 1, size needs to be set to the block size, and the
 *  link pointers need to be updated such that it is double-linked
 *  to the previous block.
 *
 *  @param size Allocation size in bytes.
 *  @return Pointer to the usable part of allocated memory.
 */
void *my_malloc3(int size)
{
  static struct Block *curr;
  if (head == NULL)
  {
    head = sbrk(sizeof(struct Block));
    head->occ = 1;
    head->size = size + sizeof(struct Block);
    curr = head;
  }
  else
  {
   struct Block *temp = sbrk(sizeof(struct Block));
   temp->occ = 1;
   temp->size = size + sizeof(struct Block);

   curr->next = temp;
   temp->prev = curr;
   curr = temp;
  }

  return sbrk(size);
}

/** @brief Mark the block free by unsetting the occ bit
 *
 *  In a real heap the freed block would be reused but
 *  this simple heap always allocats from the top of the
 *  heap by increasing heap space.
 *
 *  @param data Pointer to area being freed by user.
 *  @return Void.
 */
void my_free(void *data)
{
  struct Block *cur = data - sizeof(struct Block);
  cur->occ = 0;
}

/** @brief Dump the contents of the heap.
 *
 *  Traverse the heap starting from the head of the list and print
 *  each block.  While traversing, check the integrity of the heap
 *  through various assertions.
 *
 *  @return Void.
 */
void dump_heap()
{
  struct Block *cur;
  printf("head->");
  for(cur = head; cur != NULL; cur = cur->next) {
    printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
    if(cur->next != NULL) assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
  }
  printf("NULL\n");
}

struct Block {
  unsigned char header;
  struct Block *prev;
  struct Block *next;
};

void *split(int target_size, struct Block *block);
void *my_buddy_malloc(int size);
void my_free(void *ptr);
void dump_heap();

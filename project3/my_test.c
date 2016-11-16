#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
char *alloc1();
char *alloc2();
char *alloc3();
struct Fuck{
  struct Fuck *next;
  struct Fuck *prev;
};
void main(){
  struct Fuck *base = my_buddy_malloc(sizeof(struct Fuck));
  int i = 0;
  for(i; i < 100; i++){
    struct Fuck *next = my_buddy_malloc(sizeof(struct Fuck));
    next->next = base;
    base->prev = next;
    base = next;
  }
  dump_heap();
  while(base != NULL){
    struct Fuck *next = base->next;
    my_free(base);
    base = next;
  }
  dump_heap();
}

char *alloc1(){
  char *str = (char *)my_buddy_malloc(15);
  return str;
}

char *alloc2(){
  char *str = (char *)my_buddy_malloc(100);
  return str;
}

char *alloc3(){
  char *str = (char *)my_buddy_malloc(100);
  return str;
}

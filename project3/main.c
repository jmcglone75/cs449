#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//replace malloc here with the appropriate version of my_malloc
#define MALLOC(n) my_malloc3(n)
//replace free here with the appropriate version of my_free
#define FREE(n)  my_free(n)
//define DUMP_HEAP() to be dump_heap() when you are done
#define DUMP_HEAP() dump_heap()

int main()
{
  char *this = MALLOC(5);
  char *is = MALLOC(3);
  char *a = MALLOC(2);
  char *test = MALLOC(5);
  char *program = MALLOC(8);
  DUMP_HEAP();

  strcpy(this, "this");
  strcpy(is, "is");
  strcpy(a, "a");
  strcpy(test, "test");
  strcpy(program, "program");
  printf("%s %s %s %s %s\n", this, is, a, test, program);
  DUMP_HEAP();

  FREE(is);
  FREE(test);
  printf("%s %s %s %s %s\n", this, "*", a, "*", program);
  DUMP_HEAP();

  FREE(this);
  FREE(a);
  FREE(program);
  printf("%s %s %s %s %s\n", "*", "*", "*", "*", "*");
  DUMP_HEAP();

  return 0;
}


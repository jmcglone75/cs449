Gerard McGlone - Project 3
Peoplesoft: 4007177

-The project compiles ("gcc mymalloc.c mallocdrv.c -lm")

-It succesfully allocates and frees/coalesces a 32 byte block, 
which is being done in test.c .  I put in a dump_heap() method before
the my_buddy_malloc(), after the alloc, and then after the free to show
that it does this correctly.  When testing I put the dump_head() method
in my_free() to show the coalescing at each step.  This ensured me it was working

-For test1 in mallocdrv.c, it succesfully allocates as I have the numbers
1-100, but it segfaults when I free and I was not able to figure this out.

- Therefore, half of test1 works.  To run test2 you will have to comment out test1
because of the segfault

-For test2, it both succesfully allocates and frees.  I get an ordered list of numbers 
that fall in the range 1-100 with some repeats(and not every number is represented but 
his assig sheet made it seem like that wasnt supposed to happen anyways).  I put dump_heap()
in my recursive my_free() method when testing to make sure it was succesfully coalescing, 
and it was.  

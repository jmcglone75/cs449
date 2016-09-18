//Gerard McGlone - CS449 Lab2b
//Recitation Number:  11657

#include <stdio.h>
#include <stdlib.h>

int* search(int* begin, int* end, int needle);

int main(int argc, char **argv)
{
	int num;
	int nums[10], i;
	int *found = NULL;
	
	if (argc != 2)
	{
		printf("Usage: search <number>\n");
		return 1;
	}
	
	num = atoi(argv[1]);
	for(i = 0; i < 10; i++)
	{
		nums[i] = 1 << i;
	}
	
	found = search(nums, &nums[9], num);
	if (found)
	{
		printf("Number %d found in index %d.\n", num, (found-nums));
	}
	else
	{
		printf("Number %d was not found.\n", num);
	}
	
	return 0;
}

//[args]
//begin: pointer to beggining of array
//end: pointer to end of array
//needle:  value that is being searched for
//[return value]
//if not found: NULL
//if found: pointer to element of array containing needle value

int* search(int* begin, int* end, int needle)
{
	//implement properly
	int index = (end - begin)/2;
	
	if (begin > end) 
	{
		return NULL;
	}
	else if (needle == *(begin+index)) //base case:  item found! 
	{
		return (begin+index);
	}
	else if (needle < *(begin + index)) //recursive case:  needle is less than middle item
	{
		//return left half of array
		return search(begin, (begin+index-1), needle);
	}
	else if (needle > *(begin + index)) //recursive case:  needle is greater than middle item
	{
		//return right half of array
		return search((begin+index+1), end, needle);
	}
}
















//Gerard McGlone - Lab3
#include <stdio.h>
#include <stdlib.h>

struct Node
{
	int grade;
	struct Node *next;
};

int main()
{
	int data;
	int sum = 0;
	int count = 0;
	struct Node *head = NULL;
	struct Node *curr = NULL;
	printf("\nEnter grades one at a time.  enter -1 when you're done: \n");
	scanf("%d", &data);

	head = (struct Node *) malloc(sizeof(struct Node));
	head->grade = data;
	curr = head;
	count++;
	
	while(data != -1)
	{
		scanf("%d", &data);
		if(data == -1)
			break;

		struct Node *temp;
		temp = (struct Node *) malloc(sizeof(struct Node));
		temp->grade = data;
		curr->next = temp;
		curr = temp;
		count++;
	}

	curr = head;
	while(curr != NULL)
	{
	//	printf("%d\n", curr->grade);
		sum += curr->grade;
		struct Node *temp = curr;
		curr = curr->next;
		free(temp);
	}
	printf("\nAverage Grade:  %.2f\n", ((float) sum)/count);

	return 0;
}

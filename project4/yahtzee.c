/*Gerard McGlone
cs449 Project 4 - /dev/dice
peoplesoft: 4007177*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

//srand((unsigned int)time(NULL));
int rolls[5]; //array holding 5 rolls of dice
int upper[6]; //array holding each of the 6 sections in the upper section
int lower[7]; //array holding each of the 7 sections in the lower section


int cmpfunc(const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}

int main()
{
	srand((unsigned int)time(NULL));

	int i;
	for(i = 0; i < 13; i++)
	{
		printf("\nYour Roll: \n");
		int j;
		for(j = 0; j < 5; j++)
		{
			rolls[j] = roll();
			printf("%d  ", rolls[j]);
		}
		printf("\n");

		re_roll();
		re_roll();
		printf("\n------Final Dice total----\n");
		qsort(rolls, 5, sizeof(int), cmpfunc);
		for(j = 0; j < 5; j++)
		{
			printf("%d  ", rolls[j]);
		}

		assign_score();
		i = 13;
	}

		
	return 0;
}


//returns random number from 1-6
int roll()
{
	//change to dice from driver
	return (rand()%6) + 1;
}

//allows player to re-roll any of the five dice and changes the values accordingly
void re_roll()
{
	char response[11];
	int reroll[5] = {0, 0, 0, 0, 0};

	printf("\nWhich dice would you like to re-roll(***0 for NO re-roll)?  ");
	fgets(response, sizeof(response), stdin);

	char *token = strtok(response, " ");
	if(atoi(token) != 0)
	{
		while(token != NULL)
		{
			int value = atoi(token);
			rolls[value-1] = roll();
			token = strtok(NULL, " ");
		}
	}
	int i = 0;
	for(i; i < 5; i++)
	{
		printf("%d  ", rolls[i]);
	}
	printf("\n");
}

//player chooses upper/lower section
//player then picks the sub-section they would like
//this method will calculate the score appropriatley for whichever subsection is chosen
void assign_score()
{
	//ask for section
	int response; 
	printf("\n\nPlace Dice into:\n1) Upper Section\n2) Lower Section\n");
	scanf("%d", &response);
	if(response == 1)
	{
		//upper
	}
	else
	{
		//lower
	}

	//ask for subsection (make sure it hasn't been filled - use while loop)
	//assign score
}

//shows upper section, lower section, and upper section bonus
void show_score()
{

}


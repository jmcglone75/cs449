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
		for(j = 0; j < 6; j++)
		{
			printf("%d  ", upper[j]);
		}
//		i = 13;
		printf("\n");
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

	printf("\nWhich dice would you like to re-roll(***0 for NO re-roll)?  \n");
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
	printf("\nSelection? ");
	scanf("%d", &response);
	if(response == 1)
	{
		//upper
		int upper_category;
		int right_category = 0;
		while(!right_category) //checks to make sure category hasn't been scored in
		{
			printf("\n\nPlace Dice into:\n1) Ones\n2) Twos\n3) Threes\n4) Fours\n5) Fives\n6) Sixes\n");
			printf("\nSelection? ");
			scanf("%d", &response);
		
			upper_category = response - 1;
			if(upper[upper_category] == 0) //0 means category has not been scored in 
			{
				right_category = 1; //break out of loop
			}
		}

		if(upper_category == 0) //ones
		{	
			int sum = 0;	
			int i;
			for(i = 0; i < 5; i++)
			{
				if (rolls[i] == 1)
					sum = sum + rolls[i];
			}
			upper[upper_category] = sum;
		}
		else if(upper_category == 1) //twos
		{
			int sum = 0;	
			int i;
			for(i = 0; i < 5; i++)
			{
				if (rolls[i] == 2)
					sum = sum + rolls[i];
			}
			upper[upper_category] = sum;
		}
		else if(upper_category == 2) //threes
		{
			int sum = 0;	
			int i;
			for(i = 0; i < 5; i++)
			{
				if (rolls[i] == 3)
					sum = sum + rolls[i];
			}
			upper[upper_category] = sum;
		}
		else if(upper_category == 3)//fours
		{
			int sum = 0;	
			int i;
			for(i = 0; i < 5; i++)
			{
				if (rolls[i] == 4)
					sum = sum + rolls[i];
			}
			upper[upper_category] = sum;
		}
		else if(upper_category == 4)//fives
		{
			int sum = 0;	
			int i;
			for(i = 0; i < 5; i++)
			{
				if (rolls[i] == 5)
					sum = sum + rolls[i];
			}
			upper[upper_category] = sum;
		}
		else//sixes
		{
			int sum = 0;	
			int i;
			for(i = 0; i < 5; i++)
			{
				if (rolls[i] == 6)
					sum = sum + rolls[i];
			}
			upper[upper_category] = sum;
		}

		while(getchar() != '\n'); //flushes stdin - scanf() does not consume new line char which messes up the next call to fgets()
	}
	else
	{
		//lower
		int lower_category;
		int right_category = 0;
		while(!right_category)
		{
			printf("\n\nPlace Dice into:\n1) Three of a Kind\n2) Four of a Kind\n3) Small Straight\n4) Large Straight\n5) Full House\n6) Yahtzee\n7) Chance\n");
			printf("\nSelection? ");
			scanf("%d", &response);

			lower_category = response - 1;
			if(lower[lower_category] == 0)
			{
				right_category = 1;
			}
		}

		if(lower_category == 0) //three of a kind
		{
			
		}
	}

	//ask for subsection (make sure it hasn't been filled - use while loop)
	//assign score
}

//shows upper section, lower section, and upper section bonus
void show_score()
{

}












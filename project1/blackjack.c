//Gerard McGlone
//CS 449 - Project 1

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main()
{
	/*Random Number Generator Info:
	* there are 13 total groups of cards, so we will generate nums (inclusivley) between 2 and 14:
	* if value == 2-9, card has that same value
	*else if value == 10, 11, 12, 13: card has value 10
	*else if value == 14, card is an Ace and will be adjusted to 1 or 11 appropriatley
	*/
	srand((unsigned	int)time(NULL));
	int value = rand() % (14 - 2 + 1) + 2;
	//printf("%d\n", value);
	
	int dealerSum = 0, playerSum = 0, dealerCardValue, playerCardValue;
	int dealerAce = 0, playerAce = 0;
	int playersTurn = 1, dealersTurn = 1;
	char response[20];  //character array to hold "hit" or "stand"
	char *hit = "hit";

	//initialize player and dealer hands

	//get dealer cards
	printf("The Dealer:\n");
	int i = 0;
	for(i; i < 2; i++)
	{
		int dealerCard = rand() % (13) + 2;
		if (dealerCard >= 2 && dealerCard <= 9) //cards 2-9
		{
			dealerSum += dealerCard;
			dealerCardValue = dealerCard;
		}
		else if(dealerCard >= 10 && dealerCard <=13) //face card or 10
		{
			dealerSum += 10;
			dealerCardValue = 10;
		}
		else //ace
		{
			dealerSum += 11;
			dealerAce = 1;
			dealerCardValue = 11;
		}
	}
	printf("? + %d\n", dealerCardValue);
	//printf("%d\n", dealerSum);	
	
	//get player cards
	printf("\nThe Player:\n");
	i = 0;	
	for(i; i < 2; i++)
	{
		int playerCard = rand() % (13) + 2;
		if (playerCard >= 2 && playerCard <= 9) //cards 2-9
		{
			playerSum += playerCard;
			playerCardValue = playerCard;
		}
		else if (playerCard >= 10 && playerCard <= 13) //face card or 10
		{
			playerSum += 10;
			playerCardValue = 10;
		}
		else //ace
		{
			playerSum += 11;
			playerAce = 1;
			playerCardValue = 11;
		}
		if (i == 0)
		{
			printf("%d", playerCardValue);
		}
	}
	printf(" + %d = %d\n", playerCardValue, playerSum);
	//printf("%d\n", playerSum);
	if (playerSum == 21)
	{
		printf("BLACKJACK\n");
		playersTurn = 0;
	}

	//players turn loop
	while(playersTurn)
	{
		printf("Would you like to \"hit\" or \"stand\"?  ");
		scanf("%s", &response);
		printf("\n\n");
		//printf("%s\n", response);
		if (strcmp(hit, response) == 0) //player chose hit
		{
			//playersTurn = 0;
			int playerCard = rand() % (13) + 2;
			if (playerCard >= 2 && playerCard <= 9)
			{
				if((playerSum + playerCard > 21) && playerAce) //check for bust with ace
				{
					playerSum -= 10; //change ace value from 11 to 1 to prevent bust
					printf("Player:\n");
					printf("%d + %d = %d\n", playerSum, playerCard, playerSum+playerCard);
					playerSum += playerCard;
					playerAce = 0; //even though the ace is still there, it can only be valued at 1 anymore, so we dont need to worry about it
				}
				else
				{
					printf("Player: \n");
					printf("%d + %d = %d\n", playerSum, playerCard, playerSum+playerCard);
					playerSum += playerCard;

				}
			}
			else if (playerCard >=10 && playerCard <= 13)
			{
				if((playerSum + 10 > 21) && playerAce)
				{
					playerSum -= 10; //change ace value from 11 to 1 to prevent bust
					printf("Player:\n");
					printf("%d + 10 = %d\n", playerSum, playerSum+10);
					playerSum += 10;
					playerAce = 0; 
				}
				else{
					printf("Player:\n");
					printf("%d + 10 = %d\n", playerSum, playerSum+10);
					playerSum += 10;
				}
			}
			else //ace
			{
				if (playerSum + 11 > 21) //11 valued ace would make bust, change to 1
				{
					printf("Player:\n");
					printf("%d + 1 = %d\n", playerSum, playerSum+1);
					playerSum++;
				}
				else
				{
					printf("Player:\n");
					printf("%d + 11 = %d\n", playerSum, playerSum+11);
					playerSum += 11;
					playerAce = 1;
				}
			}
			if (playerSum > 21)
			{
				printf("BUSTED\n");
				playersTurn = 0;
				dealersTurn = 0;
				printf("\nYOU LOSE\n");
			}
			else if (playerSum == 21)
			{
				printf("BLACKJACK\n");
				playersTurn = 0;
			}
		}
		else //player chose stand (stand will also be defaulty seelected if they mis-enter their response)
		{
			printf("Player Total After Stand: %d\n", playerSum);
			playersTurn = 0;
		}
	}

	while(dealersTurn)
	{
		printf("\nDealer Total:  %d\n", dealerSum);
		if (dealerSum < 17)
		{
			int dealerCard = rand() % (13) + 2;
			if (dealerCard >= 2 && dealerCard <= 9)
			{
				if ((dealerSum + dealerCard > 21) && dealerAce)
				{
					dealerSum -= 10;
					printf("Dealer:\n");
					printf("%d + %d = %d\n", dealerSum, dealerCard, dealerSum+dealerCard);
					dealerSum += dealerCard;
					dealerAce = 0;
				}
				else
				{
					printf("Dealer:\n");
					printf("%d + %d = %d\n", dealerSum, dealerCard, dealerSum+dealerCard);
					dealerSum += dealerCard;
				}
			}
			else if (dealerCard >=10 && dealerCard <= 13)
			{
				if((dealerSum + 10 > 21) && dealerAce)
				{
					dealerSum -= 10; //change ace value from 11 to 1 to prevent bust
					printf("Dealer:\n");
					printf("%d + 10 = %d\n", dealerSum, dealerSum+10);
					dealerSum += 10;
					dealerAce = 0; 
				}
				else{
					printf("Dealer:\n");
					printf("%d + 10 = %d\n", dealerSum, dealerSum+10);
					dealerSum += 10;
				}
			}
			else //ace
			{
				if (dealerSum + 11 > 21) //11 valued ace would make bust, change to 1
				{
					printf("Dealer:\n");
					printf("%d + 1 = %d\n", dealerSum, dealerSum+1);
					dealerSum++;
				}
				else
				{
					printf("Dealer:\n");
					printf("%d + 11 = %d\n", dealerSum, dealerSum+11);
					dealerSum += 11;
					dealerAce = 1;
				}
			}
			if (dealerSum > 21)
			{
				printf("DEALER BUSTED\n");
				dealersTurn = 0;
				printf("\nYOU WIN\n");
			}
			else if (dealerSum == 21)
			{
				printf("\nDEALER HAS BLACKJACK\n");
				dealersTurn = 1;
			}
		
		}
		else
		{
			printf("Dealer STANDS\n\n");
			printf("Dealer Has:  %d\n", dealerSum);
			printf("Player Has:  %d\n", playerSum);
			if(playerSum > dealerSum)
			{
				printf("\nYOU WIN!!!!\n");
			}
			else if (dealerSum > playerSum)
			{
				printf("\nYOU LOSE\n");
			}
			else
			{
				printf("\nPUSH\n");
			}
			dealersTurn = 0;
		}
	}


	return 0;	
}

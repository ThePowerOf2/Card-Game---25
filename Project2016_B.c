// Corey Ryan.
// 5/12/16.
// Part B - Application of a game of 25 against the computer.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CONSTANTS.h"

// Declaring functions for the pre-processor.
void shuffle();
void printDeck(); // Only used for debugging the program.
void createDeck();
int validMove(int);
void firstAce(int*);
int computersMove();
void assignStrength();
void makeCardName(int,char);
void printHand(int,int,int);
int checkWinner(int,int,int);
void assignOwner(int,int,int);

// Declaring structure for a card.
struct card{
     char suit;
     int value;
     int strength;
     int owner;
};

// Declaring an array of 52 cards to make the deck.
struct card deck[52];

// Declaring Global variables.
int i,j; // Counters.
int tmpValue; // Tempory variable for the value of a card.
int tmpStrength; // Tempory variable for the strength of a card.
char tmpSuit; // Tempory variable for the suit of a card.
int rndm;

int main(){
    
    // Assigning suits and values to all of the cards in the deck.
    // And then shuffles the deck.
    createDeck();
    shuffle();

    // Declaring local variables.
    int toComputer,toPlayer; // Represents how many cards initially are dealt to the player and the computer.
    int dealingTurn,currentTurn; // 0 Represents the players turn. 1 Represents the computers turn.
    int playersChoice,computersChoice,cutDepth,winner;
    int computerGamesWon = 0;
    int playerGamesWon = 0;
    int computerScore = 0;  
    int playerScore = 0;
    int handsThrown = 0;
    char shuffleChoice;
    
    
    // Printing a welcome message
    printf("-----------------------------\n");
    printf("| Welcome to my game of 25! |\n");
    printf("-----------------------------\n");
    
    // Showing the user who got first ace for deal.
    printf("It's first ace for deal. You want to draw the first card?(Y or N): ");
    scanf(" %c",&shuffleChoice);
    
    // Setting dealing choice based on the players input.
    if((shuffleChoice == 'Y') || (shuffleChoice == 'y')){
         dealingTurn = 0;
    }
    else if((shuffleChoice == 'N') || (shuffleChoice == 'n')){
         dealingTurn = 1;
    }
    
    // Choosing who goes first using firstAce.
    firstAce(&dealingTurn);
    
    if(dealingTurn == 0){        
         printf("You got the first Ace.\n");
         currentTurn = 1;
    }
    else if(dealingTurn == 1){
         printf("Computer got the first Ace.\n");
         currentTurn = 0;
    }
    
    // The game goes on until someone has won RUBBER(3) games.
    while((playerGamesWon < RUBBER) && (computerGamesWon < RUBBER)){
                            
    // Shuffling the deck every turn.              
    shuffle();
        
         // Dealing as the computer.
         if(dealingTurn == 1){
              printf("-----------------------------\n");
              printf("|      Computers Deal!      |\n");
              printf("-----------------------------\n");
              printf("Cut or run? (C or R): ");
              scanf(" %c",&shuffleChoice);
              
              // Cutting the deck if the user decides to cut them.
              if((shuffleChoice == 'C') || (shuffleChoice == 'c')){
                   printf("How far down the deck do you want to cut? (1-52): ");
                   scanf(" %d", &cutDepth);
                   
                   // Cutting the deck.
                   for(i=0;i<(cutDepth);i++){
                        tmpValue = deck[0].value;
                        tmpSuit = deck[0].suit;
                        tmpStrength = deck[0].strength;
                        for(j=0;j<51;j++){
                             deck[j].value = deck[j+1].value;
                             deck[51].value= tmpValue;
                        }
                   }  
              }
              
              // Computer always does a 'County Home' Deal.
              toPlayer = 4;
              toComputer = 4;
              currentTurn = 0;
         }
         // Dealing as the player.
         else if(dealingTurn == 0){
              
              printf("-----------------------------\n");
              printf("|      Its Your Deal!       |\n");
              printf("-----------------------------\n");
              
              printf("\nThe Computer chooses to run the Cards\n");
              
              // Asking the player how they want to deal the cards.
              printf("\nHow many cards would you like to deal the Computer? (1-4): ");
              scanf(" %d",&toComputer);
              printf("How many cards would you like to deal to yourself? (1-4): ");
              scanf(" %d",&toPlayer);
              currentTurn = 1;
         }
         // Assigning strengths and owners to both hands of cards.
         assignStrength();
         assignOwner(toPlayer,toComputer,dealingTurn);
         
         // Continute till all cards are used or someone wins the current game.
         while(((playerScore < WIN) && (computerScore < WIN)) && ((handsThrown < 5) && (handsThrown < 5))){
              // Printing out the current Trump.
              printf("\nTrump = ");
              makeCardName(deck[10].value,deck[10].suit);
       
              if(currentTurn == 1){
                        
                   // Printing out the computers choice.
                   computersMove();
                   computersChoice = computersMove();
                   printf("\nComputer Chooses : ");
                   makeCardName(deck[computersChoice].value,deck[computersChoice].suit);
                   printf("\n");
              
                   // Asking the user to choose a card.
                   // Keeps repeating until they choose a valid card.
                   do{
                        printHand(toPlayer,toComputer,dealingTurn);
                        printf("\nPlease select a card to use: ");
                        scanf(" %d",&playersChoice);
                        j = 0;
                        for(i=0;i<10;i++){
                             if(deck[i].owner == 1){
                                  j++;
                             }
                             if(j == playersChoice){
                                  playersChoice = i;
                                  break;
                             }
                        }                    
                   }while (validMove(playersChoice) == 0);
              
              }
              else{
                   // Asking the user to choose a card.
                   // Keeps repeating until they choose a valid card.
                   do{
                        printHand(toPlayer,toComputer,dealingTurn);
                        printf("Please select a card to use: ");
                        scanf(" %d",&playersChoice);
                        j=0;
                        for(i=0;i<10;i++){

                             if(deck[i].owner == 1){
                                  j++;
                             }
                             if(playersChoice == j){
                                  playersChoice = i;
                                  break;
                             }
                        }
                   }while (validMove(playersChoice) == 0);
                   
                   // Printing out the computers choice.
                   computersMove();
                   computersChoice = computersMove();
                   printf("\nComputer Chooses : ");
                   makeCardName(deck[computersChoice].value,deck[computersChoice].suit);
                   printf("\n");
              }

              winner = checkWinner(playersChoice,computersChoice,currentTurn);
              
              // Printing out who wins the hand played.
              if(winner == 0){
                   printf("You win that hand!\n");
                   playerScore += 5;
                   handsThrown++;
                   currentTurn = 0;
              }
              else if(winner == 1){
                   printf("Computer wins that hand!\n");
                   computerScore += 5;
                   handsThrown++;
                   currentTurn = 1;
                   
              }
              
              // Printing out the current scores.
              printf("\nPlayers Score = %d",playerScore);
              printf("\nComputers Score = %d\n",computerScore);
              
         }
         
         // Reset handsThrown as the hands are reset.
         handsThrown = 0;
         
         // Printing out who won the current game.
         if(computerScore == WIN){
              printf("The computer won that game!\n\n");
              computerGamesWon++;
              playerScore = 0;
              computerScore = 0;
         }
         else if(playerScore == WIN){
              printf("You won that game!\n\n");
              playerGamesWon++;
              playerScore = 0;
              computerScore = 0;
         }
         
         // Changing the dealer based on who dealt last.
         if(dealingTurn == 1){
              dealingTurn = 0;
         }
         else if(dealingTurn == 0){
              dealingTurn = 1;
         }
         
         // Printing out how many games each player has won.
         printf("\nPlayers Games won: %d\n",playerGamesWon);
         printf("Computer Games won: %d\n",computerGamesWon);
              
    } // End of while loop.
    
    // Printing out the winner of the whole game.
    if(computerGamesWon == RUBBER){
         printf("\nThe Computer has won!!\n\n");
    }
    else{
         printf("\nYou've won!!!!!!!\n\n");
    }
    
    printf("Thank you for playing!!");
              
    getch();	
    return 0;
} // End of main().

void shuffle(){
     
     // Declaring seed to generate the random number.
     srand(time(0));
     
     // Loop to continue swapping cards.
     for(i=0;i<52;i++){
          // Calculating a random number.
          rndm = (rand()%52);
          
          // Swapping the cards suit and value.
          tmpSuit = deck[rndm].suit;
          tmpValue = deck[rndm].value;
          deck[rndm].suit = deck[i].suit;
          deck[rndm].value = deck[i].value;
          deck[i].suit = tmpSuit;
          deck[i].value = tmpValue;
     }
          
} // End of shuffle().

void printDeck(){
     
     // Loops to run through each card in the deck to print out there details.
     for(i=0;i<52;i++){
          printf("Suit = %c",deck[i].suit);
          printf("\tValue = %d",deck[i].value);
          printf("\tStrength = %d",deck[i].strength);
          printf("\tOwner = %d\n",deck[i].owner);
    }
    
} // End of printDeck().

void createDeck(){
     
     // Loops to assign the suits and values to the cards.
     for(i=0;i<13;i++){
          deck[i].suit = 'C';
          deck[i].value = i+1;
     }
     for(i=13;i<26;i++){
          deck[i].suit = 'H';
          deck[i].value = i-12;
     }
     for(i=26;i<39;i++){
          deck[i].suit = 'S';
          deck[i].value = i-25;
     }
     for(i=39;i<52;i++){
          deck[i].suit = 'D';
          deck[i].value = i-38;
     }
     
} // End of createDeck().

int validMove(int Choice){
    
    if((deck[Choice].strength == 100) || (Choice > 10)){
         printf("Invalid Move!");
         return 0;
    }
    else{
         return 1;
    }
    
}// End validMove().

void firstAce(int* choice){
     
     // Iterating through the deck till the first ace is drawn.
     for(i=0;i<52;i++){
          
          // If the first ace is in a odd position nothing changes.
          if((deck[i].value == 1) && ((i%2) == 1)){
               break;
          }
          
          // Else if its in a even position the dealer is the other player.
          else if((deck[i].value == 1) && ((i%2) == 0)){
               if(*choice == 1){
                    *choice = 0;
                    break;
               }
               else if(*choice == 0){
                    *choice = 1;
                    break;
               }
          }
     }
     
} // End of firstAce().

int computersMove(){
    
     // Using the computers first card(For the moment).
     for(i=0;i<10;i++){
          if((deck[i].owner == 2) && (deck[i].strength != 100)){
               return i;
          }
     }
     
} // End of computersMove().

void assignStrength(){
     
     // Declaring variables.
     char SUIT = deck[10].suit;
     int VALUE = deck[10].value;
     
     // Looping throught the first 10 cards(both hands) and assigning strengths to each card.
     for(i=0;i<10;i++){

          // Assigning values to the non trumped cards.
          if((deck[i].suit == 'C') || (deck[i].suit == 'S')){
               if (deck[i].value == 13){
                    deck[i].strength = 16;
               }
               else if (deck[i].value == 12){
                    deck[i].strength = 17;
               }
               else if (deck[i].value == 11){
                    deck[i].strength = 18;
               }
               else if (deck[i].value == 1){
                    deck[i].strength = 19;
               }
               else if (deck[i].value == 2){
                    deck[i].strength = 20;
               }
               else if (deck[i].value == 3){
                    deck[i].strength = 21;
               }
               else if (deck[i].value == 4){
                    deck[i].strength = 22;
               }
               else if (deck[i].value == 5){
                    deck[i].strength = 23;
               }
               else if (deck[i].value == 6){
                    deck[i].strength = 24;
               }
               else if (deck[i].value == 7){
                    deck[i].strength = 25;
               }
               else if (deck[i].value == 8){
                    deck[i].strength = 26;
               }
               else if (deck[i].value == 9){
                    deck[i].strength = 27;
               }
               else if (deck[i].value == 10){
                    deck[i].strength = 28;
               }
          }
          if((deck[i].suit == 'H') || (deck[i].suit == 'D')){
               if (deck[i].value == 13){
                    deck[i].strength = 16;
               }
               else if (deck[i].value == 12){
                    deck[i].strength = 17;
               }
               else if (deck[i].value == 11){
                    deck[i].strength = 18;
               }
               else if (deck[i].value == 10){
                    deck[i].strength = 19;
               }
               else if (deck[i].value == 9){
                    deck[i].strength = 20;
               }
               else if (deck[i].value == 8){
                    deck[i].strength = 21;
               }
               else if (deck[i].value == 7){
                    deck[i].strength = 22;
               }
               else if (deck[i].value == 6){
                    deck[i].strength = 23;
               }
               else if (deck[i].value == 5){
                    deck[i].strength = 24;
               }
               else if (deck[i].value == 4){
                    deck[i].strength = 25;
               }
               else if (deck[i].value == 3){
                    deck[i].strength = 26;
               }
               else if (deck[i].value == 2){
                    deck[i].strength = 27;
               }
               else if (deck[i].value == 1){
                    deck[i].strength = 28;
               }
          }
          
          // Assigning strengths to the trumped cards.
          if((deck[i].suit == SUIT) && (deck[i].value == 5)){
               deck[i].strength = 1;
          }
          else if((deck[i].suit == SUIT) && (deck[i].value == 11)){
               deck[i].strength = 2;
          }
          else if((deck[i].suit == 'H') && (deck[i].value == 1)){
               deck[i].strength = 3;
          }
          else if((deck[i].suit == SUIT) && (deck[i].value == 1)){
               deck[i].strength = 4;
          }
          else if((deck[i].suit == SUIT) && (deck[i].value == 13)){
               deck[i].strength = 5;
          }
          else if((deck[i].suit == SUIT) && (deck[i].value == 12)){
               deck[i].strength = 6;
          }
          else if(deck[i].suit == SUIT){
               if((deck[i].suit == 'H') && (deck[i].value == 10)){
                    deck[i].strength = 7;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 10)){
                    deck[i].strength = 7;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 2)){
                    deck[i].strength = 7;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 2)){
                    deck[i].strength = 7;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 9)){
                    deck[i].strength = 8;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 9)){
                    deck[i].strength = 8;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 3)){
                    deck[i].strength = 8;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 3)){
                    deck[i].strength = 8;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 8)){
                    deck[i].strength = 9;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 8)){
                    deck[i].strength = 9;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 4)){
                    deck[i].strength = 9;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 4)){
                    deck[i].strength = 9;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 7)){
                    deck[i].strength = 10;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 7)){
                    deck[i].strength = 10;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 6)){
                    deck[i].strength = 11;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 6)){
                    deck[i].strength = 11;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 6)){
                    deck[i].strength = 11;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 6)){
                    deck[i].strength = 11;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 7)){
                    deck[i].strength = 12;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 7)){
                    deck[i].strength = 12;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 4)){
                    deck[i].strength = 13;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 4)){
                    deck[i].strength = 13;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 8)){
                    deck[i].strength = 13;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 8)){
                    deck[i].strength = 13;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 3)){
                    deck[i].strength = 14;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 3)){
                    deck[i].strength = 14;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 9)){
                    deck[i].strength = 14;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 9)){
                    deck[i].strength = 14;
               }
               else if((deck[i].suit == 'H') && (deck[i].value == 2)){
                    deck[i].strength = 15;
               }
               else if((deck[i].suit == 'D') && (deck[i].value == 2)){
                    deck[i].strength = 15;
               }
               else if((deck[i].suit == 'S') && (deck[i].value == 10)){
                    deck[i].strength = 15;
               }
               else if((deck[i].suit == 'C') && (deck[i].value == 10)){
                    deck[i].strength = 15;
               }
          }
      }
          
} // End of assignStrength().

int checkWinner(int P,int C,int T){
    
    // If either player has a trump.
    if((deck[10].suit == deck[P].suit) || (deck[10].suit == deck[C].suit)){
         if(deck[C].strength < deck[P].strength){
              deck[C].strength = 100;
              deck[P].strength = 100;
              return 1;
         }
         else{
              deck[C].strength = 100;
              deck[P].strength = 100;
              return 0;
         }
    }
    
    // If neither player has a trump but both suits are the same.
    if(deck[C].suit == deck[P].suit){
         if(deck[C].strength < deck[P].strength){
              deck[C].strength = 100;
              deck[P].strength = 100;
              return 1;
         }
         else{
              deck[C].strength = 100;
              deck[P].strength = 100;
              return 0;
         }
    }
    
    // If neither player has a trump or the same suit.
    if(T == 0){
         deck[C].strength = 100;
         deck[P].strength = 100;
         return 0;
    }
    if(T == 1){
         deck[C].strength = 100;
         deck[P].strength = 100;
         return 1;
    }
    
} // End of checkWinner().

void makeCardName(int V,char S){
     
     // Changes just a int value for the value of the card to a proper name where applicableand prints it out.
     if(V == 1){
          printf("Ace of ");
     }
     else if(V == 11){
          printf("Jack of ");
     }
     else if(V == 12){
          printf("Queen of ");
     }
     else if(V == 13){
          printf("King of ");
     }
     else{
          printf("%d of ",V);
     }
     
     // Changes just a character of a suit into the full word of a suit and prints it out.
     if(S == 'C'){
          printf("Clubs.\n");
     }
     else if(S == 'H'){
          printf("Hearts.\n");
     }
     else if(S == 'S'){
          printf("Spades.\n");
     }
     else{
          printf("Diamonds.\n");
     }
          
} // End makeCardName().

void printHand(int Player,int Computer,int Dealer){
     
     j = 1;
     
     printf("\nCurrent Hand:\n");
     
     for(i=0;i<10;i++){
          if(deck[i].owner == 1){
               printf("Card %d = ",j);
               j++;
               if(deck[i].strength != 100){
                    makeCardName(deck[i].value,deck[i].suit);
               }
               else{
                    printf("USED\n");
               }
          }    
     }
} // End printHand().

void assignOwner(int Player,int Computer,int Dealer){
     
     // Setting the owners if the player has dealt.
     if(Dealer == 0){
               for(i=Computer;i<(Computer + Player);i++){
                    deck[i].owner = 1;
               }
               for(i=(Player+Computer)+(5-Computer);i<10;i++){
                    deck[i].owner = 1;
               }
               for(i=0;i<Computer;i++){
                    deck[i].owner = 2;
               }
               for(i=(Computer+Player);i<((Computer+Player)+(5-Computer));i++){
                    deck[i].owner = 2;
               }
     }
     // Setting the owners if the computer has dealt.
     else if(Dealer == 1){
               for(i=0;i<Player;i++){
                    deck[i].owner = 1;
               }
               for(i=((Player+Computer));i<(((Player+Computer))+(5-Player));i++){
                    deck[i].owner = 1;
               }
               for(i=Player;i<(Player+Computer);i++){
                    deck[i].owner = 2;
               }
               for(i=((Player+Computer)+(5-Player));i<10;i++){
                    deck[i].owner = 2;
               }
     }
} // End printHand().

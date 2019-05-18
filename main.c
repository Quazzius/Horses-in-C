#include <stdio.h>
#include <stdlib.h>
#include<limits.h>
#define SIZE 6

/*this program simulates the dice game "horses"*/
//needs dice keeping option
//needs rebuttle for past payers that did not lose
int *rollDice(int); //prototypes
void play(int);
char *nextPlayer(int,int,int);
int keepDice();

//global variables
int score[3] = {0,0,0};
int playcount = 0;
int *p;

int main()
{
    char again = 'y';
    //keep playing loop
    while (again == 'y'){
        srand(time(NULL));
        int players = 0;
        //choose players
        puts("how many players?");
        scanf("%d",&players);

        for (int j = 0; j < players; j++)
        {
            //play game
            play(players);
        }
        //play another round
        playcount = 0;
        puts("\nplay again?\ny/n?");
        scanf("%s",&again);
    }
}
// roll them  dice
int * rollDice(int kept)
{
    static int dice[SIZE];
    for (; kept < SIZE; kept++)
    {
        dice[kept] = 1+ (rand()%6);
    }
    //output rolled dice
    for(int i = 0; i < SIZE; i++)
    {
        printf("%3d", dice[i]);
    }

    return dice;
}
void play(int players)
{
    int count = 0;
    char keep = 'n';
    int kept = 0;
    //keep rolling
    for (; keep == 'n' && count < 3; count++)
    {
        puts("\nroll'em!");
        // roll dice function
        p = rollDice(kept);
         //user can keep however many dice per turn
        puts("\nkeep any of these dice?\ny/n");
        char pick = 'n';
        scanf ("%s",&pick);
        if (pick == 'y'){
            kept = keepDice(p);
    }
        //prompt to if they want to  stay with their hand
        if (count < 3)
        {
            puts("\nStay?\ny/n?");
            scanf("%s", &keep);
        }
    }
    //test print after keep decision
     printf("test print array:");
        for (int i = 0; i < SIZE; i++){
            printf("%d ", *(p+i));
        }
    //count amounts of repeating values
    int most[SIZE] = {0,0,0,0,0,0};
    for (int i = 0; i < SIZE; i++)
    {
        switch(*(p+i))
        {
        case 1:
            most[0]++;
            break;
        case 2:
            most[1]++;
            break;
        case 3:
            most[2]++;
            break;
        case 4:
            most[3]++;
            break;
        case 5:
            most[4]++;
            break;
        case 6:
            most[5]++;
        }

        }
         printf("\ntest print totals:");
        for (int i = 0; i < SIZE; i++){
            printf("%d ", most[i]);
    }
    //find number with highest duplicate amount
    //if tied, the duplicate with the highest number is chosen
    int number = 0;
    int amount = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (amount < most[i])
        {
            amount = most[i];
            number = i + 1;
        }
        else if (amount == most[i]){
            if( i + 1 > number){
                amount = most[i];
                number = i + 1;
            }
        }
    }
    //find how many of the same number of the count with the highest value
    int numCount = 0;
    for (int i = 0; i < SIZE; i++)
    {
        if (*(p+i) == number)
        {
            numCount++;
        }
    }
    //print player score
    printf("\n%d %d's in %d roll(s)\n",numCount,number,count);
    if(playcount == 0){
        puts("score set\n");
        score[0] = numCount;
        score[1] = number;
        score[2] = count;
    }
    //compare score
    else{
        printf("\n %s",nextPlayer(numCount,number,count));
    }
    playcount++;
    if (playcount < players){
        puts("\n\nNext player");
    }

}
//next player's score compared to first player
char *nextPlayer(int nCount, int num, int ct){
    if (score[2] < ct){
       return "you lose!";
    }
    else if(score[0] == nCount){
        if(score[1] > num){
            return "you lose!";
        }
        else if(score[1] == num && score[2] == ct){
          return "tie!";

        }
        else{
            score[0] = nCount;
            score[1] = num;
            score[2] = ct;
            return "you win! - new score set";
        }
    }
    else if(score[2] > ct && score[1] < num){
        score[0] = nCount;
        score[1] = num;
        score[2] = ct;
        return "you win! - new score set";
    }
    else if (score[0] < nCount){
        score[0] = nCount;
        score[1] = num;
        score[2] = ct;
        return "you win! - new score set";

    }
    else{
        return "you lose!";
    }
}
//user can keep all of a single number per roll.
int keepDice(int dice[]){
    puts("keep which number?");
    int number = 0;
    scanf("%d",&number);

    //loop to find all dice for requested number
    int count = 0;
    for (int i = 0; i < SIZE; i++){
        if (dice[i] == number){
            count++;
        }
    }
    //assign number to first part of array to not be re-rolled
    for (int i = 0; i < count; i++){
        dice[i] = number;
    }
    //clear remaining spots so as not to  be added into final count
    for (int i = count; i < SIZE; i++){
        dice[i] = 0;
    }
    printf("Kept %d %d's",count, number);
    return count;
}


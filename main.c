#include <stdio.h>
#include <stdlib.h>
#include<limits.h>
#define SIZE 5

/*this program simulates the dice game "horses"
this game supports up to 10  players.the first player sets the mark and
the next players have to match or beat the first players mark. the score is set
by multiples of the same number and the player can keep however many of the same number per roll.
 ex: rolled 65463, player can keep whatever but if user picks 6, they keep both 6s.
 multiples beat out numbers. ex: three 1s beat out two 6s.
 the amount of rolls is important as whomever sets the mark also sets the amount
 of rolls it took to make it. ex: p1 rolled 3 6s in 2 rolls. p2 only
 gets up to 2 rolls to beat this score. players keep  playing until there is 1 left.
 each  player starts with $5 + amount of players each round and has to put a dollar into the pool for each
 turn, the total winnings are calculated at the end of each round*/

//AI opponents?

//prototypes
void rollDice(int[], int);//rolls dice
void play(int,int[], int[], int[], int); //game-play function
int nextPlayer(int,int,int,int[]);//compares scores
int keepDice(int[]); //choose dice to keep,calculates amount kept, allocates integers

int main()
{
    static int playcount = 0;//rounds played
    int dice[SIZE];//the dice
    char again = 'y';//play the game
    int players = 0;//amount of players
    int total[10] = {0,0,0,0,0,0,0,0,0,0};//maximum players
    int win = 0;//round winner

    //keep playing loop
    while (again == 'y'){
        int score[3] = {0,0,3};//reset score conditions
        int pool = 0;//money pool
        srand(time(NULL));
        //choose players
        if (playcount == 0){
            puts("how many players?");
            scanf("%d",&players);
        }
            //make array of players and cash
        int playerNumber[players];
        int dollar[players];
        for (int p = 0; p < players; p++){
            dollar[p] = 5 + players; //set players cash
            playerNumber[p] = 1; //set player's status
        }
        playcount = 0;
        //loop through amount of players
        for (int j = win; j < players; j++)
        {
            //each player takes a first turn and adds a dollar to the pool
            dollar[j]--;
            pool++;
            printf("\nplayer %d's turn\n$%d",j + 1, dollar[j]);
            play(j,playerNumber, dice, score, playcount);
            playcount ++;
        }
        //rebuttle for player that did not lose but have not beaten the high score
        //cycle to find those who didn't lose on an roll
        int sum = 0;
        int hold = 0;
        while (sum != 1){
            char putIn = 'y';
            for (int i = 0; i < players; i++){
                hold = score[0];//fixes strange disconnect bug
                sum = 0;
                 //check player status every turn
                for (int j = 0; j < players; j++){
                    if (playerNumber[j] == 1){
                        sum++;
                        win = j;
                    }
                }
                //winning condition
                if (sum == 1){
                    printf("\nplayer %d takes the %d dollar pot!\n",win +1, pool);
                    dollar[win] += pool;
                    break;
                }
                else if (playerNumber[i] == 1){

                    printf("Pot: $%d\n",pool);
                    if (playcount >= players)
                    {
                        printf("\nplayer %d's turn\n$%d",i + 1, dollar[i]);
                        puts("\nadd a dollar to roll?\ny/n");
                        scanf("%s",&putIn);
                    }
                    else{
                        printf("\nplayer %d's turn\n$%d",i + 1, dollar[i]-1);
                    }
                    if (putIn == 'y')
                    {
                        score[0] = hold;//fixes strange disconnect bug
                        pool++;
                        dollar[i]--;
                        play(i, playerNumber, dice, score, playcount);
                        playcount ++;
                    }
                    else{
                        playerNumber[i] = 0;
                    }
                }
            }
        }
        //calculate totals
        for (int t = 0; t < players; t++){
            total[t] += dollar[t] - (5 + players);
        }

        puts("total winnings:\n");
        for (int k = 0; k < players; k++){
            printf("player %d - $%d\n",k + 1, total[k]);
        }
        //play another round with same players
        puts("\nplay again?\ny/n?");
        scanf("%s",&again);
    }
}
// roll them  dice
void rollDice(int dice[],int amount)
{
    //randomize roll and output
    for (int i = amount; i < SIZE; i++)
    {
        dice[i] = 1+ (rand()%6);
    }
    for (int i = 0; i < SIZE; i++){
        printf("%3d", dice[i]);
    }
}
//game-play structure
void play(int player,int playerNumber[], int dice[], int score[], int playcount)
{

    int count = 0;//how many rolls
    char keep = 'n';
    int amnt = 0;
    int rolls = 0;
    //keep rolling
    for (; keep == 'n' && count < 3 && rolls < score[2]; count++)
    {
        if (playcount > 0){
            printf("\n(Must Beat: %d %d's in %d roll(s))",score[0],score[1],score[2]);
        }
        puts("\nroll'em!");
        // roll dice function
        rollDice(dice, amnt);
         //user picks number to keep from roll
        amnt = keepDice(dice);
        //prompt to if they want to  stay with their hand
        rolls++;
        if (count < 3 && rolls < score[2])
        {
            puts("\nStay?\ny/n?");
            scanf("%s", &keep);
        }
    }
    //count amounts of repeating dice values
    int most[6] = {0,0,0,0,0,0};
    for (int i = 0; i < SIZE; i++)
    {
        switch(dice[i])
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
    //find number with highest duplicate amount
    //if tied, the duplicate with the highest number is chosen
    int number = 0;
    int amount = 0;
    for (int i = 0; i < 6; i++)
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
        if (dice[i] == number)
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
    //compare score, return comparison status
    else{
      playerNumber[player] = nextPlayer(numCount,number,count,score);
    }
}
//next player's score compared to high score
int nextPlayer(int nCount, int num, int ct, int score[]){
    if (score[2] < ct){
       puts("you lose!");
       return 0;
    }
    else {
        if (score[0] > nCount){
            puts("you lose!");
            return 0;
        }
        else{
            if (score[0] < nCount){
                score[0] = nCount;
                score[1] = num;
                score[2] = ct;
                puts("New high score set");
                return 1;
            }
            else{
                if (score[1] > num){
                    puts("you lose!");
                    return 0;
                }
                else if (score[1] == num){
                    puts("tie! - push");
                    return 1;
                }
                else{
                    score[0] = nCount;
                    score[1] = num;
                    score[2] = ct;
                    puts("New high score set");
                    return 1;
                }
            }
        }
    }
    return -1;
}
//user can keep all of a single number per roll.
int keepDice(int dice[]){
    puts("\nkeep which number?");
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
    printf("Kept %d %d's\n",count, number);
    return count;
}


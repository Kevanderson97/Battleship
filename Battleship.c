//Kevin Anderson
//COP 3502H
//Made in Code::Blocks

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define AI 1
#define HUMAN 0
//W == water, S == Ship (used for the actualBoard)
//X == Hit, O == Miss, M == Unknown (m stands for "mystery"), Z == Sunk (used for guessBoard) (X and Z are used for both)
typedef enum tiles {W,S,X,O,U,Z}tiles;

//used to track x and y coordinates simultaneously. Not used throughout the majority of the program for reasons
typedef struct coordinates{
    int x;
    int y;
}coor;

typedef struct ship{
    int start[2];
    int stop[2];
    int size;
    char dir;
    bool sunk;
}ship;
typedef char board[10][10];


board* playerPlace(board* actualBoard, ship **ships);
void playerTurn(board* actualBoard, board* guessBoard, ship **ships);
board* aiPlace(board* actualBoard, ship **ships);
void aiTurn(board* actualBoard, board* guessBoard, ship **ships);
bool checkPlace(board* actualBoard, int x, int y, char dir, int length, int player);
coor AI_FindHits(board* guessBoard);
ship* placeShip(board* actualBoard, int x, int y, char dir, int length, int player);
void printGrid(board *myBoard,int player);
//tp == turn player
void hitMiss(board* actualBoard,board* guessBoard,int x,int y,int tp,ship** ships);
void sunken(board* actualBoard,board* guessBoard,int x,int y,int tp,ship** ships);
void checkWL(board* actualBoard);

int main()
{
    //There are two board arrays. One (actualBoard) contains the actual board that the
    //players have placed ships on. The other (guessBoard) contains the board that
    //the players can see (i.e. has hit/miss/unknown tiles). Index 0 contains the
    //boards that the player can see, while index 1 contains the boards that the
    //AI can see.
    int i = 0, j = 0, k = 0;
    board actualBoard[2];
    board guessBoard[2];
    ship *pShips[5], *aiShips[5];

    srand(time(NULL));

    printf("On the grid to be used for this game, S=Ship, W=Water, X=Hit, O=Miss, M=Mystery, Z=Sunken Ship\n");

    for (i=0;i<2;i++)
        for(j=0;j<10;j++)
            for(k=0;k<10;k++)
                actualBoard[i][j][k]=W;
    for (i=0;i<2;i++)
        for(j=0;j<10;j++)
            for(k=0;k<10;k++)
                guessBoard[i][j][k]=U;


    playerPlace(actualBoard,pShips);
    aiPlace(actualBoard,aiShips);

    //I know that I didn't use a function for the game loop like the instructions wanted,
    //but I thought it was kind of unnecessary. It's just a loop.
    while(true){
        playerTurn(actualBoard,guessBoard,aiShips);
        printf("\n\n");
        aiTurn(actualBoard,guessBoard,pShips);
        printf("\n\n");
    }


    return 0;
}

board* playerPlace(board* actualBoard, ship **ships){
    char startPos[2];
    char direction;
    int x,y;

    printGrid(actualBoard,0);

    //Aircraft Carrier
    printf("Enter the start position (in the form of the capital letter coordinate followed by the number coordinate) and direction (N, E, S, or W) of the Aircraft Carrier (length 5) ");
    while(true){
        //scans in coordinates, converts them to integers, and checks if a ship can be placed there in the given direction.
        //If a ship can be placed there, do so.
        scanf(" %c %c",&startPos[0],&startPos[1]);
        scanf(" %c",&direction);
        startPos[0] = toupper(startPos[0]);
        x = startPos[0] - 'A';
        y = startPos[1] - '0';
        direction = toupper(direction);
        if(checkPlace(actualBoard,x,y,direction,5,0)){
            ships[0] = placeShip(actualBoard,x,y,direction,5,0);
            break;
        }
        else
            printf("Please enter a valid input.");
    }
    //Print out the grid so that the player can see their new ship on the field
    printGrid(actualBoard,0);

    //Battleship
    printf("Enter the start position (in the form of the capital letter coordinate then the number coordinate) and direction (N, E, S, or W) of the Battleship (length 4) ");
    while(true){
        //Same as the first loop.
        scanf(" %c %c",&startPos[0],&startPos[1]);
        scanf(" %c",&direction);
        startPos[0] = toupper(startPos[0]);
        x = startPos[0] - 'A';
        y = startPos[1] - '0';
        direction = toupper(direction);
        if(checkPlace(actualBoard,x,y,direction,4,0)){
            ships[1] = placeShip(actualBoard,x,y,direction,4,0);
            break;
        }
        else
            printf("Please enter a valid input.");
    }
    printGrid(actualBoard,0);

    //Submarine
    printf("Enter the start position (in the form of the capital letter coordinate then the number coordinate) and direction (N, E, S, or W) of the Submarine (length 3) ");
    while(true){
        scanf(" %c %c",&startPos[0],&startPos[1]);
        scanf(" %c",&direction);
        startPos[0] = toupper(startPos[0]);
        x = startPos[0] - 'A';
        y = startPos[1] - '0';
        direction = toupper(direction);
        if(checkPlace(actualBoard,x,y,direction,3,0)){
            ships[2] = placeShip(actualBoard,x,y,direction,3,0);
            break;
        }
        else
            printf("Please enter a valid input.");
    }
    printGrid(actualBoard,0);

    //Battleship
    printf("Enter the start position (in the form of the capital letter coordinate then the number coordinate) and direction (N, E, S, or W) of the Destroyer (length 3) ");
    while(true){
        scanf(" %c %c",&startPos[0],&startPos[1]);
        scanf(" %c",&direction);
        startPos[0] = toupper(startPos[0]);
        x = startPos[0] - 'A';
        y = startPos[1] - '0';
        direction = toupper(direction);
        if(checkPlace(actualBoard,x,y,direction,3,0)){
            ships[3] = placeShip(actualBoard,x,y,direction,3,0);
            break;
        }
        else
            printf("Please enter a valid input.");
    }
    printGrid(actualBoard,0);

    //Patrol Boat
    printf("Enter the start position (in the form of the capital letter coordinate then the number coordinate) and direction (N, E, S, or W) of the Patrol Boat (length 2) ");
    while(true){
        scanf(" %c %c",&startPos[0],&startPos[1]);
        scanf(" %c",&direction);
        startPos[0] = toupper(startPos[0]);
        x = startPos[0] - 'A';
        y = startPos[1] - '0';
        direction = toupper(direction);
        if(checkPlace(actualBoard,x,y,direction,2,0)){
            ships[4] = placeShip(actualBoard,x,y,direction,2,0);
            break;
        }
        else
            printf("Please enter a valid input.");
    }
    return actualBoard;
}

board* aiPlace(board* actualBoard, ship **ships){
    char direction;
    int x,y,temp;

    //Aircraft Carrier
    //Loop, generating random coordinates and directions until a valid ship configuration is chosen.
    while(true){
        x = rand()%10;
        y = rand()%10;
        temp = rand()%4;
        //0 == North, 1 == South, 2 == East, 3 == West
        switch(temp){
        case 0:
            //If it is along the top wall, the ship will not fit going north, no matter the size. Continue to save time.
            if(y==0)
                continue;
            else {
                direction = 'N';
                break;}
        case 1:
            if(y==9)
                continue;
            else {
                direction = 'S';
                break;}
        case 2:
            if(x==9)
                continue;
            else {
                direction = 'E';
                break;}
        case 3:
            if(x==0)
                continue;
            else {
                direction = 'W';
                break;}
        }
        if(checkPlace(actualBoard,x,y,direction,5,1)){
            ships[0] = placeShip(actualBoard,x,y,direction,5,1);
            break;
        }
        else
            continue;
    }

    //Battleship
    while(true){
        x = rand()%10;
        y = rand()%10;
        temp = rand()%4;
        //0 == North, 1 == South, 2 == East, 3 == West
        switch(temp){
        case 0:
            //If it is along the top wall, the ship will not fit going north, no matter the size. Restart loop to save time.
            if(y==0)
                continue;
            else {
                direction = 'N';
                break;}
        case 1:
            if(y==9)
                continue;
            else {
                direction = 'S';
                break;}
        case 2:
            if(x==9)
                continue;
            else {
                direction = 'E';
                break;}
        case 3:
            if(x==0)
                continue;
            else {
                direction = 'W';
                break;}
        }
        if(checkPlace(actualBoard,x,y,direction,4,1)){
            ships[1] = placeShip(actualBoard,x,y,direction,4,1);
            break;
        }
        else
            continue;
    }

    //Submarine
    while(true){
        x = rand()%10;
        y = rand()%10;
        temp = rand()%4;
        //0 == North, 1 == South, 2 == East, 3 == West
        switch(temp){
        case 0:
            //If it is along the top wall, the ship will not fit going north, no matter the size. Continue to save time.
            if(y==0)
                continue;
            else {
                direction = 'N';
                break;}
        case 1:
            if(y==9)
                continue;
            else {
                direction = 'S';
                break;}
        case 2:
            if(x==9)
                continue;
            else {
                direction = 'E';
                break;}
        case 3:
            if(x==0)
                continue;
            else {
                direction = 'W';
                break;}
        }
        if(checkPlace(actualBoard,x,y,direction,3,1)){
            ships[2] = placeShip(actualBoard,x,y,direction,3,1);
            break;
        }
        else
            continue;
    }

    //Destroyer
    while(true){
        x = rand()%10;
        y = rand()%10;
        temp = rand()%4;
        //0 == North, 1 == South, 2 == East, 3 == West
        switch(temp){
        case 0:
            //If it is along the top wall, the ship will not fit going north, no matter the size. Continue to save time.
            if(y==0)
                continue;
            else {
                direction = 'N';
                break;}
        case 1:
            if(y==9)
                continue;
            else {
                direction = 'S';
                break;}
        case 2:
            if(x==9)
                continue;
            else {
                direction = 'E';
                break;}
        case 3:
            if(x==0)
                continue;
            else {
                direction = 'W';
                break;}
        }
        if(checkPlace(actualBoard,x,y,direction,3,1)){
            ships[3] = placeShip(actualBoard,x,y,direction,3,1);
            break;
        }
        else
            continue;
    }

    //Patrol Boat
    while(true){
        x = rand()%10;
        y = rand()%10;
        temp = rand()%4;
        //0 == North, 1 == South, 2 == East, 3 == West
        switch(temp){
        case 0:
            //If it is along the top wall, the ship will not fit going north, no matter the size. Continue to save time.
            if(y==0)
                continue;
            else {
                direction = 'N';
                break;}
        case 1:
            if(y==9)
                continue;
            else {
                direction = 'S';
                break;}
        case 2:
            if(x==9)
                continue;
            else {
                direction = 'E';
                break;}
        case 3:
            if(x==0)
                continue;
            else {
                direction = 'W';
                break;}
        }
        if(checkPlace(actualBoard,x,y,direction,2,1)){
            ships[4] = placeShip(actualBoard,x,y,direction,2,1);
            break;
        }
        else
            continue;
    }
    return actualBoard;
}

bool checkPlace(board* actualBoard, int x, int y, char dir, int length, int player){
    int i = 0;
    length--;
    //If the selected space is occupied already, then it's obviously not valid. Return false.
    if(actualBoard[player][x][y]!=W)
        return false;

    switch(dir){
    case 'W':
        //If the ship length would overflow off the end of the grid, the configuration is invalid. Return false.
        if((x-length) < 0)
            return false;
        //Loop through the board spaces in the desired direction. If a space is reached that is not water
        //(and thusly occupied), return false. If the end of the loop is reached without this happening, return true.
        for(i=1;i<length+1;i++){
            if(actualBoard[player][x-i][y]!=W)
                return false;
        }
        return true;
    case 'E':
        if((x+length) > 9)
            return false;
        for(i=1;i<length+1;i++){
            if(actualBoard[player][x+i][y]!=W)
                return false;
        }
        return true;
    case 'S':
        if((y+length) > 9)
            return false;
        for(i=1;i<length+1;i++){
            if(actualBoard[player][x][y+i]!=W)
                return false;
        }
        return true;
    case 'N':
        if((y-length) < 0)
            return false;
        for(i=1;i<length+1;i++){
            if(actualBoard[player][x][y-i]!=W)
                return false;
        }
        return true;
    default: return false;
    }
}

ship* placeShip(board* actualBoard, int x, int y, char dir, int length, int player){
    //Allocates memory for new ship and initializes all values except stop[].
    ship *newShip = malloc(sizeof(ship));
    newShip->start[0]=x;
    newShip->start[1]=y;
    newShip->size=length;
    newShip->dir=dir;
    newShip->sunk=false;
    int i = 0;
    switch(dir){
        case 'W':
            //traverses the board array based on the input direction. Set all passed cells to S.
            //Additionally, when the end is reached, set stop[] to that cell's x and y coordinates.
            for(i=0;i<length;i++){
                actualBoard[player][x-i][y] = S;
            }
            newShip->stop[0]=x-(length-1);
            newShip->stop[1]=y;
            break;
        case 'E':
            for(i=0;i<length;i++){
                actualBoard[player][x+i][y] = S;
            }
            newShip->stop[0]=x+(length-1);
            newShip->stop[1]=y;
            break;
        case 'S':
            for(i=0;i<length;i++)
                actualBoard[player][x][y+i]= S;
            newShip->stop[0]=x;
            newShip->stop[1]=y+(length-1);
            break;
        case 'N':
            for(i=0;i<length;i++){
                actualBoard[player][x][y-i] = S;
            }
            newShip->stop[0]=x;
            newShip->stop[1]=y-(length-1);
            break;
    }
    //return a pointer to the ship.
    return newShip;
}

void printGrid(board *myBoard,int player){
    int j,k=j=0;
    printf("\n");
    //prints the header of the table
    printf("  A B C D E F G H I J");
    //Loops through the board array, printing its contents as well as the y-axis header.
        for(j=0;j<10;j++){
            printf("\n");
            printf("%d ",j);
            for(k=0;k<10;k++)
                printf("%s",myBoard[player][k][j]==S ? "S " : myBoard[player][k][j]==W ? "W " :
                       myBoard[player][k][j]==X ? "X " :myBoard[player][k][j]==O ? "O " :myBoard[player][k][j]==Z ? "Z " :
                           myBoard[player][k][j]==U ? "M " : "B ");
        }
    printf("\n\n");
    return;
}

void playerTurn(board* actualBoard, board* guessBoard, ship **ships){
    char startPos[2];
    char input;
    int x,y;

    //Prints the top and bottom boards for the player to see.
    printf("This is what you know about the enemy's board:");
    printGrid(guessBoard,HUMAN);
    printf("This is your board:");
    printGrid(actualBoard,HUMAN);
    while(true){
        //Get coordinates to shoot a torpedo at.
        printf("Where do you want to fire your next torpedo (enter a letter followed by a number)? ");
        scanf(" %c %c",&startPos[0],&startPos[1]);
        x = toupper(startPos[0]) - 'A';
        y = startPos[1] - '0';
        if(x > 9 || x < 0 || y > 9 || y < 0){
            printf("Please enter a valid input.\n");
            continue;
        }
        if(guessBoard[HUMAN][x][y]!=U){
            //If the player input a coordinate they've already discovered, double check that they want to do this.
            //If they don't say yes, restart the loop.
            printf("It's recommended that you fire your torpedo somewhere you haven't already attacked. Proceed anyway (Y/N)? ");
            scanf(" %c",&input);
            if(input!='Y' && input!='y')
                continue;
        }
        break;
    }
    hitMiss(actualBoard,guessBoard,x,y,HUMAN,ships);
    printf("Turn shift! It's the AI's turn now.\n");
    return;
}

void hitMiss(board* actualBoard,board* guessBoard,int x,int y,int tp,ship** ships){
    //Note that since tp must be either 1 or 0, 1-tp will always get the opponent player
    if(actualBoard[1-tp][x][y]==S){
        actualBoard[1-tp][x][y]=X;
        guessBoard[tp][x][y]=X;
        printf("%s: Hit!\n", 1-tp==1 ? "AI": "Player");
        //figure out a way to find out which ship is hit
        sunken(actualBoard,guessBoard,x,y,tp,ships);
        checkWL(actualBoard);


    }
    else if(actualBoard[1-tp][x][y]==W){
        actualBoard[1-tp][x][y]=O;
        guessBoard[tp][x][y]=O;
        printf("%s: Miss!\n", 1-tp==1 ? "AI": "Player");}
    else
        printf("%s: You've previously hit this location. That's a bit of overkill, don't you think?\n", 1-tp==1 ? "AI:": "Player");
    return;
}
void aiTurn(board* actualBoard, board* guessBoard, ship **ships){
    int x,y;
    int surroundings = 4;
    coor target;
    //If there is a hit on the board, find it and try to guess other related hits in a somewhat smart way
    target = AI_FindHits(guessBoard);
    x=target.x;
    y=target.y;
    if(target.x<0 || target.y<0)
        while(true){
            //Generate a random guess.
            x = rand()%10;
            y = rand()%10;
            //If the guess has already been chosen, guess again.
            if(guessBoard[AI][x][y]!=U){
                continue;
            }
            //This series of ifs essentially says "If the guessed space is surrounded by walls
            //and/or spaces already revealed, then choose a new guess." Because, a space surrounded
            //by already guessed spaces can't be a ship, as ships have a min. size of 2.
            if(x<9 && guessBoard[AI][x+1][y]!=U)
                surroundings--;
            else if (x==9)
                surroundings--;
            if (x>0 && guessBoard[AI][x-1][y]!=U)
                surroundings--;
            else if (x==0)
                surroundings--;
            if (y<9 && guessBoard[AI][x][y+1]!=U)
                surroundings--;
            else if (y==9)
                surroundings--;
            if (y>0 && guessBoard[AI][x][y-1]!=U)
                surroundings--;
            else if (y==0)
                surroundings--;
            if(surroundings==0)
                continue;
            break;
        }
    //Prints AI space, calls hit/miss protocol, then returns so player 1 can go.
    printf("AI calls the space... %c%d!!!!\n",x+'A',y);
    hitMiss(actualBoard,guessBoard,x,y,AI,ships);
    printf("Turn shift! It's the Player's turn now.\n");
    return;
}

//Finds out if any given ship has been sunken. Called upon a new hit.
void sunken(board* actualBoard,board* guessBoard,int x,int y,int tp,ship** ships){
    bool sunken;
    int i=0,j=0;
    for(i=0;i<5;i++){
        sunken=true;
        //Basically, for each potential circumstance, loop through spaces on the board from the ship's
        //start to stop location. If all of those spaces are Xs, the ship has been sunk. Set ship->sunk to true
        //and set spaces on the board to Z.
        if(ships[i]->sunk==false && ships[i]->start[0]!=ships[i]->stop[0] && ships[i]->start[1]==y){
            if(ships[i]->dir=='E'){
                for(j=ships[i]->start[0];j<=ships[i]->stop[0];j++){
                    if(actualBoard[1-tp][j][y]!=X)
                        sunken=false;
                }
                if (sunken==true)
                    for(j=ships[i]->start[0];j<=ships[i]->stop[0];j++){
                        actualBoard[1-tp][j][y]=Z;
                        guessBoard[tp][j][y]=Z;
                        ships[i]->sunk=true;
                }
            }
            else if (ships[i]->dir=='W'){
                for(j=ships[i]->start[0];j>=ships[i]->stop[0];j--){
                    if(actualBoard[1-tp][j][y]!=X)
                        sunken=false;
                }
                if (sunken==true)
                    for(j=ships[i]->start[0];j>=ships[i]->stop[0];j--){
                        actualBoard[1-tp][j][y]=Z;
                        guessBoard[tp][j][y]=Z;
                        ships[i]->sunk=true;
                }
            }
        }
        else if(ships[i]->sunk==false && ships[i]->start[1]!=ships[i]->stop[1] && ships[i]->start[0]==x){
            if(ships[i]->dir=='S'){
                for(j=ships[i]->start[1];j<=ships[i]->stop[1];j++){
                    if(actualBoard[1-tp][x][j]!=X)
                        sunken=false;
                }
                if (sunken==true)
                    for(j=ships[i]->start[1];j<=ships[i]->stop[1];j++){
                        actualBoard[1-tp][x][j]=Z;
                        guessBoard[tp][x][j]=Z;
                        ships[i]->sunk=true;
                }
            }
            else if (ships[i]->dir=='N'){
                for(j=ships[i]->start[1];j>=ships[i]->stop[1];j--){
                    if(actualBoard[1-tp][x][j]!=X)
                        sunken=false;
                }
                if (sunken==true)
                    for(j=ships[i]->start[1];j>=ships[i]->stop[1];j--){
                        actualBoard[1-tp][x][j]=Z;
                        guessBoard[tp][x][j]=Z;
                        ships[i]->sunk=true;
                }
            }
        }
        else
            sunken = false;
        //If a battleship was sunk, print a message saying so.
        if(sunken==true)
            printf("%s: You've sunk my battleship!!\n", 1-tp==1 ? "AI:": "Player");
    }
}
//I tried to give it a relatively smart AI, so if there is an existing hit, this function will investigate spaces near it,
//enabling the AI to sink whole ships much faster. The only major problem with it is that it will always give horizontal
//searching priority, regardless of whether or not a vertical pattern emerges.
coor AI_FindHits(board* guessBoard){
    int j,k;
    coor point;
    for(j=0;j<10;j++)
            for(k=0;k<10;k++){
                //If a hit exists on the board, investigate spaces near it, giving priority to horizontal investigation.
                if (guessBoard[AI][k][j]==X){
                    if(k<9 && (guessBoard[AI][k+1][j]==U || guessBoard[AI][k+1][j]==X)){
                        //If you have discovered a horizontal pattern of Xs, backtrack to see if
                        //spaces behind you are also Xs (like if the initial guess was in the middle of the ship).
                        if(k>0 && guessBoard[AI][k+1][j]==X && guessBoard[AI][k-1][j]==U){
                            point.x=k-1;
                            point.y=j;
                            return point;
                        }
                        if (guessBoard[AI][k+1][j]==X)
                            continue;
                        //Return x and y coordinates of unknown space immediately
                        //to the right of a currently known X.
                        point.x=k+1;
                        point.y=j;
                        return point;
                    }
                    //Repeat the same as above for the 3 other potential cases.
                    else if (k>0 && (guessBoard[AI][k-1][j]==U || guessBoard[AI][k-1][j]==X)){
                        if(k<9 && guessBoard[AI][k-1][j]==X && guessBoard[AI][k+1][j]==U){
                            point.x=k+1;
                            point.y=j;
                            return point;
                        }
                        if (guessBoard[AI][k-1][j]==X)
                            continue;
                        point.x=k-1;
                        point.y=j;
                        return point;
                    }
                    else if (j<9 && (guessBoard[AI][k][j+1]==U || guessBoard[AI][k][j+1]==X)){
                        if(j>0 && guessBoard[AI][k][j+1]==X && guessBoard[AI][k][j-1]==U){
                            point.x=k;
                            point.y=j-1;
                            return point;
                        }
                        if (guessBoard[AI][k][j+1]==X)
                            continue;
                        point.x=k;
                        point.y=j+1;
                        return point;
                    }
                    else if (j>0 && (guessBoard[AI][k][j-1]==U || guessBoard[AI][k][j-1]==X)){
                        if(j>0 && guessBoard[AI][k][j-1]==X && guessBoard[AI][k][j+1]==U){
                            point.x=k;
                            point.y=j+1;
                            return point;
                        }
                        if (guessBoard[AI][k][j-1]==X)
                            continue;
                        point.x=k;
                        point.y=j-1;
                        return point;
                    }
                }
            }
    //If there are no Xs, return -1, indicating that a random number must be chosen.
    point.x=-1;
    point.y=-1;
    return point;
}

//Count the number of Zs on the board, indicating if a player has won/lost. If they have, exit the program.
void checkWL(board* actualBoard){
    int i,j,k;
    int counter_AI=0, counter_Human=0;
    for (i=0;i<2;i++)
        for(j=0;j<10;j++)
            for(k=0;k<10;k++)
                if(actualBoard[i][k][j]==Z)
                    i==HUMAN ? counter_AI++ : counter_Human++;
    if(counter_Human==17){
        printf("Congrats! You beat the AI and won the game!!\n");
        exit(0);
    }
    else if (counter_AI==17){
        printf("Too bad! The AI schooled you and eliminated all of your ships. Better luck next time.\n");
        exit(0);
    }
    return;

}

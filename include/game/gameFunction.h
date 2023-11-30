#ifndef GAMEFUNCTION_H
#define GAMEFUNCTION_H

typedef struct
{
    int   cases[12];
    char* moves;
} Board;

typedef struct
{
    Board board;
    int   score1;
    int   score2;
} Game;

Game initGame();
int  emptyBoard(Board* b, int firstCell, int lastCell);
int  testFamine(Game g, int actualPlayer);
int  isWin(Game* g, int actualPlayer);
void showGame(Game g);
char enterAction();
int  validMove(Game g, char c, int actualPlayer);
int  charToCase(char c);
void playMove(Game* g, char c);
int  moveSeeds(Board* b, int numCase);
void saveMove(Board* b, char c);
int  getNbFirstCellEaten(Board b, int arrivedCell);
int  getNbSeedsAfterEat(Board b, int arrivedCell);

#endif
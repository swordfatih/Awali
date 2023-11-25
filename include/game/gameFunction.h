#ifndef GAMEFUNCTION_H
#define GAMEFUNCTION_H

typedef struct {
    int cases[12];
    char* moves;
} Board;

typedef struct {
    Board board;
    int score1;
    int score2;
} Game;

Game initGame();
int emptyBoard(Board *b);
int testFamine(Game g, int actualPlayer);
int isWin(Game *g, int actualPlayer);
void showBoard(Board b);
char enterAction();
int coupValide(Game g, char c, int actualPlayer);
int charToCase(char c);
void playMove(Game *g, char c, int actualPlayer);
int moveSeeds(Board *b, int numCase);


 #endif
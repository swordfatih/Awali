#include "gameFunction.h"
#include <stdlib.h>
#include <stdio.h>

Game initGame() {
    Game g;
    int i = 0;
    for(i = 0 ; i < 12 ; i++){
        g.board.cases[i] = 4;
    }
    g.board.moves = malloc(1024);
    g.score1 = 0;
    g.score2 = 0;
}

int emptyBoard(Board *b){
    int rest = 0;
    int i;
    for (i = 0 ; i < 12; i++){
        rest = b->cases[i];
        b->cases[i] = 0;
    }
    return rest;
}

int testFamine(Game *g, int actualPlayer){
    int debut;
    if (actualPlayer == 1){
        debut = 0;
    } else {
        debut = 6;
    }
    int total = 0;
    int i;
    for (i = debut; i < debut + 6 ; i++){
        total += g->board.cases[i];
        printf("i : %d, total : %d\n",i, total);
    }
    if (total == 0){ // si le joueur ne peut pas jouer car il n'a plus de grain, je lui met tous les grains restants
        int rest = emptyBoard(&g->board);
        if (actualPlayer == 1){
            g->score2 += rest;
        } else {
            g->score1 += rest;
        }
        return 2;
    }
    return 0;
}

int isWin(Game *g, int actualPlayer){
    if (g->score1 > 24 || g->score2 > 24){
        return 1;
    }

    int famine = testFamine(g, actualPlayer);
    if (famine > 0){
        return famine;
    }

    //tester plus moyen de manger

    return 0;
    
}
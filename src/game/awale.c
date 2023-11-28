#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "gameFunction.h"

int main (int narg, char** argv) {
    Game game = initGame();
    int actualPlayer = 1; ///TODO A GENERER ALEATOIREMENT
    char c = 0;

    //Test avec lecture d'un historique
    if (narg == 2){
        FILE *f;
        f = fopen(argv[1], "r");
        if (!f){
            printf("Open error\n");
            exit(-1);
        }
        while(c != EOF){
            showGame(game);
            c = fgetc(f);
            playMove(&game, c);
            actualPlayer = (actualPlayer == 1) ? 2 : 1;
            getchar();
            c = fgetc(f);
        }
        fclose(f);
        exit(-1);
    }

    srand(time(NULL));
    actualPlayer = rand()%2 + 1;

    while (isWin(&game, actualPlayer) == 0) {
        printf("Tour du joueur : %d\n", actualPlayer);
        showGame(game);

        c = enterAction();
        while (validMove(game, c, actualPlayer) == 0){
            c = enterAction();
        }

        saveMove(&(game.board), c);
        playMove(&game, c);
      
        actualPlayer = (actualPlayer == 1) ? 2 : 1;
    }

    if(game.score1 > game.score2){
        printf("Le joueur 1 a gagné avec %d points !\n", game.score1);
    } else {
        printf("le joueur 2 a gagné avec %d points !\n", game.score2);
    }

    //Ecriture de l'historique dans un fichier
    FILE *f;
    f = fopen("partie.txt", "w");
    fputs(game.board.moves, f);
    fclose(f);

    free(game.board.moves);

    return 0;
}
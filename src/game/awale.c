#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "gameFunction.h"

int main () {
    Game game = initGame();
    int actualPlayer = 1; ///TODO A GENERER ALEATOIREMENT
    char c;
    showBoard(game.board);

    while (isWin(&game, actualPlayer) == 0) {
      printf("Tour du joueur : %d\n", actualPlayer);

      c = enterAction();
      while (coupValide(game, c, actualPlayer) == 0){
        c = enterAction();
      }

      playMove(&game, c, actualPlayer);
      
      showBoard(game.board);
      actualPlayer = (actualPlayer == 1) ? 2 : 1;
    }
    return 0;
}
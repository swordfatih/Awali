#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "gameFunction.h"

int main () {
    Game game = initGame();
    int actualPlayer = 1; ///TODO A GENERER ALEATOIREMENT
    //while(isWin(&game, actualPlayer) == 0){
//
  //  }
    int a = isWin(&game, actualPlayer);
    printf("resultat : %d\n", a);

    return 0;
}
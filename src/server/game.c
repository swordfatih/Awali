#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * initialise les scores et le plateau
*/
Game initGame() {
    Game g;
    int i = 0;
    for(i = 0 ; i < 12 ; i++){
        g.board.cases[i] = 4;
    }
    g.board.moves = malloc(1024);
    g.board.moves[0] = '\0';
    g.score1 = 0;
    g.score2 = 0;
    return g;
}

/**
 * visual representation of the game with scores & board
*/
void showGame(Game g){
    printf("\nJoueur 1 : %d points\t Joueur 2 : %d points\n\n", g.score1, g.score2);
    int i;
    for (i = 65 ; i < 65+6 ; i++){
        printf(" %c", i);
    }
    printf("\n");
    for (i = 0 ; i < 6 ; i++){
        printf("|%d", g.board.cases[i]);
    }
    printf("|\n");
    for (i = 11 ; i > 5 ; i--){
        printf("|%d", g.board.cases[i]);
    }
    printf("|\n");
    for (i = 97 ; i < 97+6 ; i++){
        printf(" %c", i);
    }
    printf("\n\n");
}

/**
 * vide le tableau
 * renvoie le nombre de graines restantes
*/
int emptyBoard(Board *b, int firstCell, int lastCell){
    int rest = 0, i;
    for (i = firstCell ; i <= lastCell; i++){
        rest += b->cases[i];
        b->cases[i] = 0;
    }
    return rest;
}

/**
 * test si le joueur actuel a des graines de son coté
 * renvoie le nombre de graine de son cote
*/
int testFamine(Game g, int actualPlayer){
    int debut;
    if (actualPlayer == 1){
        debut = 0;
    } else {
        debut = 6;
    }
    int total = 0;
    int i;
    for (i = debut; i < debut + 6 ; i++){
        total += g.board.cases[i];
    }
    return total;
}

/**
 * test si la partie est fini : 
 * return 0 : la partie continue
 * return 1 : la partie est fini par score
 * return 2 : la partie est fini car famine
 * return 3 : la partie est fini car plus de coups faisables (non implémenter)
*/
int isWin(Game *g, int actualPlayer){
    if (g->score1 > 1 || g->score2 > 1){
        return 1;
    }

    int total = testFamine(*g, actualPlayer);
    if (total == 0){
        // si le joueur ne peut pas jouer car il n'a plus de grain
        // l'adversaire gagne des points
        int rest = emptyBoard(&g->board, 0, 11);
        if (actualPlayer == 1){
            g->score2 += rest;
        } else {
            g->score1 += rest;
        }
        return 2;
    }
    //tester plus moyen de manger

    return 0;
}

/**
 * demande une lettre au joueur
 * verifie qu'il n'y a qu'1 caractere
 * renvoie le caractere taper
*/
char enterAction(){
    int valid = 0;
    char *buff = malloc(3);
    char ch;
    do {
        printf("Entrez la case de votre choix : ");
        fgets(buff, 3, stdin);
        if(buff[1] != '\n'){
            while (((ch = getchar()) != '\n') && (ch != EOF)){}
            printf("Trop de caracteres\n");
        } else {
            valid = 1;
        }
    } while (valid == 0);
    ch = buff[0];
    free(buff);
    return ch;
}


/**
 * teste si le coup du joueur est valide
 * renvoie 0 si le coup est mauvais
 * renvoie 1 si le coup est bon
*/
int validMove(Game g, char c, int actualPlayer){
    // Test bon cote
    int nCell = charToCase(c);
    //Tests caractere valide
    if (nCell == 100){
        printf("La valeur entrée n'est pas comprise entre a et f ou A et F\n");
        return 0;
    }
    if (actualPlayer == 1 && nCell > 5){
        printf("Le joueur 1 doit entrer une valeur entre A et F\n");
        return 0;
    }
    if (actualPlayer == 2 && nCell < 6){
        printf("Le joueur 2 doit entrer une valeur entre a et f\n");
        return 0;
    }

    //Test case non vide
    if(g.board.cases[nCell] == 0){
        printf("Vous avez choisi une case vide\n");
        return 0;
    }

    //Test : ne laisse pas l'adversaire en famine
    if (testFamine(g, (actualPlayer%2)+1) == 0){ 
        //si l'adversaire est en famine
        int nbGraine = g.board.cases[nCell];
        int nbMin = 6 - nCell%6;
        if (nbGraine < nbMin){
            printf("L'adversaire n'a pas de graine, vous devez le nourrir\n");
            return 0;
        }
    }
    return 1;
}

/**
 * renvoie la case qui correspond a la lettre : 
 * A : 0, B : 1, ... F : 5, a : 11, b : 10, ... f : 6 
 * renvoie 100 en cas d'erreur
*/
int charToCase(char c)
{
    int numCase = 0;
    switch (c){
        case 'A' : numCase = 0;   break;
        case 'B' : numCase = 1;   break;
        case 'C' : numCase = 2;   break;
        case 'D' : numCase = 3;   break;
        case 'E' : numCase = 4;   break;
        case 'F' : numCase = 5;   break;
        case 'a' : numCase = 11;  break;
        case 'b' : numCase = 10;  break;
        case 'c' : numCase = 9;   break;
        case 'd' : numCase = 8;   break;
        case 'e' : numCase = 7;   break;
        case 'f' : numCase = 6;   break;
        default  : numCase = 100; break;
    }
    return numCase;
}

/**
 * Modifie le plateau
 * Met a jour les scores
*/
void playMove(Game *g, char c){
    int playedCell = charToCase(c);
    int arrivalCell = moveSeeds(&(g->board), playedCell);

    if (playedCell/6 != arrivalCell/6 && //je suis dans le camp adverse
        (g->board.cases[arrivalCell] == 2 || g->board.cases[arrivalCell] == 3)) {
        // et la case d'arrivé contient 2 ou 3 graines

        int dontCompt = 0;
        // Il ne faut pas comptre de point si on famine l'adversaire
        int firstEatenCell = getNbFirstCellEaten(g->board, arrivalCell);
        if(firstEatenCell == 0 || firstEatenCell == 6){
            int totalReste = getNbSeedsAfterEat(g->board, arrivalCell+1);
            if (totalReste == 0)
                dontCompt = 1;
        }

        if (dontCompt == 0){
            int total = emptyBoard(&(g->board), firstEatenCell, arrivalCell);
            if(playedCell < 6){
                // je suis le joueur 1
                g->score1 += total;
            } else {
                g->score2 += total;
            }
        }
    }
}

/**
 * Vide la case donnée
 * Ajoute une graines dans les n cases suivantes
*/
int moveSeeds(Board *b, int numCase)
{
    int nbGrain = b->cases[numCase];
    b->cases[numCase] = 0;
    int caseAct = numCase;
    while(nbGrain > 0){
        caseAct = (caseAct + 1)%12;
        if(caseAct == numCase){
            caseAct = (caseAct + 1)%12;
        }
        b->cases[caseAct]++;
        nbGrain--;
    }
    return caseAct;
}

/**
 * store the move in a string
*/
void saveMove(Board *b, char c){
    if (strlen(b->moves) == 0){
        b->moves[0] = c;
        b->moves[1] = '\0';
    } else {
        char add[] = ", ";
        add[1] = c;
        strcat(b->moves, add);
    }
}

/**
 * return the num of the first eaten cell
*/
int getNbFirstCellEaten(Board b, int arrivedCell){
    int actCell = arrivedCell;
    while(actCell != 0 && actCell != 6 && (b.cases[actCell-1] == 2 || b.cases[actCell-1] == 3)) {
        actCell--;
    }
    return actCell;
}

/**
 * return the number of seed between arrived cell and the end of the side
*/
int getNbSeedsAfterEat(Board b, int arrivedCell){
    int actCell = arrivedCell;
    int total = 0;
    while(actCell != 12 && actCell != 6){
        total += b.cases[actCell];
        actCell++;
    }
    return total;
}
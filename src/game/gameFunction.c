#include "gameFunction.h"
#include <stdlib.h>
#include <stdio.h>

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
    g.score1 = 0;
    g.score2 = 0;
    return g;
}

/**
 * vide le tableau
 * renvoie le nombre de graines restantes
*/
int emptyBoard(Board *b){
    int rest = 0;
    int i;
    for (i = 0 ; i < 12; i++){
        rest = b->cases[i];
        b->cases[i] = 0;
    }
    return rest;
}

/**
 * test si le joueur actuel a des graines de son coté
 * renvoie 2 s'il n'a plus de graines
 * renvoie 0 s'il reste des graines
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
    if (g->score1 > 24 || g->score2 > 24){
        return 1;
    }

    int total = testFamine(*g, actualPlayer);
    if (total == 0){
        // si le joueur ne peut pas jouer car il n'a plus de grain
        // l'adversaire gagne des points
        int rest = emptyBoard(&g->board);
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
 * affiche le board
*/
void showBoard(Board b){
    int i;
    for (i = 65 ; i < 65+6 ; i++){
        printf(" %c", i);
    }
    printf("\n");
    for (i = 0 ; i < 6 ; i++){
        printf("|%d", b.cases[i]);
    }
    printf("|\n");
    for (i = 11 ; i > 5 ; i--){
        printf("|%d", b.cases[i]);
    }
    printf("|\n");
    for (i = 97 ; i < 97+6 ; i++){
        printf(" %c", i);
    }
    printf("\n");
}

/**
 * demande une lettre au joueur
 * verifie qu'il n'y a qu'1 caractere
 * verifie que le caractere est compris entre A et F ou a et f
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
            ch = buff[0];
            if (charToCase(ch) != 100){
                valid = 1;
            }
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
int coupValide(Game g, char c, int actualPlayer){
    if (actualPlayer == 1 && c > 'Z')
        return 0;
    if (actualPlayer == 2 && c < 'Z')
        return 0;

    if (testFamine(g, (actualPlayer%2)+1) == 0){ 
        //si l'adversaire est en famine
        printf("COUPVALIDE : ADVERSAIRE EN FAMINE\n");
        int nCase = charToCase(c);
        int nbGraine = g.board.cases[nCase];
        int nbMin = 6 - nCase%6;
        if (nbGraine < nbMin){
            printf("Ce coup n'est pas calide car il faut nourrir l'adversaire.");
            return 0;
        }
    }
    printf("COUPVALIDE : LE COUP EST VALIDE\n");
    return 1;
}

/**
 * renvoie la case qui correspond a la lattre : 
 * A : 0, B : 1, ... F : 5, a : 11, b : 10, ... f : 6 
 * renvoie 100 en cas d'erreur
*/
int charToCase(char c)
{
    int numCase = 0;
    switch (c){
        case 'A' : numCase = 0; break;
        case 'B' : numCase = 1; break;
        case 'C' : numCase = 2; break;
        case 'D' : numCase = 3; break;
        case 'E' : numCase = 4; break;
        case 'F' : numCase = 5; break;
        case 'a' : numCase = 11; break;
        case 'b' : numCase = 10; break;
        case 'c' : numCase = 9; break;
        case 'd' : numCase = 8; break;
        case 'e' : numCase = 7; break;
        case 'f' : numCase = 6; break;
        default : numCase = 100; break;
    }
    return numCase;
}

/**
 * Modifie le plateau
 * Met a jour les score
*/
void playMove(Game *g, char c, int actualPlayer){
    int caseDepart = charToCase(c);
    int caseArrive = moveSeeds(&(g->board), caseDepart);

    //verfier que si on mange, il n'y a pas une famine
    // manger
    //mettre a jour le score
}

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
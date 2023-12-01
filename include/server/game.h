#ifndef GAME_H
#define GAME_H

/**
 * @brief Plateau de jeu
 * 
 */
typedef struct
{
    int  cases[12];     ///< Cases du plateau
    char moves[1024];   ///< Coups joués
} Board;

/**
 * @brief Partie
 * 
 */
typedef struct
{
    Board board;    ///< Plateau de jeu
    int   score1;   ///< Score du joueur 1
    int   score2;   ///< Score du joueur 2
} Game;

/**
 * @brief Initialise une partie
 * 
 * @return Game Partie
 */
Game initGame();

/**
 * @brief Vide le plateau entre les cases firstCell et lastCell
 * @param b Plateau
 * @param firstCell Première case
 * @param lastCell Dernière case
 * 
 * @return nombre de graines mangées 
 */
int  emptyBoard(Board* b, int firstCell, int lastCell);

/**
 * @brief Teste si le joueur actuel est en famine
 * 
 * @return Board Plateau
 */
int  testFamine(Game g, int actualPlayer);

/**
 * @brief Teste si le joueur actuel a gagné
 * @param g Partie
 * @param actualPlayer Joueur actuel
 * 
 * @return Board Plateau
 */
int  isWin(Game* g, int actualPlayer);

/**
 * @brief Enregistre un coup
 * 
 * @return char Coup
 */
char enterAction();

/**
 * @brief Teste si le coup est valide
 * 
 * @param g Partie
 * @param c Coup
 * @param actualPlayer Joueur actuel 
 * @return int 1 si le coup est valide, 0 sinon
 */
int  validMove(Game g, char c, int actualPlayer);

/**
 * @brief Convertit un caractère en coup
 * 
 * @param c Caractère
 * @return int Coup
 */
int  charToCase(char c);

/**
 * @brief Joue un coup
 * 
 * @param g Partie
 * @param c Coup
 */
void playMove(Game* g, char c);

/**
 * @brief Déplace les graines
 * @param b Plateau
 * @param numCase Case de départ
 * @return int Case d'arrivée
 */
int  moveSeeds(Board* b, int numCase);

/**
 * @brief Enregistre un coup
 * 
 * @param b Plateau
 * @param c Coup
 */
void saveMove(Board* b, char c);

/**
 * @brief Récupérer la première case mangée
 * 
 * @param b Plateau
 * @param arrivedCell Case d'arrivée 
 * @return int La première case mangée
 */
int  getNbFirstCellEaten(Board b, int arrivedCell);

/**
 * @brief Récupérer le nombre de graines après avoir mangé
 * 
 * @param b Plateau
 * @param arrivedCell Case d'arrivée
 * @return int Le nombre de graines après avoir mangé
 */
int  getNbSeedsAfterEat(Board b, int arrivedCell);

#endif
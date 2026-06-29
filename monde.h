#ifndef MONDE_H
#define MONDE_H

/**
 * \brief Largeur de l'écran de jeu
 */
#define SCREEN_WIDTH 800

/**
 * \brief Hauteur de l'écran de jeu
 */
#define SCREEN_HEIGHT 600

/**
 * \brief Taille de la cellule
 */
#define CELL_SIZE 20

/**
 * \brief Nombres de lignes de tableau rockLayout
 */
#define lignes 30

/**
 * \brief Nombres de colonnes de tableau rockLayout
 */

#define colonnes 40

/**
 * \brief Structure représente un point
*/
typedef struct {
    int x, y;
} Point;

/**
 * \brief Structure représente la vitesse du snake
*/
typedef struct {
    int x, y;
} Velocity;

/**
 * \brief Structure représente un segment du snake.
*/
typedef struct SnakeSegment {
    Point position;
    struct SnakeSegment* next;
} SnakeSegment;

/**
 * \brief Structure représente le snake.
*/
typedef struct {
    SnakeSegment* tete;
    SnakeSegment* taille;
    int length;
    Velocity velocity;
} Snake;

/**
 * \brief Structure représente la nourriture
*/
typedef struct {
    Point position;
} Food;

/**
 * \brief Structure représente le lapin
*/
typedef struct {
    int x, y;
} Rabbit;

/**
 * \brief Structure représente les rochers
*/
typedef struct {
    int x, y;
} Rock;



//Variables globales de jeu
extern Snake snake;
extern Rabbit rabbit;
extern int gameover;
extern Food food;
extern int score;
extern int rabbitVisible;
extern int applesEaten ;
extern int NUM_ROCKS;
extern Rock *rocks;



/**
 * \brief fonction qui ajoute des segments au corps du snake
 */
void addBodySegment();

/**
 * \brief fonction qui initialise le serpent
 */
void initialSerpent();

/**
 * \brief fonction qui initialise les rochers depuis un tableau
 */
void initRocksFromLayout();

/**
 * \brief fonction qui controle si la nourriture ou le lapin sont dans la meme position des rochers
 */
int isInsideRocks(int x, int y);

/**
 * \brief fonction pour générer une nouvelle position pour la nourriture de manière aléatoire 
 */
void AlearFood();

/**
 * \brief fonction pour générer une position pour le lapin de manière aléatoire 
 */
void AlearRabbit();

/**
 *\brief fonction pour controler les limites de bornes et le déplacement du snake 
 * \param positionX pointeur qui pointe vers un entier 
 * \param positionY pointeur qui pointe vers un entier 
 */
void ControlLimiteBorne(int* positionX, int* positionY);

/**
 *\brief Fonction pour déplacer le serpent 
 */
void moveSnake();

/**
 *\brief La fonction indique si le serpent est en collision avec les rochers
 */
int checkcollisionWithRocks();

/**
 *\brief fonction indique si le serpent est en collision avec son corps
 */
int checkcollisionWithBody();

/**
 *\brief fonction qui sauvegarde le score dans un fichier
 */
void sauvegarderScore(int score);

#endif //MONDE_H

#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * \brief jouer
 */
#define JOUER 1

/**
 * \brief quitter
 */
#define QUITTER 2

/**
 * \brief Structure représente les boutons
*/
typedef struct {
    SDL_Rect rect;  // Rectangle de position et de taille du bouton
    int action;      // Action associée au bouton (par exemple, JOUER, QUITTER)
} Bouton;


// Variables globales de SDL
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* background ;
extern SDL_Texture* snakeTexture ;
extern SDL_Texture* foodTexture ;
extern SDL_Texture* rabbitTexture ;
extern SDL_Texture* bodyTexture ;
extern SDL_Texture* rocksTexture ;
extern SDL_Texture* texte;
extern SDL_Texture* textureTexteJouer ;
extern SDL_Texture* textureTexteQuitter ;
extern SDL_Texture* menuTexture ;
extern SDL_Texture* scoreTexture ;
extern int lastDirection ;
extern  int dansMenu ;
extern Bouton boutonJouer;
extern Bouton boutonQuitter;

/**
 * \brief fonction pour créer une texture à partir d'un texte
 * \param message le message
 * \param renderer le renderer
 * \param font la font
 * \param color la couleur
 * \return la texture crée
*/
SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer,TTF_Font *font, SDL_Color color);

/**
 * \brief fonction pour gerer l'erreur
 * \param message le message
*/
void SDL_ExitWithError(const char *message);

/**
 * \brief fonction pour gerer l'erreur
 * \param message le message
 * \param window  la fenetre
 * \param rendu le renderer
*/
void SDL_exit(const char *message,SDL_Window *window ,SDL_Renderer *rendu);

/**
 * \brief fonction pour charger une image dans une texture
 * \param nomfichier le nom de l'image
 * \param renderer le renderer
 * \return le texture crée
*/
SDL_Texture* charger_image(const char* nomfichier, SDL_Renderer *renderer);


/**
 * \brief fonction qui initialise le jeu: initialisation de la partie graphique (SDL), chargement des textures, initialisation des données
 */
 void initial();

/**
 * \brief fonction qui initialise les boutons du menu
 */
void initialBouton();

/**
 * \brief Fonction pour gérer les événements de saisie utilisateur
 */
void handleInput();

/**
 * \brief fonction qui va appliquer la texture du menu sur le renderer 
*/
void renderMenu();

/**
 * \brief fonction qui va appliquer la texture snakeTexture associée au snake sur le renderer à la position indiquée dans le snake
*/
void rendersnake();

/**
 * \brief fonction qui va appliquer la texture foodTexture associée au food sur le renderer renderer à la position indiquée dans food
*/
void renderFood();

/**
 * \brief fonction qui va appliquer la texture rabbitTexture associée au s sprite sur le renderer renderer à la position indiquée dans le sprite
*/
void renderRabbit();

/**
 * \brief fonction qui va appliquer la texture text sur le renderer renderer
*/
void rendertext();

/**
 * \brief fonction qui va appliquer la texture scoreTexture sur le renderer 
*/
void renderScore();

/**
 * \brief fonction qui va appliquer la texture rocksTexture associée au rocks sur le renderer 
*/
void renderRocks();

/**
 * \brief fonction qui va appliquer les textures sur le renderer
*/
void render();

/**
 * \brief La fonction qui gère la boucle de jeu
 */
void boucle_jeu();

/**
* \brief fonction qui nettoie le jeu: nettoyage de la partie graphique (SDL), nettoyage des textures, nettoyage des données
*/
void cleanup(); 





#endif //TEXTURE_H

#include "monde.h"
#include "texture.h"
#include <stdio.h>


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* background = NULL;
SDL_Texture* snakeTexture = NULL;
SDL_Texture* foodTexture = NULL;
SDL_Texture* rabbitTexture = NULL;
SDL_Texture* bodyTexture = NULL;
SDL_Texture* rocksTexture = NULL;
SDL_Texture* texte = NULL;
SDL_Texture* menuTexture = NULL;
SDL_Texture* textureTexteJouer = NULL;
SDL_Texture* textureTexteQuitter = NULL;
SDL_Texture* scoreTexture = NULL;
int lastDirection = SDLK_RIGHT;
int dansMenu = 1;
Bouton boutonJouer;
Bouton boutonQuitter;



void SDL_ExitWithError(const char *message){
    SDL_Log("Erreur %s > %s \n",message,SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void SDL_exit(const char *message,SDL_Window *window ,SDL_Renderer *rendu){
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(window);
    SDL_ExitWithError(message);
}


SDL_Texture* charger_image(const char* nomfichier, SDL_Renderer *renderer){
	SDL_Texture *texture = NULL ;
    // Charger une image
    SDL_Surface *surface = NULL;
    // SDL_LoadBMP est utilisée pour charger une image au format BMP depuis un fichier et la convertir en une surface 	   	SDL .Cette surface peut ensuite être utilisée pour le rendu dans une fenêtre SDL.
    surface = SDL_LoadBMP(nomfichier) ;
    // Convertir la surface de l’image au format texture avant de l’appliquer
    texture = SDL_CreateTextureFromSurface(renderer,surface) ;
     //liberer la surface
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer,TTF_Font *font, SDL_Color color){
    SDL_Texture *texture = NULL ;
    SDL_Surface *surface = NULL;
    // Écrire le texte sur une surface SDL
    surface = TTF_RenderText_Solid(font,message,color);
    // Convertir la surface de l’image au format texture avant de l’appliquer
    texture = SDL_CreateTextureFromSurface(renderer,surface) ;
     //liberer la surface
    SDL_FreeSurface(surface);
    return texture;
}

void initial() {
	// Initialisation de la SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) 
        	SDL_ExitWithError("initialisation SDL");

    // Initialisation de SDL_ttf
    if (TTF_Init() < 0) {
        SDL_ExitWithError("initialisation SDL_ttf");
    }

	// Créer la fenêtre
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, 		SDL_WINDOW_SHOWN);
    	if(window == NULL) // En cas d’erreur
       	SDL_ExitWithError("creation de la fenetre echoue");
	   
	// Créer un contexte de rendu (renderer) pour l’image
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED) ;
    	if (renderer==NULL)
        	SDL_ExitWithError("creation du rendu echoue");
	
    background = charger_image("ressources/fond.bmp",renderer);
	if(background==NULL){
        	SDL_exit("impossible de creer la texture",window,renderer);
        }
	
	snakeTexture = charger_image("ressources/s.bmp", renderer);
    	if (snakeTexture == NULL){
        	SDL_exit("Impossible de charger l'image de la tête du serpent", window, renderer);
        	}
	
	foodTexture = charger_image("ressources/apple.bmp", renderer);
    	if (foodTexture == NULL){
        	SDL_exit("Impossible de charger l'image de la nourriture", window, renderer);
        	}
	rabbitTexture = charger_image("ressources/rabbit.bmp", renderer);
    	if (rabbitTexture == NULL){
        	SDL_exit("Impossible de charger l'image du lapin", window, renderer);
        	}
	bodyTexture = charger_image("ressources/body.bmp", renderer);
    	 if (bodyTexture == NULL){
        	SDL_exit("Impossible de charger l'image du corps du serpent", window, renderer);
        	}
	
	rocksTexture = charger_image("ressources/pierre.bmp", renderer);
    	if (rocksTexture == NULL){
        	SDL_exit("Impossible de charger l'image du lapin", window, renderer);
        	}

	menuTexture = charger_image("ressources/menu.bmp", renderer);
    if (menuTexture == NULL)
        SDL_exit("Impossible de charger l'image du menu", window, renderer);
	
	// Initialisation des boutons
    initialBouton();

    // Initialisation des rochers
	initRocksFromLayout();

    // Initialisation du serpent
    initialSerpent();
	
    // Initialisation de la nourriture
	AlearFood();

}

void initialBouton(){ 
	// Initialisation des boutons
    boutonJouer.rect = (SDL_Rect){100, 200, 150, 50};  // Position et taille du bouton "Jouer"
    boutonJouer.action = JOUER;

    boutonQuitter.rect = (SDL_Rect){100, 300, 150, 50};  // Position et taille du bouton "Quitter"
    boutonQuitter.action = QUITTER;
}


void handleInput(){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT){
            gameover = 1;
        }
        if (dansMenu){
            if (e.type == SDL_MOUSEBUTTONDOWN){
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                // Vérifiez si le clic de souris est sur un bouton et effectuez l'action appropriée
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &boutonJouer.rect)) {
                    dansMenu = 0;  // Commencer le jeu
                } else if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &boutonQuitter.rect)) {
                    gameover = 1;  // Quitter le jeu
                }
            }
        }
        else{
            if (e.type == SDL_KEYDOWN) {
			// Vérification de la direction de la touche enfoncée et si elle est opposée à la direction actuelle
            if ((e.key.keysym.sym == SDLK_UP && lastDirection != SDLK_DOWN) ||
                (e.key.keysym.sym == SDLK_DOWN && lastDirection != SDLK_UP) ||
                (e.key.keysym.sym == SDLK_LEFT && lastDirection != SDLK_RIGHT) ||
                (e.key.keysym.sym == SDLK_RIGHT && lastDirection != SDLK_LEFT)) {
					
				// Mise à jour de la dernière direction et de la vélocité du serpent
                lastDirection = e.key.keysym.sym;
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        snake.velocity.x = 0;
                        snake.velocity.y = -CELL_SIZE;
                        break;
                    case SDLK_DOWN:
                        snake.velocity.x = 0;
                        snake.velocity.y = CELL_SIZE;
                        break;
                    case SDLK_LEFT:
                        snake.velocity.x = -CELL_SIZE;
                        snake.velocity.y = 0;
                        break;
                    case SDLK_RIGHT:
                        snake.velocity.x = CELL_SIZE;
                        snake.velocity.y = 0;
                        break;
                    }
                }
            }
        }
    }
}


void rendersnake() {
    // Pointeur vers le segment actuel du serpent, initialisé à la tête du serpent
    SnakeSegment* current = snake.tete;
    // Rendu de la tête du serpent avec rotation
    SDL_Rect headRect = {current->position.x, current->position.y, CELL_SIZE, CELL_SIZE};
    double rotationAngle = 0.0;
    // Détermination de l'angle de rotation en fonction de la direction du serpent
    if (snake.velocity.x > 0) {
        rotationAngle = 0.0; // Aucune rotation pour la droite
    } else if (snake.velocity.x < 0) {
        rotationAngle = 180.0; // Rotation de 180 degrés pour la gauche
    } else if (snake.velocity.y > 0) {
        rotationAngle = 90.0; // Rotation de 90 degrés pour le bas
    } else if (snake.velocity.y < 0) {
        rotationAngle = -90.0; // Rotation de -90 degrés pour le haut
    }
    SDL_RenderCopyEx(renderer, snakeTexture, NULL, &headRect, rotationAngle, NULL, SDL_FLIP_NONE);
    // Passage au segment suivant dans le corps du serpent
    current = current->next;
    // Rendu du reste du corps du serpent
    while (current != NULL) {
        // Rendu du corps du serpent à la position du segment
        SDL_RenderCopy(renderer, bodyTexture, NULL, &(SDL_Rect){current->position.x, current->position.y, CELL_SIZE, CELL_SIZE});
        // Passage au segment suivant dans le corps du serpent
        current = current->next;
    }
}

void renderFood() {
    SDL_RenderCopy(renderer, foodTexture, NULL, &(SDL_Rect){food.position.x, food.position.y, CELL_SIZE, CELL_SIZE});
}

void renderRabbit() {
    SDL_Rect rabbitRect = {rabbit.x, rabbit.y, CELL_SIZE, CELL_SIZE};
    SDL_RenderCopy(renderer, rabbitTexture, NULL, &rabbitRect);
}

void rendertext() {
    TTF_Font *font = TTF_OpenFont("ressources/arial.ttf", 40);
    if (font == NULL) {
        SDL_ExitWithError("Chargement de la police échoué");
    }

    SDL_Color color = {0, 0, 0, 0};
    char msg[40];
    
    if (checkcollisionWithBody() || checkcollisionWithRocks()) {
        sprintf(msg, "Game Over!!! Score: %d", score);
        texte = charger_texte(msg, renderer, font, color);

        int texteW, texteH;
        SDL_QueryTexture(texte, NULL, NULL, &texteW, &texteH);
        if (SDL_QueryTexture(texte, NULL, NULL, &texteW, &texteH) != 0)
            SDL_exit("Impossible de rendre la texture", window, renderer);

        SDL_Rect text_pos;
        text_pos.x = (SCREEN_WIDTH - texteW) / 2;
        text_pos.y = (SCREEN_HEIGHT - texteH) / 2;
        text_pos.w = texteW;
        text_pos.h = texteH;

        SDL_RenderCopy(renderer, texte, NULL, &text_pos);
        TTF_CloseFont(font);
    }
}

void renderScore() {
   TTF_Font* font = TTF_OpenFont("ressources/arial.ttf", 20);
    if (font == NULL) {
        SDL_ExitWithError("Chargement de la police échoué");
    }

    SDL_Color color = {0, 0, 0, 0};
    char scoreText[40];
    sprintf(scoreText, "Score: %d", score);

    SDL_Texture* scoreTexture = charger_texte(scoreText, renderer, font, color);
    if (scoreTexture == NULL) {
        SDL_ExitWithError("Impossible de charger la texture du score");
    }

    // Rendu du rectangle rouge
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect scoreRect = {0, 0, 100, 30}; // Position et taille du rectangle rouge
    SDL_RenderFillRect(renderer, &scoreRect);

    // Positionner le texte dans le coin supérieur gauche du rectangle rouge
    int scoreTextW, scoreTextH;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreTextW, &scoreTextH);
    SDL_Rect scorePos;
    scorePos.x = scoreRect.x + 5; // Légèrement décalé à droite pour plus d'espace
    scorePos.y = scoreRect.y + (scoreRect.h - scoreTextH) / 2; // Centrer le texte verticalement
    scorePos.w = scoreTextW;
    scorePos.h = scoreTextH;

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

    SDL_DestroyTexture(scoreTexture);
    TTF_CloseFont(font);

    // Rétablir la couleur du rendu à blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void renderRocks() {
    for (int i = 0; i < NUM_ROCKS; i++) {
        SDL_RenderCopy(renderer, rocksTexture, NULL, &(SDL_Rect){rocks[i].x, rocks[i].y, CELL_SIZE, CELL_SIZE});
    }
}


void renderMenu() {
    // Effacement du rendu précédent
    SDL_RenderClear(renderer);

    // Définir la couleur du rendu à rouge
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Rendu de la texture du menu sur toute la surface de la fenêtre
    SDL_RenderCopy(renderer, menuTexture, NULL, NULL);

    // Chargement d'une police de caractères
    TTF_Font* police = TTF_OpenFont("ressources/arial.ttf", 24);
    if (police == NULL) {
        SDL_ExitWithError("chargement police echoue");
    }
    // Couleur du texte (blanc)
    SDL_Color couleurTexte = {255, 255, 255, 255};

    // Rendu du texte sur les boutons
    textureTexteJouer = charger_texte("Jouer",renderer,police,couleurTexte);
    textureTexteQuitter = charger_texte("Quitter",renderer,police,couleurTexte);

    SDL_Rect destRectJouer = {boutonJouer.rect.x + 10, boutonJouer.rect.y + 10, boutonJouer.rect.w - 20, boutonJouer.rect.h - 20};
    SDL_Rect destRectQuitter = {boutonQuitter.rect.x + 10, boutonQuitter.rect.y + 10, boutonQuitter.rect.w - 20, boutonQuitter.rect.h - 20};

    // Dessiner les boutons avec la couleur rouge
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &boutonJouer.rect);
    SDL_RenderFillRect(renderer, &boutonQuitter.rect);

    // Rétablir la couleur du rendu à blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderCopy(renderer, textureTexteJouer, NULL, &destRectJouer);
    SDL_RenderCopy(renderer, textureTexteQuitter, NULL, &destRectQuitter);

    // Affichage du rendu à l'écran
    SDL_RenderPresent(renderer);

    // Libération de la police de caractères
    TTF_CloseFont(police);
}


void render() {
    SDL_RenderClear(renderer);

    if (dansMenu) {
        renderMenu();
    } else {
        // Afficher uniquement le fond
        SDL_RenderCopy(renderer, background, NULL, NULL);

        if (checkcollisionWithBody() || checkcollisionWithRocks()) {
            rendertext();  // Afficher le message "Game Over" et le score
            SDL_RenderPresent(renderer);
            return;  // Arrêter ici si le serpent est mort
        }

        // Reste du rendu du jeu (snake, food, rabbit, rocks, score, etc.)
        rendersnake();
        renderFood();
        if (rabbitVisible) {
            renderRabbit();
        }
        renderRocks();
        renderScore();
    }

    SDL_RenderPresent(renderer);
}


void boucle_jeu() {
  while (!gameover) {
        handleInput();

        if (dansMenu) {
            renderMenu();
        } else {
            moveSnake();
            render();
        }
        SDL_Delay(100);
    }
}


void cleanup() {
    // Libérer les segments du serpent
    SnakeSegment* currentSegment = snake.tete;
    while (currentSegment != NULL) {
        SnakeSegment* temp = currentSegment;
        currentSegment = currentSegment->next;
        free(temp);
    }
	 free(rocks);
    // Libérer les textures SDL
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(snakeTexture);
	SDL_DestroyTexture(rabbitTexture);
	SDL_DestroyTexture(bodyTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyTexture(rocksTexture);
    SDL_DestroyTexture(menuTexture);
    SDL_DestroyTexture(texte);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(textureTexteJouer);
    SDL_DestroyTexture(textureTexteQuitter);
    // Libérer le rendu et la fenêtre
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quitter SDL
    SDL_Quit();
}


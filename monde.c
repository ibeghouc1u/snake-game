#include "monde.h"
#include <stdlib.h>
#include <stdio.h>


Snake snake;
Rabbit rabbit;
int gameover = 0;
Food food;
int score = 0;
int rabbitVisible = 0;
int applesEaten = 0;
int NUM_ROCKS = 400;
Rock *rocks = NULL;

char rockLayout[lignes][colonnes] = {
    "4444440000000000000000000000000000000000",
    "4444440000000000000000000000000000000000",
    "4000000000000000000000000000000000000000",
    "4000000000000000000000000000000000000000",
    "4000000000000000000000000000000000000000",
    "4000000000000000000000000000000000000000",
    "0000000044444444444444444444440000000004",
    "0000000040000000000000000000040000000004",
    "0000000040000000000000000000040000000004",
    "0000000040000000000000000000040000000004",
    "0000000040000444444444440000040000000004",
    "0000000040000400000000040000040000000004",
    "0000000040000400000000040000040000000000",
    "0000000040000400000000040000040000000000",
    "0000000040000444440000040000040000000000",
    "0000000040000000000000040000040000000000",
    "0000000040000000000000040000040000000000",
    "0000000040000000000000040000040000000000",
    "0000000040000000000000040000040000000004",
	"0000000044444444444444440000040000000004",
    "0000000000000000000000000000000000000004",
    "0000000000000000000000000000000000000004",
	"0000000000000000000000000000000000000004",
    "0000000000000000000000000000000000000004",
    "0000000000000000000000000000000000000000",
	"0000000000000000000000000000000000000000",
    "0000000000000000000000000000000000000000",
    "0000000000000000000000000000000000000000",
    "0000000000000000000000000000000000000000",
    "0000000000044444444444444444400000000000"
};

void addBodySegment() {
    // Création d'un nouveau segment du corps du serpent
    SnakeSegment* newBodySegment = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    newBodySegment->position.x = snake.tete->position.x;
    newBodySegment->position.y = snake.tete->position.y;
    newBodySegment->next = NULL;

    // Si le serpent n'a pas de queue, le nouveau segment devient la tête et la queue
    if (snake.taille == NULL) {
        snake.tete->next = newBodySegment;
        snake.taille = newBodySegment;
    } else {
        // Ajout du nouveau segment à la fin du corps du serpent
        SnakeSegment* current = snake.tete;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newBodySegment;
        snake.taille = newBodySegment;
    }

    // Augmentation de la longueur du serpent
    snake.length += 1;
}


void initialSerpent() {
    snake.tete = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    snake.tete->position.x = CELL_SIZE;  // Déplacer vers la gauche pour laisser de l'espace à gauche
    snake.tete->position.y = SCREEN_HEIGHT - CELL_SIZE*4;  // Déplacer vers le bas
    snake.tete->next = NULL;
    snake.taille = snake.tete;
    snake.length = 1;

    // Ajouter des segments de corps supplémentaires
    for (int i = 1; i < 3; i++) {
        addBodySegment();
    }

    // Définir la direction initiale du serpent vers la droite
    snake.velocity.x = CELL_SIZE;
    snake.velocity.y = 0;
}

void initRocksFromLayout() {
    rocks = malloc(NUM_ROCKS * sizeof(Rock));  // Allouer de la mémoire pour les rochers
	if (rocks == NULL) {
        // Gestion de l'erreur d'allocation mémoire
        fprintf(stderr, "Erreur d'allocation mémoire pour rocks\n");
        exit(EXIT_FAILURE);
    }

    int numRocks = 0;  // Utiliser une variable pour suivre le nombre de rochers

    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes ; j++) {
            if (rockLayout[i][j] == '4') {
                rocks[numRocks].x = j * CELL_SIZE;
                rocks[numRocks].y = i * CELL_SIZE;
                numRocks++;
            }
        }
    }
}

int isInsideRocks(int x, int y) {
    for (int i = 0; i < NUM_ROCKS; i++) {
        if (x >= rocks[i].x && x < rocks[i].x + CELL_SIZE &&
            y >= rocks[i].y && y < rocks[i].y + CELL_SIZE) {
            return 1; // La position est à l'intérieur d'un rocher
        }
    }
    return 0; // La position n'est pas à l'intérieur des rochers
}


void AlearFood() {
    do {
        // Génération de nouvelles coordonnées pour la nourriture à l'intérieur des limites de l'écran
        food.position.x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
        food.position.y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
    } while (isInsideRocks(food.position.x, food.position.y));

}

int checkcollisionWithBody() {
    SnakeSegment* current = snake.tete->next;
	
	// Parcours de chaque segment du corps du serpent
    while (current != NULL) {
		// Vérification de la collision avec la tête du serpent
        if (snake.tete->position.x == current->position.x && snake.tete->position.y == current->position.y) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void AlearRabbit() {
    do {
        // Génération de nouvelles coordonnées pour le lapin à l'intérieur des limites de l'écran
        rabbit.x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
        rabbit.y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
    } while (isInsideRocks(rabbit.x, rabbit.y));
}

void ControlLimiteBorne(int* positionX, int* positionY) {
    if (*positionX >= SCREEN_WIDTH) {
        *positionX = 0;
    } else if (*positionX < 0) {
        *positionX = SCREEN_WIDTH - CELL_SIZE;
    }

    if (*positionY >= SCREEN_HEIGHT) {
        *positionY = 0;
    } else if (*positionY < 0) {
        *positionY = SCREEN_HEIGHT - CELL_SIZE;
    }
}


int checkcollisionWithRocks(){
    for (int i = 0 ;i<NUM_ROCKS ; i++){
        if(snake.tete->position.x == rocks[i].x && snake.tete->position.y == rocks[i].y){
            return 1;
        }
    }
    return 0;
}

void moveSnake() {
    // Création d'une nouvelle tête du serpent
    SnakeSegment* newtete = (SnakeSegment*)malloc(sizeof(SnakeSegment));
    newtete->position.x = snake.tete->position.x + snake.velocity.x;
    newtete->position.y = snake.tete->position.y + snake.velocity.y;
    newtete->next = snake.tete;
    snake.tete = newtete;


	// Vérification de la collision avec de la nourriture
    if (snake.tete->position.x == food.position.x && snake.tete->position.y == food.position.y) {
        score++;
        applesEaten++;
	    addBodySegment();

        // Si un certain nombre de pommes ont été mangées, le lapin apparaît et le serpent grandit
        if (applesEaten == 3) {
            rabbitVisible = 1;
			applesEaten = 0;
			AlearRabbit();
			
        }

        // Spawn d'une nouvelle nourriture
        AlearFood();
    } 
    // Vérification de la collision avec le lapin
    if (snake.tete->position.x == rabbit.x && snake.tete->position.y == rabbit.y) {
        score += 5;
        rabbitVisible = 0;
	    addBodySegment();
 
    }
    
    SnakeSegment* current = snake.tete;
    while (current->next != NULL && current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
    
	// Correction pour le wrap-around de la tête du serpent
    ControlLimiteBorne(&(snake.tete->position.x), &(snake.tete->position.y));

    // Vérification des collisions avec le corps du serpent 
    if (checkcollisionWithRocks()) {
        gameover = 1; 
	}
     // Vérification des collisions avec les rochers
    if (checkcollisionWithBody()) {
        gameover = 1;
    }
	
	
	// Mise à jour des positions du corps du serpent
    while (current != NULL) {
        ControlLimiteBorne(&(current->position.x), &(current->position.y));
        current = current->next;
    }
}

void sauvegarderScore(int score) {
    FILE *fichier = fopen("score.txt", "w"); // Ouvrir le fichier en mode écriture

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Écrire le score dans le fichier
    fprintf(fichier, "score :%d", score);

    // Fermer le fichier
    fclose(fichier);
}




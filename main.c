#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "monde.h"
#include "texture.h"


int main(int argc, char* argv[]) {
    initial();
    boucle_jeu();
    printf("Game over! Your score: %d\n", score);
    sauvegarderScore(score);
    SDL_Delay(900);
    cleanup();
    return 0;
}

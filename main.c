#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <math.h>
#include <time.h>
#include "boucle_jeu.h"

void lancertoutallegro()
{
    // initialisation allegro obligatoire
    allegro_init();

    // pour disposer du clavier
    install_keyboard();

    // définir un mode graphique
    // détection et sortie anticipée en cas de problème
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}
int main()
{

    lancertoutallegro();
    srand(time(NULL));

    int ret;

    ret=jouer(1);

    if(ret==1)
    {
        printf("Gagné!");
    }
    else if (ret==0)
    {
        printf("Perdu");
    }
    else
    {
        printf("Abandon");
    }


    return 0;
}
END_OF_MAIN();

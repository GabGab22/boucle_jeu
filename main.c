#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <math.h>
#include "boucle_jeu.h"





void menu();

BITMAP * load_bitmap_check(char *nomImage)
{
    BITMAP *bmp;
    bmp=load_bitmap(nomImage,NULL);
    if (!bmp)
    {
        allegro_message("image non trouvee %s",nomImage);
        exit(EXIT_FAILURE);
    }
    return bmp;
}




void musique ()
{
    MIDI* musique;
    int i ;

    musique=load_midi("Musiques/Musique.mid");
    if(!musique) printf("erreur");

    play_midi(musique,10);

    for(i=0; i<200; i++) rest(10);
}

void sous_menu_jouer()
{
    BITMAP *sous_menu_jouer;
    sous_menu_jouer = load_bitmap_check("Images/sous_menu_jouer.bmp");

    BITMAP *page;

    page=create_bitmap(0,0);

    clear_bitmap(page);

    int fin_sous_menu_jouer = 0;
    int choix=0;

    blit(sous_menu_jouer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    while(fin_sous_menu_jouer!=1)
    {
        if((mouse_b&1) && mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+290) && mouse_y>=SCREEN_H/2.4 && mouse_y<=(SCREEN_H/2.4+60))
        {
            choix = 1;
        }
        if((mouse_b&1) && mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+290) && mouse_y>=SCREEN_H/1.76 && mouse_y<=(SCREEN_H/1.76+60))
        {
            choix = 2;
        }
        if((mouse_b&1) && mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+290) && mouse_y>=SCREEN_H/1.43 && mouse_y<=(SCREEN_H/1.43+60))
        {
            choix = 3;
        }
        switch(choix)
        {
        case 1 :
            jouer(1);
            break;
        case 2 :
            jouer(2);
            break;
        case 3 :
            jouer(3);
            break;
        default :
            break;
        }
        if(key[KEY_ESC])
            {
                menu();
            }
    }
}

/*void sous_menu_jouer()
{
    BITMAP *fond_menu;
    fond_menu = load_bitmap_check("Images/Fond_Menu.bmp");
    BITMAP *page;
    page=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *Niveau1;
    Niveau1 = load_bitmap_check("Images/Niveau1.bmp");
    BITMAP *Niveau2;
    Niveau2 = load_bitmap_check("Images/Niveau2.bmp");
    BITMAP *Niveau3;
    Niveau3 = load_bitmap_check("Images/Niveau3.bmp");
    int fin_sous_menu_jouer = 0;
    int choix;
    clear_bitmap(page);
    draw_sprite(page, fond_menu, 0, 0);
    draw_sprite(page, Niveau1, SCREEN_W/2.66, SCREEN_H/3);
    draw_sprite(page, Niveau2, SCREEN_W/2.66, SCREEN_H/2);
    draw_sprite(page, Niveau3, SCREEN_W/2.66, SCREEN_H/1.5);
    blit(page, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
    while(fin_sous_menu_jouer!=1)
    {
        if( ( (mouse_x>=SCREEN_W/2.66) && (mouse_x<=(SCREEN_W/2.66+260)) && (mouse_y>=SCREEN_H/3) && (mouse_y<=(SCREEN_H/3+50)) ) && (mouse_b & 1) )
        {
                choix = 1;
        }
        if((mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+260) && mouse_y>=SCREEN_H/2 && mouse_y<=(SCREEN_H/2+50)) && mouse_b & 1)
        {
                choix = 2;
        }
        if((mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+260) && mouse_y>=SCREEN_H/1.5 && mouse_y<=(SCREEN_H/1.5+50)) && mouse_b & 1)
        {
                choix = 3;
        }
            if(key[KEY_ESC])
            {
                menu();
            }
        }
        switch(choix)
        {
        case 1 :
            jouer(1);
            break;
        case 2 :
            jouer(2);
            break;
        case 3 :
            jouer(3);
            break;
        default :
            break;
        }
        if(key[KEY_ESC])
        {
            fin_sous_menu_jouer=1;
        }
}*/

void afficher_credits()
{
    BITMAP *afficher_credits;
    afficher_credits = load_bitmap_check("Images/Afficher_Credits_Menu.bmp");
    BITMAP *page;
    page=create_bitmap(0,0);
    clear_bitmap(page);
    int fin_afficher_credits = 0;
    blit(afficher_credits, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    while(fin_afficher_credits!=1)
    {
        if(key[KEY_ESC])
            {
                clear_bitmap(page);
                menu();
            }
    }
}

void afficher_regles()
{
    BITMAP *afficher_regles;
    afficher_regles = load_bitmap_check("Images/Afficher_Regles_Menu.bmp");
    BITMAP *page;
    page=create_bitmap(0,0);
    clear_bitmap(page);
    int fin_afficher_regles = 0;
    blit(afficher_regles, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    blit(page, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
    while(fin_afficher_regles!=1)
    {
        if(key[KEY_ESC])
            {
                clear_bitmap(page);
                menu();
            }
    }
}

void menu()
{
    BITMAP *fond_menu;
    fond_menu = load_bitmap_check("Images/Fond_Menu.bmp");
    BITMAP *page;
    page=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *NOM;
    NOM = load_bitmap_check("Images/Nom_Menu.bmp");
    BITMAP *JOUER;
    JOUER = load_bitmap_check("Images/Jouer_Menu.bmp");
    BITMAP *REGLES;
    REGLES = load_bitmap_check("Images/Regles_Menu.bmp");
    BITMAP *CREDITS;
    CREDITS = load_bitmap_check("Images/Credits_Menu.bmp");
    BITMAP *QUITTER;
    QUITTER = load_bitmap_check("Images/Quitter_Menu.bmp");
    int fin_menu = 0;
    int choix;
    clear_bitmap(page);
    draw_sprite(page, fond_menu, 0, 0);
    draw_sprite(page, NOM, SCREEN_W/8, SCREEN_H/60);
    draw_sprite(page, JOUER, SCREEN_W/2.66, SCREEN_H/3.33);
    draw_sprite(page, REGLES, SCREEN_W/2.66, SCREEN_H/2.31);
    draw_sprite(page, CREDITS, SCREEN_W/2.66, SCREEN_H/1.76);
    draw_sprite(page, QUITTER, SCREEN_W/2.66, SCREEN_H/1.43);
    blit(page, screen, 0,0,0,0,SCREEN_W, SCREEN_H);
    while(fin_menu!=1)
    {
        rest(250);
        clear_keybuf();

        if( ( (mouse_x>=SCREEN_W/2.66) && (mouse_x<=(SCREEN_W/2.66+260)) && (mouse_y>=SCREEN_H/3.33) && (mouse_y<=(SCREEN_H/3.33+50)) ) && (mouse_b & 1) )
        {
                choix = 1;
        }
        if((mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+260) && mouse_y>=SCREEN_H/2.31 && mouse_y<=(SCREEN_H/2.31+50)) && mouse_b & 1)
        {
                choix = 2;
        }
        if((mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+260) && mouse_y>=SCREEN_H/1.76 && mouse_y<=(SCREEN_H/1.76+50)) && mouse_b & 1)
        {
                choix = 3;
        }
        if((mouse_x>=SCREEN_W/2.66 && mouse_x<=(SCREEN_W/2.66+260) && mouse_y>=SCREEN_H/1.43 && mouse_y<=(SCREEN_H/1.43+50)) && mouse_b & 1)        {

            {
                choix = 4;
            }
            if(key[KEY_ESC])
            {
                exit(EXIT_SUCCESS);
            }
        }
        switch(choix)
        {
        case 1 :
            sous_menu_jouer();
            break;
        case 2 :
            afficher_regles();
            break;
        case 3 :
            afficher_credits();
            break;
        case 4 :
            exit(EXIT_SUCCESS);
            break;
        default :
            break;
        }
        if(key[KEY_ESC])
        {
            fin_menu=1;
        }
    }
}

int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen);

    while(!key[KEY_ESC])
    {
        musique();
        menu();
    }
    rest(100);
    return 0;
}
END_OF_MAIN();

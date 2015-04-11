#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <math.h>

#define SCROLL 5
#define REST 20
#define PROB 100
#define DEPXPERSO 3
#define DEPYPERSO 3
#define DEPMX 5
typedef struct Mur
{
    BITMAP*image;
    int posx;
    int depx;
    int fin;

} t_Mur;

#define MORT 1
#define VIVANT 0
typedef struct Perso
{
    BITMAP*image;
    int posx;//position des x du joueur
    int posy;//position des y du joueur
    int depx;
    int depy;
    int etat; //MORT ou VIVANT
    int fin; //0 ou 1

} t_Perso;

typedef struct Ennemi
{
    float posx;
    int posy;
    float depy;
    int etat;
    BITMAP*image;

} t_Ennemi;

typedef struct Boss
{
    BITMAP*image;
    int posx;
    int posy;
    int depx;
    int depy;
    int fin;

} t_Boss;

typedef struct tir
{
    BITMAP*image;
    int posmx;
    int posmy;
    int depmx;
    int etat;
    struct tir* suiv;
} t_tir;

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

t_Perso* AllouePerso()
{
    t_Perso*perso;
    //allocation dynamique de la structure joueur
    perso=(t_Perso*)malloc(1*sizeof(t_Perso));
    //initialisation de la structure
    perso->image=load_bitmap_check("Globule_Blanc.bmp");
    perso->posx=100;
    perso->posy=200;
    perso->depx=DEPXPERSO;
    perso->depy=DEPYPERSO;
    perso->fin=0;
    perso->etat=VIVANT;
    return perso;

}

t_Mur* AlloueMur(int niveau)
{
    t_Mur*mur;

    mur=(t_Mur*)malloc(1*sizeof(t_Mur));
    switch (niveau)
    {
    case 1:

        mur->image=load_bitmap_check("murs_niveau1.bmp");

        break;
    case 2:
        mur->image=load_bitmap_check("murs_niveau2.bmp");
        break;
    case 3:
        mur->image=load_bitmap_check("murs_niveau3.bmp");
        break;
    }
    mur->posx=0;
    mur->depx=SCROLL;
    mur->fin=0;
    return mur;

}


void Affichage(BITMAP*fond,t_Mur*mur, t_Perso *perso,t_Ennemi tabEnnemi[25],t_Boss*boss,t_tir*tirPerso,t_tir*tirEnnemi)
{

    BITMAP *page; //création d'un double buffer
    int i;
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);
    t_tir* courant;
    blit(fond,page,0,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(mur->image,page,mur->posx,0,0,0,SCREEN_W,SCREEN_H);

    //affichage des ennemis
    for(i=0; i<25; i++)
    {
        if(tabEnnemi[i].etat)
        {
            masked_blit(tabEnnemi[i].image,page,0,0,tabEnnemi[i].posx-mur->posx,tabEnnemi[i].posy,tabEnnemi[i].image->w,tabEnnemi[i].image->h);
            //affichage des tirs ennemis
            courant=tirEnnemi;
            while(courant!=NULL)
            {
                if(courant->etat==1)
                {
                    masked_blit(courant->image,page,0,0,courant->posmx,courant->posmy,courant->image->w,courant->image->h);
                }
                courant=courant->suiv;

            }
        }

    }

    //affichage du boss si on est en fin de niveau
    if(boss!=NULL)
    {
        masked_blit(boss->image,page,0,0,boss->posx,boss->posy,boss->image->w,boss->image->h);
    }

    courant=tirPerso;
    //affichage des tirs du personnage
    while(courant!=NULL)
    {
        if(courant->etat==1)
        {
            masked_blit(courant->image,page,0,0,courant->posmx,courant->posmy,courant->image->w,courant->image->h);

        }

        courant=courant->suiv;
    }



    masked_blit(perso->image,page,0,0,perso->posx,perso->posy,perso->image->w,perso->image->h);//affichage du personnage


    //Affichage buffer
    blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);


    destroy_bitmap(page);
}

void DeplacementPerso(t_Perso*perso)
{
    if(perso->etat==VIVANT)
    {
        if(key[KEY_UP]) //si on appuie sur la touche haut
        {
            if(perso->posy>0)
                perso->posy=perso->posy-(perso->depy);//l'ordonnée est diminué

        }
        else if (key[KEY_DOWN]) //si on appuie sur la touche du bas
        {
            if(perso->posy<SCREEN_H-perso->image->h)
                perso->posy=perso->posy+perso->depy; //l'ordonné est augmenté

        }
        else if(key[KEY_LEFT])// si on appuie su rla touche de gauche
        {
            if(perso->posx>0)
                perso->posx=perso->posx-perso->depx; //l'abscisse est diminué

        }
        else if(key[KEY_RIGHT])// si on appuie sur la touche de droite
        {
            if(perso->posx<SCREEN_W-perso->image->w)
                perso->posx=perso->posx+perso->depx;//l'abscisse est augmenté

        }
    }
    else //si il y a eu collision avec un mur ou un missile
    {
        perso->posy=perso->posy+10; //le personage n'est pus déplacé par le joueur
        if (perso->posy>SCREEN_H)  //il desend tout seul
        {
            perso->fin=1;//quand il a disparu de l'écran son état change pour qu'on puisse sortir de la boucle de jeu
        }
    }
}

void ScrollingMur(t_Mur*mur,t_Perso*perso)
{
    if(perso->etat==VIVANT)//si le personnage est vivant
    {
        mur->posx=mur->posx+mur->depx;//les murs se déplacent

        if(mur->posx>mur->image->w-SCREEN_W)//si on arrive au bout de l'image
        {
            mur->fin=1; //changement d'état pour signifier qu'on arrête le scrollling
        }
    }
}

void Collision(t_Perso*perso,t_Mur*mur,t_tir*tirPerso,t_tir*tirEnnemi,t_Ennemi tabEnnemi[25])
{
    //il y a collision personnage/murs si le pixel de mur
    // aux positions données du personnage est noir
    // le personnage étant une ellipse on a décidé de regarder
    //les pixels situés à pi/4 +k*pi/2 du centre du perso
    if((getpixel(mur->image,mur->posx+perso->posx+(perso->image->w)*(0.35+0.5),perso->posy+(perso->image->h)*(0.35+0.5))==0))
    {
        perso->etat=MORT;
    }
    else if((getpixel(mur->image,mur->posx+perso->posx+(perso->image->w)*(-0.35+0.5),perso->posy+(perso->image->h)*(-0.35+0.5))==0))
    {
        perso->etat=MORT;
    }
    else if((getpixel(mur->image,mur->posx+perso->posx+(perso->image->w)*(-0.35+0.5),perso->posy+(perso->image->h)*(0.35+0.5))==0))
    {
        perso->etat=MORT;
    }
    else if((getpixel(mur->image,mur->posx+perso->posx+(perso->image->w)*(0.35+0.5),perso->posy+(perso->image->h)*(-0.35+0.5))==0))
    {
        perso->etat=MORT;
    }

    //collision tirEnnemi/perso
    //on calcule la distance entre le milieu du personnage et le milieu du missile
    //si cette distance est inférieure à une constante le personnage meurt et le missile disparait
    t_tir*courant;
    courant=tirEnnemi;

    while(courant!=NULL)
    {
        if(sqrt(pow(perso->posx+perso->image->w/2-courant->posmx-courant->image->w/2,2)+pow(perso->posy+perso->image->h/2-courant->posmy-courant->image->h/2,2))<30)
        {
            perso->etat=MORT;
            courant->etat=0;
        }
        courant=courant->suiv;
    }

    //collision tirPerso/Ennemi

    courant=tirPerso;
    int i;

    while(courant!=NULL)
    {
        for(i=0; i<25; i++)
        {
            if(tabEnnemi[i].etat==1 && courant->etat==1)
            {
                if(sqrt(pow(tabEnnemi[i].posx+tabEnnemi[i].image->w/2-mur->posx-courant->posmx-courant->image->w/2,2)+pow(tabEnnemi[i].posy+tabEnnemi[i].image->h/2-courant->posmy-courant->image->h/2,2))<30)
                {
                    tabEnnemi[i].etat=0;
                    courant->etat=0;
                }
            }

        }
        courant=courant->suiv;
    }

    //collision perso/ennemis

    for(i=0; i<25; i++)
    {
        if(tabEnnemi[i].etat==1)
        {
            if(sqrt(pow(perso->posx+perso->image->w/2-tabEnnemi[i].posx-tabEnnemi[i].image->w/2+mur->posx,2)+pow(perso->posy+perso->image->h/2-tabEnnemi[i].posy-tabEnnemi[i].image->h/2,2))<30)
            {
                perso->etat=MORT;
                tabEnnemi[i].etat=0;
            }
        }

    }


}

void DeplacementEnnemis(t_Ennemi tabEnnemi[25],t_Mur*mur)
{
    int i;

    for (i=0; i<25; i++)
    {
        if(tabEnnemi[i].etat==1)
        {
            if(mur->posx+mur->image->w>tabEnnemi[i].posx)
            {
                tabEnnemi[i].posy=tabEnnemi[i].posy+tabEnnemi[i].depy;

                if(tabEnnemi[i].posy<0)
                {
                    tabEnnemi[i].depy=-tabEnnemi[i].depy;
                }
                if(tabEnnemi[i].posy+tabEnnemi[i].image->h>SCREEN_H)
                {
                    tabEnnemi[i].depy=-tabEnnemi[i].depy;
                }

            }
        }

    }

}

t_Boss* AlloueBoss()
{
    t_Boss*boss;

    boss=(t_Boss*)malloc(1*sizeof(t_Boss));
    boss->posx=350;
    boss->posy=100;
    boss->fin=0;

    return boss;
}

t_tir*TirPerso(t_tir*tirPerso,t_Perso*perso,BITMAP*munitionPerso)
{
    if(key[KEY_SPACE])
    {
        t_tir*nouv;

        nouv=(t_tir*)malloc(1*sizeof(t_tir));
        nouv->image=munitionPerso;
        nouv->posmx=perso->posx+perso->image->w-10;
        nouv->posmy=perso->posy+perso->image->h-40;
        nouv->depmx=DEPMX;
        nouv->etat=1;
        nouv->suiv=tirPerso;
        return nouv;
    }
    return tirPerso;
}
///////////////////////////////////////////////////
t_tir* DeplacementMunition(t_tir*tir)
{
    t_tir*courant;
    courant=tir;
    t_tir*prec=NULL;

    while(courant!=NULL)
    {
        if(courant->etat==1)
        {
            courant->posmx=courant->posmx+courant->depmx;

            if((courant->posmx>SCREEN_W) || (courant->posmx<0))
            {
                courant->etat=0;

            }
        }



        if(courant->etat==0)
        {
            if(courant==tir)
            {
                tir=tir->suiv;
                free(courant);
                courant=tir;
            }
            else
            {
                prec->suiv=courant->suiv;
                free(courant);
                courant=prec->suiv;
            }

        }
        else
        {
            prec=courant;
            courant=courant->suiv;
        }

    }
    return tir;
}
////////////////////////////////////////////////////////////////
void DeplacementBoss(t_Boss*boss)
{
    if ( rand()%20==0 )
    {
        // Nouveau vecteur déplacement
        boss->depy=rand()%(12-5);
        boss->depx=rand()%(12-5);
    }
    if(boss->fin==VIVANT)
    {
        if((boss->posx<SCREEN_W/2 && boss->depx<0) || (boss->posx+boss->image->w>SCREEN_W && boss->depx>0))
        {
            boss->depx=-boss->depx;
        }
        if((boss->posy<0 && boss->depy<0) || (boss->posy+boss->image->h>SCREEN_H && boss->depy))
        {
            boss->depy=-boss->depy;
        }
        boss->posx= boss->posx+ boss->depx;
        boss->posy= boss->posy+ boss->depy;
    }


}

t_tir* TirEnnemi(t_tir* tirEnnemi,t_Ennemi tabEnnemi[25],BITMAP*munitionEnnemie,t_Mur* mur)
{
    int i;
    t_tir*ancre=NULL;

    for(i=0; i<25; i++)
    {
        if(tabEnnemi[i].etat)
        {
            if(rand()%PROB==0)
            {
                ancre=(t_tir*)malloc(1*sizeof(t_tir));
                ancre->image=munitionEnnemie;
                ancre->etat=1;
                ancre->depmx=-SCROLL-DEPMX;
                ancre->posmx=tabEnnemi[i].posx-mur->posx;
                ancre->posmy=tabEnnemi[i].posy+tabEnnemi[i].image->h/2;
                ancre->suiv=tirEnnemi;
                tirEnnemi=ancre;
            }
        }

    }

    return tirEnnemi;
}

int jouer(int niveau )
{
    BITMAP *fond;
    t_Perso *perso;
    t_Mur *mur;
    BITMAP *bacterie1,*bacterie2,*bacterie3;
    t_Boss *boss;
    int i;
    t_tir *tirPerso=NULL;
    BITMAP *munitionPerso;
    munitionPerso=load_bitmap_check("munition2.bmp");
    BITMAP *munitionEnnemie;
    munitionEnnemie=load_bitmap_check("laser.bmp");
    t_Ennemi tabEnnemi[25];
    t_tir*tirEnnemi=NULL;


    for(i=0; i<25; i++)
    {
        tabEnnemi[i].etat = 0;
    }

    bacterie1=load_bitmap_check("Bacterie_4.bmp");
    bacterie2=load_bitmap_check("Bacterie_6.bmp");
    bacterie3=load_bitmap_check("Bacterie_11.bmp");

    switch (niveau)
    {
    case 1:
        //chargement du niveau 1,fond, mur,....
        fond = load_bitmap_check("fond_rouge.bmp");
        tabEnnemi[0].posx=1593.;
        tabEnnemi[0].posy=160;
        tabEnnemi[0].depy=2;
        tabEnnemi[0].etat=1;
        tabEnnemi[0].image=bacterie1;

        tabEnnemi[1].posx=1633.;
        tabEnnemi[1].posy=270;
        tabEnnemi[1].depy=-1;
        tabEnnemi[1].etat=1;
        tabEnnemi[1].image=bacterie2;

        tabEnnemi[2].posx=1825.;
        tabEnnemi[2].posy=150;
        tabEnnemi[2].depy=1;
        tabEnnemi[2].etat=1;
        tabEnnemi[2].image=bacterie1;

        tabEnnemi[3].posx=2905.;
        tabEnnemi[3].posy=225;
        tabEnnemi[3].depy=3;
        tabEnnemi[3].etat=1;
        tabEnnemi[3].image=bacterie3;

        tabEnnemi[4].posx=3050.;
        tabEnnemi[4].posy=200;
        tabEnnemi[4].depy=0;
        tabEnnemi[4].etat=1;
        tabEnnemi[4].image=bacterie1;

        tabEnnemi[5].posx=3113.;
        tabEnnemi[5].posy=249;
        tabEnnemi[5].depy=-2;
        tabEnnemi[5].etat=1;
        tabEnnemi[5].image=bacterie2;

        tabEnnemi[6].posx=4425.;
        tabEnnemi[6].posy=305;
        tabEnnemi[6].depy=1.5;
        tabEnnemi[6].etat=1;
        tabEnnemi[6].image=bacterie2;

        tabEnnemi[7].posx=5401.;
        tabEnnemi[7].posy=417;
        tabEnnemi[7].depy=2.3;
        tabEnnemi[7].etat=1;
        tabEnnemi[7].image=bacterie1;

        tabEnnemi[8].posx=6737.;
        tabEnnemi[8].posy=250;
        tabEnnemi[8].depy=1.75;
        tabEnnemi[8].etat=1;
        tabEnnemi[8].image=bacterie3;

        tabEnnemi[9].posx=6857.;
        tabEnnemi[9].posy=100;
        tabEnnemi[9].depy=2.9999999;
        tabEnnemi[9].etat=1;
        tabEnnemi[9].image=bacterie1;

        tabEnnemi[10].posx=6977.;
        tabEnnemi[10].posy=3;
        tabEnnemi[10].depy=3;
        tabEnnemi[10].etat=1;
        tabEnnemi[10].image=bacterie3;

        tabEnnemi[11].posx=8577.;
        tabEnnemi[11].posy=169;
        tabEnnemi[11].depy=0;
        tabEnnemi[11].etat=1;
        tabEnnemi[11].image=bacterie1;

        tabEnnemi[12].posx=8705.;
        tabEnnemi[12].posy=220;
        tabEnnemi[12].depy=1.25986;
        tabEnnemi[12].etat=1;
        tabEnnemi[12].image=bacterie2;

        tabEnnemi[13].posx=8873.;
        tabEnnemi[13].posy=100;
        tabEnnemi[13].depy=-5;
        tabEnnemi[13].etat=1;
        tabEnnemi[13].image=bacterie2;

        boss=AlloueBoss();
        boss->image=load_bitmap_check("Boss_1.bmp");

        break;
    case 2:
        fond = load_bitmap_check("fond_niveau2.bmp");
        boss=AlloueBoss();
        boss->image=load_bitmap_check("Boss_2.bmp");
        tabEnnemi[0].posx=990;
        tabEnnemi[0].posy=140;
        tabEnnemi[0].depy=1;
        tabEnnemi[0].etat=1;
        tabEnnemi[0].image=bacterie1;

        tabEnnemi[1].posx=1420;
        tabEnnemi[1].posy=250;
        tabEnnemi[1].depy=2;
        tabEnnemi[1].etat=1;
        tabEnnemi[1].image=bacterie2;

        tabEnnemi[2].posx=1990;
        tabEnnemi[2].posy=140;
        tabEnnemi[2].depy=1;
        tabEnnemi[2].etat=1;
        tabEnnemi[2].image=bacterie1;

        tabEnnemi[3].posx=2380;
        tabEnnemi[3].posy=270;
        tabEnnemi[3].depy=3;
        tabEnnemi[3].etat=1;
        tabEnnemi[3].image=bacterie3;

        tabEnnemi[4].posx=3280.;
        tabEnnemi[4].posy=150;
        tabEnnemi[4].depy=1;
        tabEnnemi[4].etat=1;
        tabEnnemi[4].image=bacterie1;

        tabEnnemi[5].posx=3920;
        tabEnnemi[5].posy=150;
        tabEnnemi[5].depy=2;
        tabEnnemi[5].etat=1;
        tabEnnemi[5].image=bacterie2;

        tabEnnemi[6].posx=4650;
        tabEnnemi[6].posy=140;
        tabEnnemi[6].depy=2;
        tabEnnemi[6].etat=1;
        tabEnnemi[6].image=bacterie2;

        tabEnnemi[7].posx=5600;
        tabEnnemi[7].posy=280;
        tabEnnemi[7].depy=1;
        tabEnnemi[7].etat=1;
        tabEnnemi[7].image=bacterie1;

        tabEnnemi[8].posx=6200;
        tabEnnemi[8].posy=90;
        tabEnnemi[8].depy=3;
        tabEnnemi[8].etat=1;
        tabEnnemi[8].image=bacterie3;

        tabEnnemi[9].posx=6200;
        tabEnnemi[9].posy=420;
        tabEnnemi[9].depy=1;
        tabEnnemi[9].etat=1;
        tabEnnemi[9].image=bacterie1;

        tabEnnemi[10].posx=7360;
        tabEnnemi[10].posy=350;
        tabEnnemi[10].depy=1;
        tabEnnemi[10].etat=1;
        tabEnnemi[10].image=bacterie1;

        tabEnnemi[11].posx=8460;
        tabEnnemi[11].posy=150;
        tabEnnemi[11].depy=3;
        tabEnnemi[11].etat=1;
        tabEnnemi[11].image=bacterie3;




        break;
    case 3:
        fond = load_bitmap_check("fond_niveau3.bmp");
        boss=AlloueBoss();
        boss->image=load_bitmap_check("Boss_3.bmp");
        tabEnnemi[0].posx=900;
        tabEnnemi[0].posy=220;
        tabEnnemi[0].depy=1;
        tabEnnemi[0].etat=1;
        tabEnnemi[0].image=bacterie1;

        tabEnnemi[1].posx=900;
        tabEnnemi[1].posy=380;
        tabEnnemi[1].depy=2;
        tabEnnemi[1].etat=1;
        tabEnnemi[1].image=bacterie2;

        tabEnnemi[2].posx=1860;
        tabEnnemi[2].posy=100;
        tabEnnemi[2].depy=1;
        tabEnnemi[2].etat=1;
        tabEnnemi[2].image=bacterie1;

        tabEnnemi[3].posx=2550;
        tabEnnemi[3].posy=80;
        tabEnnemi[3].depy=3;
        tabEnnemi[3].etat=1;
        tabEnnemi[3].image=bacterie3;

        tabEnnemi[4].posx=3080.;
        tabEnnemi[4].posy=390;
        tabEnnemi[4].depy=1;
        tabEnnemi[4].etat=1;
        tabEnnemi[4].image=bacterie1;

        tabEnnemi[5].posx=3870;
        tabEnnemi[5].posy=230;
        tabEnnemi[5].depy=2;
        tabEnnemi[5].etat=1;
        tabEnnemi[5].image=bacterie2;

        tabEnnemi[6].posx=4650;
        tabEnnemi[6].posy=140;
        tabEnnemi[6].depy=2;
        tabEnnemi[6].etat=1;
        tabEnnemi[6].image=bacterie2;

        tabEnnemi[7].posx=4700;
        tabEnnemi[7].posy=320;
        tabEnnemi[7].depy=1;
        tabEnnemi[7].etat=1;
        tabEnnemi[7].image=bacterie1;

        tabEnnemi[8].posx=4910;
        tabEnnemi[8].posy=170;
        tabEnnemi[8].depy=3;
        tabEnnemi[8].etat=1;
        tabEnnemi[8].image=bacterie3;

        tabEnnemi[9].posx=5180;
        tabEnnemi[9].posy=320;
        tabEnnemi[9].depy=1;
        tabEnnemi[9].etat=1;
        tabEnnemi[9].image=bacterie1;

        tabEnnemi[10].posx=6600;
        tabEnnemi[10].posy=390;
        tabEnnemi[10].depy=1;
        tabEnnemi[10].etat=1;
        tabEnnemi[10].image=bacterie1;

        tabEnnemi[11].posx=6710;
        tabEnnemi[11].posy=220;
        tabEnnemi[11].depy=3;
        tabEnnemi[11].etat=1;
        tabEnnemi[11].image=bacterie3;

        tabEnnemi[12].posx=7490;
        tabEnnemi[12].posy=320;
        tabEnnemi[12].depy=1;
        tabEnnemi[12].etat=1;
        tabEnnemi[12].image=bacterie1;

        tabEnnemi[13].posx=7550;
        tabEnnemi[13].posy=130;
        tabEnnemi[13].depy=3;
        tabEnnemi[13].etat=1;
        tabEnnemi[13].image=bacterie3;

        tabEnnemi[14].posx=8900;
        tabEnnemi[14].posy=100;
        tabEnnemi[14].depy=1;
        tabEnnemi[14].etat=1;
        tabEnnemi[14].image=bacterie1;

        tabEnnemi[15].posx=8900;
        tabEnnemi[15].posy=300;
        tabEnnemi[15].depy=3;
        tabEnnemi[15].etat=1;
        tabEnnemi[15].image=bacterie3;

        tabEnnemi[16].posx=8900;
        tabEnnemi[16].posy=500;
        tabEnnemi[16].depy=3;
        tabEnnemi[16].etat=1;
        tabEnnemi[16].image=bacterie3;

        break;
    }
    mur=AlloueMur(niveau);

    //chargement du perso

    perso=AllouePerso();


    while(!perso->fin && !mur->fin &&!key[KEY_ESC])
    {
        DeplacementPerso(perso);
        DeplacementEnnemis(tabEnnemi,mur);
        ScrollingMur(mur,perso);
        tirPerso=TirPerso(tirPerso,perso,munitionPerso);
        tirPerso=DeplacementMunition(tirPerso);
        tirEnnemi=TirEnnemi(tirEnnemi,tabEnnemi,munitionEnnemie,mur);
        tirEnnemi=DeplacementMunition(tirEnnemi);
        Collision(perso,mur,tirPerso,tirEnnemi,tabEnnemi);
        Affichage(fond, mur, perso, tabEnnemi,NULL,tirPerso,tirEnnemi);

        rest(REST);

    }

    if (mur->fin)
    {


        while(!perso->fin&& !boss->fin&& !key[KEY_ESC])
        {
            DeplacementPerso(perso);
            Affichage(fond, mur, perso,tabEnnemi, boss,tirPerso,tirEnnemi);
            DeplacementBoss(boss);
            if(key[KEY_SPACE])
            {
                tirPerso=TirPerso(tirPerso,perso,munitionPerso);

            }
            tirPerso=DeplacementMunition(tirPerso);

            //tirPerso=SuppressionMunition(tirPerso);
            rest(REST);

        }

    }

    if(perso->fin)
    {
        return 0; //perdu
    }
    if (boss->fin)
    {
        return 1; //gagné
    }


    return 2;//abandon
}

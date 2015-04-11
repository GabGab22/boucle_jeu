#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <math.h>

#define SCROLL 1

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
    perso->depx=4;
    perso->depy=3;
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


void Affichage(BITMAP*fond,t_Mur*mur, t_Perso *perso,t_Ennemi tabEnnemi[25],t_Boss*boss,t_tir*premier)
{

    BITMAP *page;
    int i;
    page=create_bitmap(SCREEN_W,SCREEN_H);
    clear_bitmap(page);

    blit(fond,page,0,0,0,0,SCREEN_W,SCREEN_H);
    masked_blit(mur->image,page,mur->posx,0,0,0,SCREEN_W,SCREEN_H);
    for(i=0; i<25; i++)
    {
        if(tabEnnemi[i].etat)
        {
            masked_blit(tabEnnemi[i].image,page,0,0,tabEnnemi[i].posx-mur->posx,tabEnnemi[i].posy,tabEnnemi[i].image->w,tabEnnemi[i].image->h);
        }

    }
    if(boss!=NULL)
    {
        masked_blit(boss->image,page,0,0,boss->posx,boss->posy,boss->image->w,boss->image->h);
    }
        t_tir* courant;
    courant=premier;

    while(courant!=NULL)
    {
        if(courant->etat==1)
        {
        courant->image=load_bitmap("munition2.bmp",NULL);
        masked_blit(courant->image,page,0,0,courant->posmx,courant->posmy,courant->image->w,courant->image->h);

        }

        courant=courant->suiv;
    }

    masked_blit(perso->image,page,0,0,perso->posx,perso->posy,perso->image->w,perso->image->h);


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
    else
    {
        perso->posy=perso->posy+10;
        if (perso->posy>SCREEN_H)
        {
            perso->fin=1;
        }
    }
}

void ScrollingMur(t_Mur*mur,t_Perso*perso)
{
    if(perso->etat==VIVANT)
    {
        mur->posx=mur->posx+mur->depx;

    if(mur->posx>mur->image->w-SCREEN_W)
    {
        mur->fin=1;
    }
    }


}

void Collision(t_Perso*perso,t_Mur*mur)
{
    //il y a collision personnage/murs si le pixel de mur au centre du personnage est noir
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
t_tir*AlloueMunition(t_tir*premier,t_Perso*perso)
{
    t_tir*nouv;

    nouv=(t_tir*)malloc(1*sizeof(t_tir));

    nouv->posmx=perso->posx+perso->image->w-10;
    nouv->posmy=perso->posy+perso->image->h-40;
    nouv->depmx=5;
    nouv->etat=1;
    nouv->suiv=premier;

    return nouv;

}
///////////////////////////////////////////////////
void DeplacementMunition(t_tir*premier)
{
    t_tir*courant;
    courant=premier;

    while(courant!=NULL)
    {
        courant->posmx=courant->posmx+courant->depmx;
        if((courant->posmx>SCREEN_W) || (courant->posmx<0))
        {
            courant->etat=0;

        }
        courant=courant->suiv;
    }

}
////////////////////////////////////////////////////////////////
t_tir* SuppressionMunition(t_tir*premier)
{
    t_tir*courant;
    courant=premier;
    t_tir*prec=NULL;

    while(courant!=NULL)
    {
      if(courant->etat==0)
      {
          if(courant==premier)
          {
              premier=premier->suiv;
              free(courant);

              break;

          }
          prec->suiv=courant->suiv;
          free(courant);
          break;
      }
      prec=courant;
      courant=courant->suiv;
    }
    return premier;
}

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

t_tir* AlloueMunitionEnnemie(t_tir*ancre)
{
  t_tir*nouv;

  nouv=(t_tir*)malloc(1*sizeof(t_tir));
  nouv->etat=1;
  nouv->depmx=-5;
  nouv->suiv=ancre;

  return nouv;
}

t_tir* TirEnnemi(t_Ennemi tabEnnemi[25])
{
    int i;
    t_tir*ancre=NULL;

    if(rand()%100<=50)
    {
        for(i=0;i<25;i++)
        {
            if(tabEnnemi[i].etat)
            {
               ancre=AlloueMunitionEnnemie(ancre);
               ancre->posmx=tabEnnemi[i].posx;
               ancre->posmy=tabEnnemi[i].posy+tabEnnemi[i].image->h;
               DeplacementMunition(ancre);
               ancre=SuppressionMunition(ancre);
            }
        }
    }
    return ancre;
}

void AffichageMunition(t_tir*ancre,t_Mur*mur,t_Ennemi tabEnnemi[25])
{
    t_tir*courant;
    courant=ancre;
    int i;

    while(courant!=NULL)
    {
        for(i=0;i<25;i++)
        {
           if(mur->posx+mur->image->w>tabEnnemi[i].posx)
            {
        if(courant->etat==1)
        {
            courant->image=load_bitmap_check("laser.bmp");
            masked_blit(courant->image,screen,0,0,courant->posmx,courant->posmy,courant->image->w,courant->image->h);
        }
        courant=courant->suiv;
        }

    }
    }
}

int jouer(int niveau )
{
    BITMAP *fond;
    t_Perso*perso;
    t_Mur*mur;
    BITMAP*bacterie1,*bacterie2,*bacterie3;
    t_Boss*boss;
    int i;
    int alea;
    t_tir*premier=NULL;


    t_Ennemi tabEnnemi[25];
    t_tir*missile=NULL;
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

        break;
    case 3:
        fond = load_bitmap_check("fond_niveau3.bmp");
        boss=AlloueBoss();
        boss->image=load_bitmap_check("Boss_3.bmp");
        break;
    }
    mur=AlloueMur(niveau);

    //chargement du perso

    perso=AllouePerso();


    while(!perso->fin && !mur->fin &&!key[KEY_ESC])
    {
        alea=rand()%10;
        DeplacementPerso(perso);
        DeplacementEnnemis(tabEnnemi,mur);
        ScrollingMur(mur,perso);
        Collision(perso, mur);
        Affichage(fond, mur, perso, tabEnnemi,NULL,premier);
        if(key[KEY_SPACE])
            {
               premier=AlloueMunition(premier,perso);

            }
                DeplacementMunition(premier);

               premier=SuppressionMunition(premier);
               missile=TirEnnemi(tabEnnemi);
               AffichageMunition(missile,mur,tabEnnemi);
               rest(20);

    }

    if (mur->fin)
    {


        while(!perso->fin&& !boss->fin&& !key[KEY_ESC])
        {
            DeplacementPerso(perso);
            Affichage(fond, mur, perso,tabEnnemi, boss,premier);
            DeplacementBoss(boss);
            if(key[KEY_SPACE])
            {
               premier=AlloueMunition(premier,perso);

            }
            DeplacementMunition(premier);

               premier=SuppressionMunition(premier);
               rest(20);

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

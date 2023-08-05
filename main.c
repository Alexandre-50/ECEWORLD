#include "header.h"

#include "jeuRiviere/header.h"
#include "snakeGame/header.h"
#include "jackpotJeu/header.h"
#include "jeuBallons/header.h"
#include "taupeLa/header.h"
#include "jeuLabyrinthe/header.h"
#include "guitareHero/header.h"
#include "jeuCanard/header.h"
#include "displayStats/header.h"
#include "parisHippiques/header.h"


int main(void)
{
    // Initialisation des différents éléments essentiels au menu
    initialisation(0);
    FONT** T80M = initPolices();
    t_Perso* perso = initPersonnages(32, "../images/personnages.bmp");
    int nbJoueurs = 2, modeJeu = 0;
    t_Joueur* joueurs = initJoueurs(&nbJoueurs, &modeJeu, T80M, perso);

    //Zone de test pour les jeux
    printf("GLOBAL - START\n");

    //snakeGame(T80M);
    //jeuRiviere();
    //jackpotJeu();
    //jeuBallons(T80M);
    //taupeLa();
    //guitareHero(T80M);
    //jeuLabyrinthe(T80M);
    //jeuCanard(T80M);
    //parisHippiques();

    printf("GLOBAL - STOP\n");


    // Déclaration des variables de premier ordre
    bool CONDEXIT = 0;                  // Condition de sortie
    clock_t tStop = clock() + DIFTIME;  // Variable d'incrémentation du pas de temps (DIFTIME)
    int playing = 0;                    // Variable contenant le joueur qui joue
    int gagnant = -1;                   // Variable du joueur gagnant
    BITMAP* map = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    map = load_bitmap("../images/map.bmp", NULL);

    // Disposition du personnage de chaque joueur à une position par défaut pour le premier affichage
    for(int i = 0; i < nbJoueurs; i++)
    {
        joueurs[i].dirx = 0; joueurs[i].diry = 1;
        joueurs[i].x = SCREEN_WIDTH/2 - joueurs[i].perso.dim/2;
        joueurs[i].y = SCREEN_HEIGHT - 32*(i+1);
        joueurs[i].var = 0; joueurs[i].nimg = 0;
    }

    // Boucle globale de jeu
    while(!CONDEXIT)
    {
        // Vérification de la position du joueur par rapport aux zones de jeu
        int dpGame = retGame(joueurs[playing], 16);

        if(keypressed())
        {
            // Affectation de la derniere valeur du keybuf et conversion en constante de touche
            int val = readkey() >> 8;

            // Si le mode de jeu est en Arcade permettre le changement de joueur sur appui d'une touche
            if( modeJeu == 1 && val == KEY_EQUALS )
            {
                joueurs[playing].nimg = 0;
                if(val == KEY_EQUALS)
                {
                    playing = (playing+1)%nbJoueurs;
                }
                clear_keybuf();
            }

            // Si une des touche de déplacement est pressée
            if( val == KEY_UP || val == KEY_LEFT || val == KEY_DOWN || val == KEY_RIGHT )
            {

                // Si le pas de temps est passé changer le sprite du personnage et relancer le décompte
                if(clock() >= tStop)
                {
                    tStop = clock() + DIFTIME; joueurs[playing].nimg++;
                };
                // Remise à 0 de la position du joueur
                joueurs[playing].dirx = 0; joueurs[playing].diry = 0;

                // En fonction de la touche pressée affecter la direction correspondante
                if(val == KEY_UP)
                {
                    joueurs[playing].diry = -1;
                }
                if(val == KEY_LEFT)
                {
                    joueurs[playing].dirx = -1;
                }
                if(val == KEY_DOWN)
                {
                    joueurs[playing].diry = 1;
                }
                if(val == KEY_RIGHT)
                {
                    joueurs[playing].dirx = 1;
                }

                // Vérification de la position du joueur par rapport aux zones de déplacement
                if(proMov(joueurs[playing], 16))
                {
                    // Déplacement
                    joueurs[playing].x += joueurs[playing].dirx * MOVING;
                    joueurs[playing].y += joueurs[playing].diry * MOVING;

                }
            }

            // Si la touche d'action est pressée
            if( val == KEY_ENTER)
            {

                // Liaison des actions aux zones de jeu correspondant à un jeu ou à una action spéciale (Sortie ou Accueil)
                if(dpGame >= 2)
                {
                    // Si c'est un jeu lacement de la routine de lancement de jeu
                    playGame(joueurs, nbJoueurs, modeJeu, dpGame, playing, T80M);
                    // Incrémentation du joueur quu joue en fonction du mode de jeu
                    if(modeJeu != 1) playing = (playing+1)%nbJoueurs;
                }
                else if (dpGame == 0)
                {
                    // Si l'accueil est sélectionné afficher les statistiques
                    displayStats(joueurs, nbJoueurs, T80M);
                }
                else if (dpGame == 1)
                {
                    // Si la sortie est sélectionnée sortir
                    CONDEXIT = 1;
                }

                // Clear du keybuf pour éviter les répétitions
                clear_keybuf();
            }

        }
        else
        {
            // Si aucune touche n'est pressée remettre le sprite du joueur à 0
            joueurs[playing].nimg = 0;
        }
        rest(20);

        // En fonction du déplacement afficher la variation de position correspondante
        if(joueurs[playing].diry == -1)
        {
            joueurs[playing].var = 2;
        }
        else if(joueurs[playing].dirx == 1)
        {
            joueurs[playing].var = 1;
        }
        else if(joueurs[playing].dirx == -1)
        {
            joueurs[playing].var = -1;
        }
        else
        {
            joueurs[playing].var = 0;
        }
        // Affichage de la map du menu
        displayMapMenu(nbJoueurs, T80M, joueurs, map, dpGame, playing);

        // Si le mode de jeu est en classique trouver un gaganant (qui a des tickets alors que les autres n'en ont plus)
        if(modeJeu != 1)
        {
            gagnant = -1;
            CONDEXIT = 1;
            for(int i = 0; i < nbJoueurs; i++)
            {
                if(joueurs[i].tickets > 0)
                {
                    if(gagnant > -1)
                    {
                        CONDEXIT = 0;
                    }   gagnant = i;
                }
            }
            //printf("WIN : %d\n", gagnant);
        }
    }

    if(modeJeu != 1 && gagnant != -1)
    {
        BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
        clear_to_color(buffer, DP_DEFBG);
        // Afficher a l'écran
        textout_centre_ex(buffer, T80M[5], "GAGNANT", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
        textout_centre_ex(buffer, T80M[4], joueurs[gagnant].nom, SCREEN_W/2, 2 * SCREEN_H/6, DP_DEFTXT, -1);
        masked_stretch_blit(joueurs[gagnant].perso.bmp[0][0], buffer, 0, 0, joueurs[gagnant].perso.dim, joueurs[gagnant].perso.dim,
                            SCREEN_W/2 - joueurs[gagnant].perso.dim*10/2, 3*SCREEN_H/5 - joueurs[gagnant].perso.dim*10/2, joueurs[gagnant].perso.dim*10, joueurs[gagnant].perso.dim*10);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        destroy_bitmap(buffer);
        rest(7500);
    }

    // Fermeture et libération mémoire
    rest(1000);
    destroy_bitmap(map);
    fermeture();
    return 0;
}END_OF_MAIN();



void initialisation(int intro)
{
    //      -- FONCTION D'INITIALISATION GLOBALE
    //      INTRO     : Selection affichage cinématique (oui/non)
    //      || RETURN :     N/A

    // Initialisations diverses
    srand(time(NULL));
    allegro_init(); install_keyboard(); install_mouse(); install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(desktop_color_depth());
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_WIDTH,SCREEN_HEIGHT,0,0))!=0)
    {
        allegro_message("Pbdemodegraphique");
        //allegro_exit();
        //exit(EXIT_FAILURE);
    }

    //Possibilité d'afficher la cinématique de démarrage
    if(intro)
    {
        BITMAP *ECEWorld = create_bitmap(132, 65);
        ECEWorld = load_bitmap("../images/ECE World BMP.bmp", NULL);
        rest(500);
        for (int i = 0; i < ECEWorld->w; i++)
        {
            clear_to_color(screen, DP_DEFBG);
            masked_blit(ECEWorld, screen, 0, 0, SCREEN_WIDTH / 2 - ECEWorld->w / 2, SCREEN_HEIGHT / 2 - ECEWorld->h / 2, i,
                 ECEWorld->h);
            rest(3);
        }
        rest(2225);
        clear_to_color(screen, DP_DEFBG);
        rest(500);
        destroy_bitmap(ECEWorld);
    }

}
void fermeture()
{
    //      -- FONCTION D'E FERMETURE GLOBALE
    //      || RETURN :     N/A

    allegro_exit();
}

t_Perso* initPersonnages(int dim, char path[])
{
    //      -- FONCTION D'INITIALISATION DES SPRITES DES PERSONNAGES
    //      DIM       : Dimension unique WH d'un sprite
    //      PATH      : Chemin vers le fichier grille de personnages
    //      || RETURN :     Tableau contenant tous les personnages

    BITMAP* persos = create_bitmap((NB_BMPMOV+1) * dim, NB_BMPCOLORS * NB_BMPPOS * dim);
    persos = load_bitmap(path, NULL);
    t_Perso* perso = (t_Perso*) malloc(NB_BMPCOLORS * sizeof(t_Perso));

    // Parcours de l'image (12 couleurs * 4 lignes par couleur et 4 variantes + 1 palette)
    for(int i = 0; i < NB_BMPCOLORS; i++)
    {
        perso[i].dim = dim;
        for(int j = 0; j < NB_BMPPOS; j++)
        {
            for(int k = 0; k < NB_BMPMOV; k++)
            {
                // Création du bitmap et copie de l'ilage à l'intérieur
                perso[i].bmp[j][k] = create_bitmap(dim, dim);
                blit(persos, perso[i].bmp[j][k], k*dim, i*NB_BMPPOS*dim + j*dim, 0, 0, dim, dim);
;           }
            perso[i].palette[j] = getpixel(persos, NB_BMPMOV*dim,i*NB_BMPPOS*dim + j*dim);
        }
    }
    destroy_bitmap(persos);
    return perso;
}
FONT** initPolices()
{
    //      -- FONCTION D'INITIALISATION DE LA POLICE
    //      || RETURN :     Tableau contenant toutes les tailles de la police

    FONT **T80M = (FONT**) malloc(6 * sizeof(FONT*));
    T80M[0] = load_font("../polices/T80M-8.pcx", NULL, NULL);
    T80M[1] = load_font("../polices/T80M-12.pcx", NULL, NULL);
    T80M[2] = load_font("../polices/T80M-20.pcx", NULL, NULL);
    T80M[3] = load_font("../polices/T80M-36.pcx", NULL, NULL);
    T80M[4] = load_font("../polices/T80M-48.pcx", NULL, NULL);
    T80M[5] = load_font("../polices/T80M-72.pcx", NULL, NULL);
    if (!T80M[0])
        printf("ERREUR T80M");
    return T80M;
}

t_Joueur* initJoueurs(int* nbJoueurs, int* modeJeu, FONT* fonts[], t_Perso* perso)
{
    //      -- FONCTION D'INITIALISATION ET DE PERSONALISATION DES JOUEURS
    //      NBJOUEURS : Nombre de joueurs total
    //      MODEJEU   : Mode de jeu selectionné
    //      FONTS     : Police d'écriture pour affichage a l'écran
    //      PERSO     : Tableau de tous les personnages disponibles
    //      || RETURN :     Tableau contenant tous les joueurs

    // Création des variables de premier ordre
    bool COND = 0;                                          // Condition de sortie
    char tempStr[STR_MAX] = "";                             // String temporaire (réutilisé entre les boucles
    char modes[2][10] = {"Classique","Arcade"};     // Correspondance des ID aux modes de jeu
    int tabAQ[5][2] = {     {17,    1},         // Correspondance des touches QWERTY vers l'AZERTY
                            {105,   13},
                            {1,     17},
                            {26,    23},
                            {23,    26}
    };
    BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

    //////    SELECTION DU MODE DE JEU    //////
    // Boucle de selection
    while(!key[KEY_ENTER])
    {
        clear_to_color(buffer, DP_DEFBG);
        // Faire varier le mode jeu
        if      (key[KEY_UP])
        {
            if(*modeJeu < 1)
            {
                (*modeJeu)++;rest(100);
            }
        }
        else if(key[KEY_DOWN])
        {
            if(*modeJeu > 0)
            {
                (*modeJeu)--;rest(100);
            }
        }
        // Afficher a l'écran
        textout_centre_ex(buffer, fonts[5], "CHOIX DU MODE DE JEU", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
        textout_centre_ex(buffer, fonts[2], "Utilisez les fleches haut et bas pour selectionner, puis entree pour valider", SCREEN_W/2, SCREEN_W/10 + 75, DP_DEFTXT, -1);
        textout_centre_ex(buffer, fonts[4], modes[*modeJeu], SCREEN_W/2, SCREEN_H/2, DP_DEFTXT, -1);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    // Délai et remise à 0 du keybuf
    rest(175);
    clear_keybuf();


    ////// SELECTION DU NOMBRE DE JOUEURS //////
    // Boucle de selection
    while(!key[KEY_ENTER])
    {
        clear_to_color(buffer, DP_DEFBG);
        // Faire varier le nombre de joueurs (verification du non-dépassement des bornes)
        if(key[KEY_UP])
        {
            if(*nbJoueurs < NB_BMPCOLORS)
            {
                (*nbJoueurs)++;
                rest(100);
            }
        }
        else if(key[KEY_DOWN])
        {
            if(*nbJoueurs > 2)
            {
                (*nbJoueurs)--;
                rest(100);
            }
        }
        // Afficher a l'écran
        textout_centre_ex(buffer, fonts[5], "CHOIX DU NOMBRE DE JOUEURS", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
        textout_centre_ex(buffer, fonts[2], "Utilisez les fleches haut et bas pour selectionner, puis entree pour valider", SCREEN_W/2, SCREEN_W/10 + 75, DP_DEFTXT, -1);
        sprintf(tempStr, "%d", *nbJoueurs);
        textout_centre_ex(buffer, fonts[4], tempStr, SCREEN_W/2, SCREEN_H/2, DP_DEFTXT, -1);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    // Délai et remise à 0 du keybuf
    rest(175);
    clear_keybuf();

    // Allocation du tableau de joueurs
    t_Joueur* joueurs = (t_Joueur*) malloc(*nbJoueurs * sizeof(t_Joueur));
    int used[NB_BMPCOLORS] = {-1};


    //////      CREATION DES JOUEURS      //////
    // Repetition pour le nombre de joueurs
    for(int i = 0; i < *nbJoueurs; i++)
    {

        // Création des variables de premier ordre
        time_t tStop = time(NULL) + 5;      //Mise en place du pas de temps (variation aléatoire du personnage)
        int selection = 0, sprite = 0;      // Sélection de perso pour le joueur et sprite sélectionné
        strcpy(joueurs[i].nom, "");         // Clear du nom

        // Boucle de sélection (si un pseudo est entré)
        while(!(key[KEY_ENTER] && strlen(joueurs[i].nom) > 0))
        {
            // Si le pas de temsp est passé changer le sprite du personnage aléatoirement et relancer le décompte
            if(time(NULL) >= tStop)
            {
                tStop = time(NULL) + 3 + rand()%6; sprite = rand()%4;
            };
            clear_to_color(buffer, DP_DEFBG);
            if(keypressed())
            {
                // Affectation de derniere valeur de la keybuf et conversion en constante de touche
                int val = readkey() >> 8;

                // En fonction de la touche pressée changer de personnages ou retirer le dernier caractere du nom
                if(key[KEY_UP])
                {
                    if(selection < NB_BMPCOLORS-1)
                    {
                        (selection)++;
                        rest(100);
                    }
                }
                else if(key[KEY_DOWN])
                {
                    if(selection > 0)
                    {
                        (selection)--;
                        rest(100);
                    }
                }
                else if(key[KEY_ENTER])
                {

                }
                else if (key[KEY_BACKSPACE])
                {
                    if(strlen(joueurs[i].nom) > 0)
                    {
                        joueurs[i].nom[strlen(joueurs[i].nom)-1] = '\0';
                    }
                }

                // Parcours du tableau de correspondance QWERTY->AZERTY pour remplacer les caractères affectés
                for(int j = 0; j < 5; j++)
                {
                    if(val == tabAQ[j][0])
                    {
                        val = tabAQ[j][1];break;
                    }
                }
                // Si la touche est une lettre et que le nom est non-rempli
                if(1 <= val && val <= 26 && STR_MAX > strlen(joueurs[i].nom))
                {
                    // Si il faut écrire en capitales
                    if(key_shifts & KB_SHIFT_FLAG)
                    {
                        sprintf(tempStr, "%c", 64 + val);
                    }
                    else
                    {
                        sprintf(tempStr, "%c", 96 + val);
                    }
                    strcat(joueurs[i].nom, tempStr);
                }
            }
            // Afficher a l'écran
            sprintf(tempStr, "CREATION DU JOUEUR %d", i+1);
            textout_centre_ex(buffer, fonts[5], tempStr, SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
            textout_centre_ex(buffer, fonts[2], "Utilisez les fleches haut et bas pour selectionner, les lettres pour ecrire, puis entree pour valider", SCREEN_W/2, SCREEN_W/10 + 75, DP_DEFTXT, -1);
            textout_centre_ex(buffer, fonts[4], joueurs[i].nom, SCREEN_W/2, 2 * SCREEN_H/5, DP_DEFTXT, -1);
            masked_stretch_blit(perso[selection].bmp[sprite][0], buffer, 0, 0, perso[selection].dim, perso[selection].dim, SCREEN_W/2 - perso[selection].dim*10/2, 2*SCREEN_H/3 - perso[selection].dim*10/2, perso[selection].dim*10, perso[selection].dim*10);
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        // Affectation du personnage choisi au joueur
        joueurs[i].perso = perso[selection];

        // En fonction du mode de jeu affecter les tickets ou non
        if(*modeJeu != 1) joueurs[i].tickets = 5;

        // Initialisation du tableau des scores inhérent a chaque joueur
        for (int j = 0; j < NBGAMES + 2; j++)
        {
            joueurs[i].score[j] = -1;
        }
        // Délai et remise à 0 du keybuf
        rest(175);
        clear_keybuf();
    }
    destroy_bitmap(buffer);
    return joueurs;
}

void displayMapMenu(int nbJoueurs, FONT** fonts, t_Joueur* joueurs, BITMAP* map, int dpGame, int playing)
{
    //      -- FONCTION D'INITIALISATION ET DE PERSONALISATION DES JOUEURS
    //      NBJOUEURS : Nombre de joueurs total
    //      FONTS     : Police d'écriture pour affichage a l'écran
    //      JOUEURS   : Tableau contenant tous les joueurs
    //      MAP       : BMP sur lequel écrire (carte menu)
    //      DPGAME    : Jeu correspondant a la zone occupée par le joueur
    //      PLAYING   : Joueur qui joue
    //      || RETURN :     N/A

    // Mise en place de la base (carte)
    BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    blit(map, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Affichage des ensembles joueurs
    for(int i = 0; i < nbJoueurs; i++)
    {
        // Affichage du joueur
        if(joueurs[i].var >= 0)
        {
            masked_blit(joueurs[i].perso.bmp[joueurs[i].var][joueurs[i].nimg % 4], buffer, 0, 0, joueurs[i].x,
                        joueurs[i].y, joueurs[i].perso.dim, joueurs[i].perso.dim);
        }
        else
        {
            draw_sprite_h_flip(buffer, joueurs[i].perso.bmp[-1 * joueurs[i].var][joueurs[i].nimg % 4], joueurs[i].x, joueurs[i].y);
        }

        // Affichage du pseudo et du cercle de sélection
        int textHeight = 1;
        if(playing == i)
        {
            circlefill(buffer, joueurs[i].x + joueurs[i].perso.dim/2 - text_length(fonts[textHeight], joueurs[i].nom)/2 - (text_height(fonts[textHeight])/2 - 2), joueurs[i].y - 10 + text_height(fonts[textHeight])/2, text_height(fonts[textHeight])/2 - 2, joueurs[i].perso.palette[2]);
            circle(buffer, joueurs[i].x + joueurs[i].perso.dim/2 - text_length(fonts[textHeight], joueurs[i].nom)/2 - (text_height(fonts[textHeight])/2 - 2), joueurs[i].y - 10 + text_height(fonts[textHeight])/2, text_height(fonts[textHeight])/2 - 1, joueurs[i].perso.palette[3]);
        }
        textout_centre_ex(buffer, fonts[textHeight], joueurs[i].nom, joueurs[i].x + joueurs[i].perso.dim/2 + (playing == i)*((text_height(fonts[textHeight])/2 - 2)), joueurs[i].y - 10, joueurs[i].perso.palette[2], joueurs[i].perso.palette[3]);
    }

    // Stockage des noms de jeu en fonction de leur ID
    char nameGame[NBGAMES + 2][STR_MAX] =
    {
            "Statistiques","Sortir","Paris Hippiques","Tir aux Ballons","Snake","Jeu de la Riviere",
            "Palais des Glaces","Jackpot","Guitar Hero","Taupe La","La Peche aux Canards"
    };

    // Affichage d'un jeu en encart en bas a gauche si une zone est traversée
    if(dpGame != -1)
    {
        int fonth = 2;
        rectfill(buffer, 0, SCREEN_HEIGHT - text_height(fonts[fonth]) - 3*2 - 2, text_length(fonts[fonth], nameGame[dpGame]) + 3*2 + 2, SCREEN_HEIGHT, DP_DEFTXT);
        rectfill(buffer, 0, SCREEN_HEIGHT - text_height(fonts[fonth]) - 3*2, text_length(fonts[fonth], nameGame[dpGame]) + 3*2, SCREEN_HEIGHT, DP_DEFBG);
        textout_ex(buffer, fonts[fonth], nameGame[dpGame], 3*1, SCREEN_HEIGHT - text_height(fonts[fonth]) - 3*1, DP_DEFTXT, -1);
    }

    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    destroy_bitmap(buffer);
}
int proMov(t_Joueur joueurs, int dimMap)
{
    //      -- FONCTION AUTORISANT LES JOUEURS A SE DEPLACER
    //      JOUEURS   : Tableau contenant tous les joueurs
    //      DIMMAP    : Taille d'un carré sur l'écran
    //      || RETURN :     1 : Autorisation de Mouvement   0 : Pas d'Autorisation de Mouvement

    // Listing des chemins
    #define NBCHEMINS 11
    int chemins[NBCHEMINS][4]=
    {
            {38,    6,  41, 47},
            {19,    37, 41, 41},
            {26,    33, 29, 41},
            {38,    34, 42, 38},
            {38,    25, 67, 29},
            {53,    24, 56, 29},
            {5,     16, 41, 20},
            {8,     16, 10, 20},
            {21,    16, 24, 20},
            {11,    16, 14, 22},
            {38,    7,  53, 11}
    };

    // Parcours des chemins en fonction du mouvement prévu
    for(int i = 0; i < NBCHEMINS; i++)
    {
        if( joueurs.x + joueurs.dirx * MOVING >= chemins[i][0]*dimMap &&
            joueurs.x + dimMap + joueurs.dirx * MOVING <= chemins[i][2]*dimMap &&
            joueurs.y + joueurs.diry * MOVING >= chemins[i][1]*dimMap &&
            joueurs.y + dimMap + joueurs.diry * MOVING <= chemins[i][3]*dimMap
        )
        {
            return 1;
        }
    }
    return 0;
}
int retGame(t_Joueur joueurs, int dimMap)
{
    //      -- FONCTION RETOURNANT LA JEU DE LA ZONE OCCUPEE
    //      JOUEURS   : Tableau contenant tous les joueurs
    //      DIMMAP    : Taille d'un carré sur l'écran
    //      || RETURN :     -1 : Aucune zone occupée   Autre : Zone Occupée

    // Listing des zones de jeu
    int zones[NBGAMES + 2][4]=
    {
            {26,    33, 29, 35},
            {38,    45, 41, 47},
            {19,    37, 20, 41},
            {40,    34, 41, 38},
            {53,    24, 56, 26},
            {66,    25, 67, 29},
            {8,     16, 10, 18},
            {21,    16, 24, 18},
            {11,    20, 14, 22},
            {38,    6,  41, 8},
            {50,    7,  53, 11}
    };

    // Parcours des jeux en fonction de la position du joueur
    for(int i = 0; i < NBGAMES + 2; i++)
    {
        if( joueurs.x >= zones[i][0]*dimMap &&
            joueurs.x <= zones[i][2]*dimMap &&
            joueurs.y >= zones[i][1]*dimMap &&
            joueurs.y <= zones[i][3]*dimMap
          )
        {
            return i;
        }
    }
    return -1;
}

void playGame(t_Joueur* joueurs, int nbJoueurs, int modeJeu, int dpGame, int playing, FONT** fonts)
{
    //      -- FONCTION PERMETTANT DE LANCER UNE PARTIE D'UN JEU ET GERER SON DEROULE
    //      JOUEURS   : Tableau contenant tous les joueurs
    //      NBJOUEURS : Nombre de joueurs total
    //      MODEJEU   : Mode de jeu selectionné
    //      DPGAME    : Jeu correspondant a la zone occupée par le joueur
    //      PLAYING   : Joueur qui joue
    //      FONTS     : Police d'écriture pour affichage a l'écran
    //      || RETURN :     N/A

    // Délai et remise à 0 du keybuf
    rest(175);
    clear_keybuf();

    // Si l'ID de jeu envoye est bien un jeu et non une action spéciale
    if(dpGame > 1)
    {
        // Initialisation et affectation d'un tableau contenant le joueur requérant et le second joueur entré par la fonction correspondante
        int players[2]; players[0] = playing;
        players[1] = chooseSecondPlayer(joueurs, nbJoueurs, playing, fonts);

        // Tableau contenant toutes les bornes faisant gagner ou perdre des tickets additionnels
        int bornesScore[NBGAMES + 2][2]=
        {
                {0, 0},
                {0, 0},
                {0, 0},
                {250, 2500},
                {250, 1500},
                {0, 1800},
                {-1, 6000},
                {-1, 1},
                {20, 70},
                {20, 40},
                {300, 2000}
        };

        BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

        // Si c'est un jeu caractéristique (qui ne suit pas le schéma de jeu ou les joueurs jouent l'un après l'autre
        if(dpGame <= 2 || dpGame >= 2 + NBGAMES)
        {
            // Si le jeu est le Paris Hippique
            // Ce jeu marche différemment des autres jeux et est donc mis a part, il dispose d'une routine différente
            if(dpGame == 2)
            {
                // Affectation des tickets
                if(modeJeu != 1) {joueurs[players[0]].tickets--;joueurs[players[1]].tickets--;}
                // Lancement du jeu
                int* scores = parisHippiques(joueurs[players[0]].nom, joueurs[players[1]].nom, fonts);
                //printf("JOUEUR 1 (%s) : %d\t\tJOUEUR 2 (%s) : %d", joueurs[players[0]].nom, scores[0], joueurs[players[1]].nom, scores[1], fonts);
                rest(2000);
                // Affectation du score
                joueurs[players[0]].score[dpGame] = scores[0]; joueurs[players[1]].score[dpGame] = scores[1];
                free(scores);
            }
        }
        // Si le jeu est un jeu suivant un schéma classique
        else
        {
            // Répétition pour les deux joueurs
            for(int i = 0; i < 2; i++)
            {
                clear_to_color(buffer, DP_DEFBG);
                //////  AFFICHAGE PRE-PARTIE : JOUEUR QUI JOUE  //////
                // Affichage du texte et des caractéristiques du joueur
                textout_centre_ex(buffer, fonts[5], "TOUR DU JOUEUR", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
                textout_centre_ex(buffer, fonts[4], joueurs[players[i]].nom, SCREEN_W/2, 2 * SCREEN_H/6, DP_DEFTXT, -1);
                masked_stretch_blit(joueurs[players[i]].perso.bmp[0][0], buffer, 0, 0, joueurs[players[i]].perso.dim, joueurs[players[i]].perso.dim,
                                    SCREEN_W/2 - joueurs[players[i]].perso.dim*10/2, 3*SCREEN_H/5 - joueurs[players[i]].perso.dim*10/2, joueurs[players[i]].perso.dim*10, joueurs[players[i]].perso.dim*10);
                // Décompte de 5 secondes de préparation pour le joueur
                for(int i = 5; i >= 0; i--)
                {
                    char tempStr[STR_MAX] = "";
                    sprintf(tempStr, "%d", i);
                    blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    textout_centre_ex(screen, fonts[5], tempStr, SCREEN_W/2, 8 * SCREEN_H/10, DP_DEFTXT, -1);
                    rest(1000);
                }

                // Retrait de ticket pour avoir joué a un jeu
                if(modeJeu != 1) joueurs[players[i]].tickets--;
                // Lancement de la fonction correspondant au jeu voulu et récupération du score afférent
                // (Certains jeux reçoivent des données supplémentaires telles que la police d'écriture ou le visuel du personnage jouant)
                switch(dpGame)
                {
                    case 2:
                        break;
                    case 3:
                        joueurs[players[i]].score[dpGame] = jeuBallons(fonts);
                        break;
                    case 4:
                        joueurs[players[i]].score[dpGame] = snakeGame(fonts);
                        break;
                    case 5:
                        joueurs[players[i]].score[dpGame] = jeuRiviere(fonts);
                        break;
                    case 6:
                        joueurs[players[i]].score[dpGame] = jeuLabyrinthe(fonts, joueurs[players[i]].perso);
                        break;
                    case 7 :
                        joueurs[players[i]].score[dpGame] = jackpotJeu(fonts);
                        break;
                    case 8:
                        joueurs[players[i]].score[dpGame] = guitareHero(fonts);;
                        break;
                    case 9:
                        joueurs[players[i]].score[dpGame] = taupeLa();
                        break;
                    case 10:
                        joueurs[players[i]].score[dpGame] = jeuCanard(fonts);
                        break;
                    default:
                        break;
                }

                clear_to_color(buffer, DP_DEFBG);

                ////// AFFICHAGE POST-PARTIE : SCORE DU JOUEUR  //////
                // Affichage du texte et des caractéristiques du joueur
                textout_centre_ex(buffer, fonts[5], "SCORE DU JOUEUR", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
                textout_centre_ex(buffer, fonts[4], joueurs[players[i]].nom, SCREEN_W/2, 2 * SCREEN_H/6, DP_DEFTXT, -1);
                masked_stretch_blit(joueurs[players[i]].perso.bmp[0][0], buffer, 0, 0, joueurs[players[i]].perso.dim, joueurs[players[i]].perso.dim,
                                    SCREEN_W/2 - joueurs[players[i]].perso.dim*10/2, 3*SCREEN_H/5 - joueurs[players[i]].perso.dim*10/2, joueurs[players[i]].perso.dim*10, joueurs[players[i]].perso.dim*10);
                // Affichage du score
                char tempStr[STR_MAX] = "";
                sprintf(tempStr, "%d", joueurs[players[i]].score[dpGame]);
                strcat(tempStr, " POINTS");
                blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                textout_centre_ex(screen, fonts[5], tempStr, SCREEN_W/2, 8 * SCREEN_H/10, DP_DEFTXT, -1);
                rest(3500);
            }
        }

        //////   AFFICHAGE POST-JEU : GAGNANT & STATS   //////
        // Initilisation du gagnant et du score gagnant à des valeurs basses
        int gagnant = -1, scoregagnant = -1000;
        // Calcul du gagnant
        for(int i = 0; i < 2; i++)
        {
            if(joueurs[players[i]].score[dpGame] > scoregagnant)
            {
                gagnant = i;
                scoregagnant = joueurs[players[i]].score[dpGame];
            }
        }
        if(joueurs[players[0]].score[dpGame] == joueurs[players[1]].score[dpGame])
        {
            gagnant = -1;
        }
        clear_to_color(buffer, DP_DEFBG);
        // Affichage du texte
        textout_centre_ex(buffer, fonts[5], "RESULTAT", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        rest(500);
        // Répétition pour les deux joueurs
        for(int i = 0; i < 2; i++)
        {
            // Affichage des caractéristiques du joueur
            char tempStr[STR_MAX] = "";sprintf(tempStr, "%d", joueurs[players[i]].score[dpGame]);strcat(tempStr, " POINTS  -  ");
            printf("SCORE : %d     MIN : %d     MAX : %d\n", joueurs[players[i]].score[dpGame], bornesScore[dpGame][0], bornesScore[dpGame][1]);
            // Calcul des scores valant un gain ou une perte de tickets additionnels
            if(joueurs[players[i]].score[dpGame] <= bornesScore[dpGame][0] && modeJeu != 1)
            {
                joueurs[players[i]].tickets--;
            }
            if(joueurs[players[i]].score[dpGame] >= bornesScore[dpGame][1] && modeJeu != 1)
            {
                joueurs[players[i]].tickets++;printf("++ ?");
            }
            //Affichage du gagnant et du perdant
            if(i == gagnant)
            {
                strcat(tempStr, "GAGNANT");
                if(modeJeu != 1)
                    joueurs[players[i]].tickets++;
            }
            else
            {
                strcat(tempStr, "PERDANT");
            }
            textout_centre_ex(buffer, fonts[3], joueurs[players[i]].nom, (i+1)*SCREEN_W/3, 2 * SCREEN_H/6, DP_DEFTXT, -1);
            masked_stretch_blit(joueurs[players[i]].perso.bmp[0][0], buffer, 0, 0, joueurs[players[i]].perso.dim, joueurs[players[i]].perso.dim, (i+1)*SCREEN_W/3 - joueurs[players[i]].perso.dim*10/2, 3*SCREEN_H/5 - joueurs[players[i]].perso.dim*10/2, joueurs[players[i]].perso.dim*10, joueurs[players[i]].perso.dim*10);
            textout_centre_ex(buffer, fonts[2], tempStr, (i+1)*SCREEN_W/3, 8 * SCREEN_H/10, DP_DEFTXT, -1);
            // Affichage des tickets en fonction du mode jeu
            if(modeJeu != 1)
                sprintf(tempStr, "%d", joueurs[players[i]].tickets);
            if(modeJeu != 1)
                strcat(tempStr, " TICKETS");
            if(modeJeu != 1)
                textout_centre_ex(buffer, fonts[2], tempStr, (i+1)*SCREEN_W/3, 8 * SCREEN_H/10+ text_height(fonts[2]) + 5, DP_DEFTXT, -1);
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            rest(1500);
        }
        // Envoi des statistiques de jeu pour évaluation et insertion dans le tableau des scores
        pushStats(joueurs, nbJoueurs);
        destroy_bitmap(buffer);
        rest(3000);
    }
}
int chooseSecondPlayer(t_Joueur* joueurs, int nbJoueurs, int playing, FONT** fonts)
{
    //      -- FONCTION FAISANT SELECTIONNER UN JOUEUR AUTRE QUE LE JOUEUR ENVOYE
    //      JOUEURS   : Tableau contenant tous les joueurs
    //      NBJOUEURS : Nombre de joueurs total
    //      PLAYING   : Joueur qui joue
    //      FONTS     : Police d'écriture pour affichage a l'écran
    //      || RETURN :     ID du 2eme joueur selectionne

    BITMAP* buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Création des variables de premier ordre
    char tempStr[STR_MAX] = "";         // String temporaire (réutilisé entre les boucles
    time_t tStop = time(NULL) + 5;      // Mise en place du pas de temps (variation aléatoire du personnage)
    int selection = 0, sprite = 0;      // Sélection de perso pour le joueur et sprite sélectionné

    // Blindage de la selection autour du joueur qui joue
    if(selection < nbJoueurs-1)
    {
        (selection)++;
        rest(100);
    }
    if(selection == playing)
    {
        if(selection+1 > nbJoueurs-1)
        {
            selection--;
        }
        else
        {
            (selection)++;
        }
    }
    // Boucle de selection
    while(!key[KEY_ENTER])
    {
        // Si le pas de temps est dépassé modifier le sprite aléatoirement et relancer le timer
        if(time(NULL) >= tStop)
        {
            tStop = time(NULL) + 3 + rand()%6; sprite = rand()%4;
        };
        clear_to_color(buffer, DP_DEFBG);
        if(keypressed())
        {
            // Faire varier le numero du joueur
            int val = readkey() >> 8;
            if(key[KEY_UP])
            {
                if(selection < nbJoueurs-1)
                {
                    (selection)++;
                    rest(100);
                }
                if(selection == playing)
                {
                    if(selection+1 > nbJoueurs-1)
                    {
                        selection--;
                    }
                    else
                    {
                        (selection)++;
                    }
                }
            }
            else if (key[KEY_DOWN])
            {
                if(selection > 0)
                {
                    (selection)--;
                    rest(100);
                }
                if(selection == playing)
                {
                    if(selection-1 < 0)
                    {
                        selection++;
                    }
                    else
                    {
                        (selection)--;
                    }
                }
            }
        }
        // Afficher a l'écran
        textout_centre_ex(buffer, fonts[5], "SELECTION DU SECOND JOUEUR", SCREEN_W/2, SCREEN_W/10, DP_DEFTXT, -1);
        textout_centre_ex(buffer, fonts[2], "Utilisez les fleches haut et bas pour selectionner, puis entree pour valider", SCREEN_W/2, SCREEN_W/10 + 75, DP_DEFTXT, -1);
        textout_centre_ex(buffer, fonts[4], joueurs[selection].nom, SCREEN_W/2, 2 * SCREEN_H/5, DP_DEFTXT, -1);
        masked_stretch_blit(joueurs[selection].perso.bmp[sprite][0], buffer, 0, 0, joueurs[selection].perso.dim, joueurs[selection].perso.dim, SCREEN_W/2 - joueurs[selection].perso.dim*10/2, 2*SCREEN_H/3 - joueurs[selection].perso.dim*10/2, joueurs[selection].perso.dim*10, joueurs[selection].perso.dim*10);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    // Délai et remise à 0 du keybuf
    rest(175);
    clear_keybuf();
    destroy_bitmap(buffer);
    return selection;
}

void draw_sprite_flip(BITMAP *bmp, BITMAP *sprite, int x, int y, int fw, int fh)
{
    if(fw == 1 && fh == 1)
    {
        draw_sprite_vh_flip(bmp, sprite, x, y);
    }
    else if(fw == 1 && fh == 0)
    {
        draw_sprite_h_flip(bmp, sprite, x, y);
    }
    else if(fw == 0 && fh == 1)
    {
        draw_sprite_v_flip(bmp, sprite, x, y);
    }
    else
    {
        draw_sprite(bmp, sprite, x, y);
    }

}

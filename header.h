#ifndef INC_2023S2_ECEWORLD_HEADER_H
#define INC_2023S2_ECEWORLD_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include <allegro.h>
//#include <windows.h>

#define SCREEN_WIDTH 1280   // Dimension en largeur de l'ecran
#define SCREEN_HEIGHT 768   // Dimension en hauteur de l'ecran
#define STR_MAX 25          // Taille maximale globale pour une chaine de caracteres

#define NB_BMPCOLORS 12     // Nombre de couleurs disponibles pour les bmp de joueurs
#define NB_BMPPOS 4         // Nombre de variations visuelles de position disponibles pour un joueur
#define NB_BMPMOV 4         // Nombre de mouvements disponibles pour une variation visuelle joueur

#define MOVING 6            // Quantite de pixels par déplacement
#define DIFTIME 75          // Nombre de milisecondes entre chaque variation de sprite de mouvement
#define NBGAMES 9           // Nombres de jeux au total

#define DP_DEFBG 1973790    // Couleur de base pour le fond
#define DP_DEFTXT 15329775  // Couleur de base pour le texte


// STRUCTURE DE PERSONNAGE
// Contient toutes les données afférentes a un personnage

typedef struct Personnage_Std{

    BITMAP* bmp[4][4];      // Tableau conteneur d'une couleur de bitmap entière (16 images : (1 pos + 3 mov) * 4 orientations)
    int palette[4];         // Couleurs de la palette de la couleur du joueur
    int dim;                // Dimensions wh d'une image

} t_Perso;


// STRUCTURE DE JOUEURS
// Contient toutes les données afférentes a un des joueurs

typedef struct Joueur_Std{

    // VARIABLES COMMUNES
    char nom[STR_MAX];      // Nom du Joueur
    int tickets;            // Nombre de tickets du joueur
    int score[NBGAMES + 2]; // Score Total du Joueur

    // VARIABLES ACTEUR
    int x,y;                // Position du coin NO
    int dirx, diry;         // Orientation

    int var, nimg;          // Variante, numéro et total d'images

    t_Perso perso;          // Structure de travail de l'affichage

} t_Joueur;



// Fonctions globales
void initialisation(int intro);
void fermeture();



// Initialisation pour le menu
t_Perso* initPersonnages(int dim, char path[]);
FONT** initPolices();
t_Joueur* initJoueurs(int* nbJoueurs, int* modeJeu, FONT* fonts[], t_Perso* perso);

// Affichage du menu et détection de déplacements
void displayMapMenu(int nbJoueurs, FONT** fonts, t_Joueur* joueurs, BITMAP* map, int dpGame, int playing);
int proMov(t_Joueur joueurs, int dimMap);
int retGame(t_Joueur joueurs, int dimMap);

// Déroulé de jeu et choix du second joueur
void playGame(t_Joueur* joueurs, int nbJoueurs, int modeJeu, int dpGame, int playing, FONT** fonts);
int chooseSecondPlayer(t_Joueur* joueurs, int nbJoueurs, int playing, FONT** fonts);

// Fonction utilitaire d'inversion de sprite
void draw_sprite_flip(BITMAP *bmp, BITMAP *sprite, int x, int y, int fw, int fh);

#endif //INC_2023S2_ECEWORLD_HEADER_H
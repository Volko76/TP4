#include <stdio.h>
#include <stdlib.h>
#include "tp4.h"
#define TAILLE_MAX_MOT 64
#define TAILLE_MAX_LIGNE 256


void afficherMenu();
void chargerFichier(T_Index* index);
void afficherCaracteristiquesIndex(T_Index index);
void afficherIndex(T_Index index);
//void rechercherMot(T_Index index);
void afficherOccurrencesMot(T_Index index);
//void construireTexte(T_Index index);
void quitter(T_Index index);

int main() {
    T_Index index;
    index.racine = NULL;
    index.nbMotsDistincts = 0;
    index.nbMotsTotal = 0;

    int choix = 0;
    do {
        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:{
                char s[128];
                scanf("%s", s);
                printf("%s", s);
                index.nbMotsTotal = indexerFichier(&index, s);
                break;
            }
            case 2:
                afficherIndex(index);
                break;
            case 3:
                parcourir(index.racine);
                break;
            case 4:
                //rechercherMot(index);
                break;
            case 5:{
                char* s;
                scanf("%s", s);
                afficherOccurencesMot(index, s);
                break;
            }
            case 6:
                //construireTexte(index);
                break;
            case 7:
                quitter(index);
                break;
            default:
                printf("Choix non valide. Veuillez choisir un nombre entre 1 et 7.\n");
                break;
        }
    } while (choix != 7);

    return 0;
}

void afficherMenu() {
    printf("Menu :\n");
    printf("1. Charger un fichier\n");
    printf("2. Caractéristiques de l'index\n");
    printf("3. Afficher l'index\n");
    printf("4. Rechercher un mot\n");
    printf("5. Afficher les occurrences d'un mot\n");
    printf("6. Construire le texte à partir de l'index\n");
    printf("7. Quitter\n");
    printf("Entrez votre choix : ");
}


void quitter(T_Index index) {
    //faire une sorte de free(index);
    printf("Au revoir !\n");
}


//bvfjdbvlbvglebgvelbgvelbgvle

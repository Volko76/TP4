#include <stdio.h>
#include <stdlib.h>
#include "tp4.h"


T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase) {
    T_Position* nouveau = malloc(sizeof(T_Position));
    nouveau->numeroLigne = ligne;
    nouveau->ordre = ordre;
    nouveau->numeroPhrase = phrase;
    nouveau->suivant = NULL;

    if (listeP == NULL) {
        // La liste est vide, le nouveau nœud devient le premier
        return nouveau;
    } else if (ligne < listeP->numeroLigne ||
               (ligne == listeP->numeroLigne && ordre < listeP->ordre)) {
        // Le nouveau nœud doit être inséré en début de liste
        nouveau->suivant = listeP;
        return nouveau;
    } else {
        // Recherche de l'emplacement adéquat dans la liste
        T_Position* prec = listeP;
        T_Position* courant = listeP->suivant;
        while (courant != NULL &&
               (ligne > courant->numeroLigne ||
                (ligne == courant->numeroLigne && ordre > courant->ordre))) {
            prec = courant;
            courant = courant->suivant;
        }
        // Insertion du nouveau nœud
        nouveau->suivant = courant;
        prec->suivant =nouveau;
        return listeP;
    }
}

int ajouterOccurence(T_Index* index, char* mot, int ligne, int ordre, int phrase) {
    // Conversion en minuscules
    for (int i = 0; mot[i]; i++) {
        //mot[i] = tolower(mot[i]);
        printf("%c", mot[i]);
    }
    printf("|%s]", index->racine->mot);
    //mot = tolower(mot);

    // Recherche du noeud correspondant dans l'ABR
    T_Noeud* n = index->racine;
    T_Noeud* parent = NULL;
    while (n != NULL) {
        int cmp = strcmp(mot, n->mot);
        printf("%s | %s\n", mot, n->mot);
        if (cmp == 0) {
            // Le mot est déjà présent dans l'ABR
            n->nbOccurences++;
            n->listePositions = ajouterPosition(n->listePositions, ligne, ordre, phrase);
            index->nbMotsTotal++;
            printf("On rajoute occu");
            return 1;
        } else if (cmp < 0) {
            // Le mot doit être cherché dans le sous-arbre gauche
            parent = n;
            n = n->filsGauche;
        } else {
            // Le mot doit être cherché dans le sous-arbre droit
            parent = n;
            n = n->filsDroit;
        }
    }

    // Le mot n'est pas encore présent dans l'ABR, on doitcréer un nouveau nœud pour l'insérer
    T_Noeud* nouveau = malloc(sizeof(T_Noeud));
    nouveau->mot = mot;
    //printf(nouveau->mot);
    nouveau->nbOccurences = 1;
    nouveau->listePositions = NULL;
    nouveau->filsGauche = NULL;
    nouveau->filsDroit = NULL;

    nouveau->listePositions = ajouterPosition(nouveau->listePositions, ligne, ordre, phrase);
    index->nbMotsTotal++;

    // Cas particulier : l'ABR est vide, le nouveau nœud devient la racine
    if (parent == NULL) {
        index->racine = nouveau;
        index->nbMotsDistincts++;
        return 1;
    }

    // Insertion du nouveau nœud dans l'ABR
    if (strcmp(mot, parent->mot) < 0) {
        parent->filsGauche = nouveau;
    } else {
        parent->filsDroit = nouveau;
    }
    index->nbMotsDistincts++;
    return 1;
}

int estSeparateur(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' || c == ';' ||
            c == ':' || c == '-' || c == '"' || c == '\'' || c == '(' || c == ')' || c == '[' ||
            c == ']' || c == '{' || c == '}' || c == '<' || c == '>' || c == '/' || c == '\\' ||
            c == '|' || c == '?' || c == '!' || c == '.' || c == '«' || c == '»');
}

int indexerFichier(T_Index* index, char* filename) {
    FILE* fichier = fopen(filename, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    int nbMotsLus = 0;
    char mot[TAILLE_MAX_MOT];
    int ligne = 1, ordre = 1, phrase = 1;
    char buff[255];

    /*

    for (int i = 0; i < 5; i++){
        fscanf(fichier, "%s", buff);
        printf("MOT : %s\n", buff);
    }
    printf("%s", fgets(mot, TAILLE_MAX_MOT, fichier));*/


    while (fgets(mot, TAILLE_MAX_MOT, fichier) != NULL) {
        char* ptr = mot;
        while (*ptr != '\0') {
            if (estSeparateur(*ptr)) {
                // Le caractère courant est un séparateur de mot
                *ptr = '\0';
                if (strlen(mot) > 0) {
                    ajouterOccurence(index, mot, ligne, ordre, phrase);
                    printf("ADDED");
                    nbMotsLus++;
                }
                mot[0] = '\0';
                ordre++;
            } else if (*ptr == '.') {
                // Le caractère courant est un point, on incrémente le numéro de phrase
                phrase++;
                ordre = 1;
            } else {
                // Le caractère courant est une lettre, on ajoute le caractère aumot en cours de construction
                strncat(mot, ptr, 1);
            }
            ptr++;
        }
        ligne++;
        ordre = 1;
    }

    fclose(fichier);
    return nbMotsLus;
}

void afficherPositions(T_Position* lp) {
    while (lp != NULL) {
        printf("    ligne %d, ordre %d, phrase %d\n", lp->numeroLigne, lp->ordre, lp->numeroPhrase);
        lp = lp->suivant;
    }
}

void afficherABR(T_Noeud* n) {
    if (n == NULL) {
        return;
    }
    afficherABR(n->filsGauche);
    printf("%s : %d occurrences\n", n->mot, n->nbOccurences);
    afficherPositions(n->listePositions);
    afficherABR(n->filsDroit);
}

void parcourir(struct Noeud *racine) {

   if (racine == NULL) return;

   parcourir(racine->filsGauche);
   printf("%s ", racine->mot);
   parcourir(racine->filsDroit);

}

void afficherIndex(T_Index index) {
    printf("Index des mots :\n");
    parcourir(index.racine);
    //afficherABR();
}

T_Noeud* rechercherMot(T_Index index, char* mot) {
    T_Noeud* n = index.racine;
    while (n != NULL) {
        int cmp = strcmp(mot, n->mot);
        if (cmp == 0) {
            // Le mot a été trouvé dans l'index
            return n;
        } else if (cmp < 0) {
            // Le mot est plus petit que le mot courant, on cherche dans le sous-arbre gauche
            n = n->filsGauche;
        } else {
            // Le mot est plus grand que le mot courant, on cherche dans le sous-arbre droit
            n = n->filsDroit;
        }
    }
    // Le mot n'a pas été trouvé dans l'index
    return NULL;
}

void afficherOccurencesMot(T_Index index, char* mot) {
    /*
    T_Noeud* n = rechercherMot(index, mot);
    if (n == NULL) {
        printf("Le mot %s n'a pas été trouvé dans le texte.\n", mot);
        return;
    }

    T_Position* lp = n->listePositions;
    while (lp != NULL) {
        char ligne[TAILLE_MAX_LIGNE];
        if (lireLigneFichier(ligne, lp->numeroLigne, index.racine) != -1) {
            printf("Phrase %d : %s\n", lp->numeroPhrase, ligne);
        }
        lp = lp->suivant;
    }*/
}

void construireTexte(T_Index index, char* filename) {
    /*
    ListePhrases* listePhrases = trierPhrases(index);
    if (listePhrases == NULL) {
        printf("Erreur : impossible de construire le texte.\n");
        return;
    }

    FILE* fichier = fopen(filename, "w");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s en écriture.\n", filename);
        return;
    }

    MaillonPhrase* mp = listePhrases->debut;
    while (mp != NULL) {
        fprintf(fichier, "%s\n", mp->phrase);
        mp = mp->suivant;
    }

    fclose(fichier);
    libererListePhrases(listePhrases);
    */
}

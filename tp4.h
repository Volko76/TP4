#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED
#define TAILLE_MAX_MOT 64
#define TAILLE_MAX_LIGNE 2048
#define TAILLE_MAX_NOM_FICHIER 512

typedef struct Position T_Position;
struct Position {
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position* suivant;
};

typedef struct Noeud T_Noeud;
struct Noeud {
    char* mot;
    int nbOccurences;
    T_Position* listePositions;
    struct Noeud* filsGauche;
    struct Noeud* filsDroit;
};

typedef struct Index T_Index;
struct Index {
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
};

T_Position* ajouterPosition(T_Position* listeP, int ligne, int ordre, int phrase);
int ajouterOccurence(T_Index* index, char* mot, int ligne, int ordre, int phrase);
int indexerFichier(T_Index* index, char* filename);
void afficherIndex(T_Index index);
T_Noeud* rechercherMot(T_Index index, char* mot);
void afficherOccurencesMot(T_Index index, char* mot);
void construireTexte(T_Index index, char* filename);

#endif // TP4_H_INCLUDED

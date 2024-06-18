#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/***************************
 *Déclaration des constantes
 **************************/
#define N 4
#define TAILLE (N * N)
const int VIDE = -1;
const int NB_TIRET = N*N;
const char TIRET = '-';
const char INTERSECTION = '+';
const char BARRE = '|';
const char VALEUR_NULLE = '.';

/***********************
 *Déclaration des types
 **********************/
typedef struct {
  int valeur;
  int candidats[TAILLE];
  int nbCandidats;
} tCase1;
typedef tCase1 tGrille[TAILLE][TAILLE];

/***********
 *Prototypes
 **********/
void ajouterCandidat(tCase1 *laCase, int val);
int chargerGrille(tGrille g, char nomFichier[30]);
void initialiserCandidats(tGrille g);
void afficherGrille(tGrille g);
void nouveauCandidat(tGrille g, int i, int j);
bool possible(tGrille grille, int numLig, int numCol, int valInsert);
bool backtracking(tGrille grille, int numeroCase);
int singletonNu(tCase1 *laCase, int *numeroCase);
int singletonCache(tGrille g, int i, int j);


/***************************
  Début du programme principal
***************************/

int main() {
  tGrille g;
  int val;
  int ind;
  int progression, i, j;
  int nbCasesVidesDebut;
  int nbCasesVidesFin;
  char nomFichier[30];
  printf("Nom du fichier ? ");
  scanf("%s", nomFichier);
  nbCasesVidesDebut = chargerGrille(g, nomFichier);
  nbCasesVidesFin = nbCasesVidesDebut;
  printf("Nombre de cases vides : %d\n", nbCasesVidesDebut);
  clock_t debut = clock();
  initialiserCandidats(g);
  progression = 1;
  afficherGrille(g);


  while (nbCasesVidesFin != 0 && progression == 1) {
    initialiserCandidats(g);
    progression = 0;
    for (i = 0; i < TAILLE; i++) {
      for (j = 0; j < TAILLE; j++) {
        nouveauCandidat(g, i, j);

        // technique du singleton nu
        if(g[i][j].valeur == 0){
          progression=singletonNu(&g[i][j], &nbCasesVidesFin);
        }
      }
    }
  }
  backtracking(g,0);

  printf("\n");

  afficherGrille(g);

  clock_t fin = clock();
  double tmpsCPU = (fin - debut)*1.0 / CLOCKS_PER_SEC;
  printf( "Temps CPU = %f secondes\n",tmpsCPU);
  return EXIT_SUCCESS;
}

/***************************
  Fin du programme principal
***************************/

void ajouterCandidat(tCase1 *laCase, int val) {
  (*laCase).candidats[(*laCase).nbCandidats] = val;
  (*laCase).nbCandidats = (*laCase).nbCandidats + 1;
}


int chargerGrille(tGrille g, char nomFichier[30]) {
  FILE *f;
  int i, j;
  int cpt = 0;
  int nbTemp;
  f = fopen(nomFichier, "rb");
  if (f == NULL) {
    printf("\n ERREUR sur le fichier %s\n", nomFichier);
  } else {
    for (i = 0; i < TAILLE; i++) {
      for (j = 0; j < TAILLE; j++) {
        fread(&(g[i][j].valeur), sizeof(int), 1, f);
        nbTemp = g[i][j].valeur;
        if (nbTemp == 0) {
          cpt++;
        }
      }
    }
    fclose(f);
  }
  return cpt;
}

void initialiserCandidats(tGrille g) {
  int i, j, ind;
  for (i = 0; i < TAILLE; i++) {
    for (j = 0; j < TAILLE; j++) {
      for (ind = 0; ind < TAILLE; ind++) {
        g[i][j].candidats[ind] = VIDE;
      }
      g[i][j].nbCandidats = 0;
    }
  }
}

void afficherGrille (tGrille grille){
    //Déclaration variables
    int i; // indice de ligne et de ligne de séparation
    int j; // indice de colonne et de ligne de séparation
    int indiceLigne;

    //Numéros de colonne


    //Ligne de données
    for (i=0 ; i<TAILLE ; i++){

        //Lignes de séparation
        if (i%N==0){
            printf("%c",INTERSECTION);
            for (indiceLigne=0;indiceLigne<N;indiceLigne++){
                for (j=0;j<NB_TIRET;j++){
                    printf("%c",TIRET);
                }   
                printf("%c",INTERSECTION);
            }
            printf("\n");
        }

        //Affichage des valeurs
        printf("%c",BARRE);
        for (j=0 ; j<TAILLE ; j++){
            if (j%N==0 && j!=0){
                printf("%c",BARRE);
            }
            if (grille[i][j].valeur != 0){
                printf(" %2d ",grille[i][j].valeur);
            }else{
                printf(" %2c ",VALEUR_NULLE);
            }
        }
        printf("%c",BARRE);
        printf("\n");
    }


    //Dernière ligne de séparation
    if (i%N==0){
        printf("%c",INTERSECTION);
        for (indiceLigne=0;indiceLigne<N;indiceLigne++){
            for (j=0;j<NB_TIRET;j++){
                printf("%c",TIRET);
            }   
            printf("%c",INTERSECTION);
        }
        printf("\n");
    }
}

void nouveauCandidat(tGrille g, int i, int j) {
  bool valPossible;
  int val;
  int ind;
  int lig;
  int col;

  for (val = 1; val <= TAILLE; val++) {
    valPossible = true;
    for (ind = 0; ind < TAILLE; ind++) {
      // test de la ligne
      if (g[i][ind].valeur == val) {
        valPossible = false;
      }

      // test de la colonne
      if (g[ind][j].valeur == val) {
        valPossible = false;
      }
    }
    // test du bloc
    for (lig = 0; lig < N; lig++) {
      for (col = 0; col < N; col++) {
        if (g[i / N * N + lig][j / N * N + col].valeur == val) {
          valPossible = false;
        }
      }
    }

    if (valPossible) {
      ajouterCandidat(&g[i][j], val);
    }
  }
}


bool possible(tGrille grille, int numLig, int numCol, int valInsert) {
  int lig;
  int col;
  bool valPossible;

  valPossible = true;
  // Vérification de la ligne
  for (col = 0; col < TAILLE; col++) {
    if (grille[numLig][col].valeur == valInsert) {
      valPossible = false;
    }
  }

  // Vérification de la colonne
  for (lig = 0; lig < TAILLE; lig++) {
    if (grille[lig][numCol].valeur == valInsert) {
      valPossible = false;
    }
  }

  // Vérification de la case
  for (lig = 0; lig < N; lig++) {
    for (col = 0; col < N; col++) {
      if (grille[numLig / N * N + lig][numCol / N * N + col].valeur == valInsert) {
        valPossible = false;
      }
    }
  }

  return valPossible;        



}
bool backtracking(tGrille grille, int numeroCase){
  int ligne;
  int colonne;
  bool resultat;
  resultat = false;

  //on a traité toutes les cases, la grille est résolue
  if (numeroCase == TAILLE * TAILLE){
    resultat = true;
  }
  else{
    // On récupère les "coordonnées" de la case
    ligne = numeroCase / TAILLE;
    colonne = numeroCase % TAILLE;
    if (grille[ligne][colonne].valeur != 0) {
      // Si la case n’est pas vide, on passe à la suivante
      // (appel récursif)
      resultat = backtracking(grille, numeroCase+1);
    }
    else{
      for(int valeur = 1; valeur < TAILLE+1; valeur++ ){
        if (possible(grille, ligne, colonne, valeur)==true){
          // Si la valeur est autorisée on l'inscrit
          //dans la case...
          grille[ligne][colonne].valeur = valeur;
          // ... et on passe à la suivante : appel récursif
          // pour voir si ce choix est bon par la suite
          if (backtracking(grille,numeroCase+1) == true) {
            resultat = true;
          }
          else{
            grille[ligne][colonne].valeur = 0;
          }
        }
      }
    }
  }
  return resultat;
}

int singletonNu(tCase1 *laCase, int *numeroCase){
  int val;
  if ((*laCase).nbCandidats == 1 && (*laCase).valeur == 0) {
    val = (*laCase).candidats[0];
    (*laCase).valeur = val;
      numeroCase = numeroCase - 1;
    // retirer ce candidat de toutes les cases de la
    // même ligne, de la même colonne et du même bloc

    return 1;
  }
  return 0;
}
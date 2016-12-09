#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<conio.h>

int n; //le nombre de reine

//déclaration d'un élément de l'arborescence
typedef struct ne *Pnoeud;
typedef struct ne{
        int *t;//vecteur : indice ligne = n° de reine, élément = N° de colonne reine
        int f;// valeur de la fonction objective
}noeud;

//Procédure générant une solution aléatoire
int* solution (){
     srand(clock());
     int* t = (int*) malloc(sizeof(int)*n);
     int i;
     for(i = 0 ; i < n ; i++){
     *(t+i) = rand() % n + 0;
     }
     return t;
}

//Procédure retournant la valeur de la fonction objective étant donné une solution
int f (int *t){// t est le vecteur du noeud courant
    int libre=1, i, c, lr, cr, nb_reine_menace = 0; //libre : variable booléenne (0 = menacée, 1 = libre)| i, k : indice
    // pour parcourir le vecteur t| c : est un temporaire pour la colonne de la reine courante
    // lr : la ligne de la reine courante (on cherche à savoir si elle est menacée ou pas)
    // cr : la colonne de la reine courante
    // nb_reine_menace : le nombre de reines qui sont menacées
    for (lr = 0 ; lr < n ; lr++){
        cr = *(t+lr);
        libre = 1;
        i = 0;
        while (i < n && libre){
              if (i!=lr){// si la ligne ne correspond pas à la ligne de la reine courante (on ne va pas tester si elle se menace elle-même)
                  c=*(t+i);
                  if((cr==c) || (fabs(lr-i) == fabs(cr-c))){// ce test vérifie les conditions suivantes :
                             //- cr == c : s'il existe une autre reine sur la même colonne
                             //- fabs(lr-i) == fabs(cr-c) : s'il existe une autre reine sur les digonale montante/descendante. fabs : valeur absolue
                             // RMQ : la condition : une seule reine par ligne, est trivial, car une seule reine est placée par ligne. (modélisation par vecteur de l'échiquier)
                       libre=0;nb_reine_menace++;
                  }
                  else
                       i++;
              }
              else
                   i++;
        }
    }
    return nb_reine_menace;
}


//Procédure copier un noeud dans un noeud
void copie_noeud(Pnoeud sol1, Pnoeud sol2){//copie le contenu du pointeur sol2 vers le pointeur sol1
     int i;int* ts1 = sol1 -> t; int* ts2 = sol2 -> t;
     for (i = 0 ; i < n ; i++)
         ts1[i] = ts2[i];
     sol1 -> f = sol2 -> f;
}

//Procédure copier un noeud dans un noeud sans la valeur de f
void copie1_noeud(Pnoeud sol1, Pnoeud sol2){//copie le contenu du pointeur sol2 vers le pointeur sol1
     int i;int* ts1 = sol1 -> t; int* ts2 = sol2 -> t;
     for (i = 0 ; i < n ; i++)
         ts1[i] = ts2[i];
}

//Procédure calcule la distance de Hamming entre 2 solutions
int distance(Pnoeud sol1, Pnoeud sol2){
    int dist = 0, i;
    for(i=0; i<n; i++){
        if((*((sol1 -> t)+i)) != (*((sol2 -> t)+i))){//Si reine placée à différente position alors incrémenter de 1 la distance
            dist = dist + 1;
        }
    }
}


//méthode affichage d'un noeud
void affiche_noeud(Pnoeud p, int numV){
     // p : pointeur sur le noeud (solution) courant
     // numV : le numéro du voisinage de la solution
     int i;
     printf("  La solution du voisinage N° %d \n", numV);
     for(i = 0 ; i < n ; i++)
     printf("  T[ %d ] = %d ", i, (*((p->t)+i)));
     printf("\n \n");
     printf("  Valeur de la fonction objective : %d ", p->f);
     printf("\n \n");
}

main (){

     int i, j, k, l, trouve = 0, existe = 0, nb_voisinage = 1;int * tp, nb_iter = 50;//Variables temporaires
     //trouve : prend la valeur 1 si on trouve un meilleur optimum local, 0 sinon

     clock_t deb,fin; //Variables ancres pour définir le temps d'exécution
     float delta;

     printf("Entrez la taille de votre échiquier (n) : \n");
     scanf("%d",&n);


     int taboue[n][n]; //Matrice modélisation de la liste taboue
                       //taboue[i,j] = 0 si déplacement (i,j) non taboue, 1 sinon
     //Boucle initialisation de la matrice taboue --> tout déplacment est permis
     for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            taboue[i][j] = 0;
        }
     }
                  /*    ---------------Traitement-----------------   */
             //Politique suivie :
             //- Recherche Taboue : intensification & diversification.
             //
             //- Voisinage à point : première solution meilleure par rapport à la solution meilleure fixé au voisinage precédent,
             //  prochaine à développer (chercher dans son voisinage)
     Pnoeud sol = (Pnoeud)malloc(sizeof(noeud)); //sol : pointeur sur  la solution initiale
     sol -> t = solution();
     sol -> f = f(sol -> t);

     //Mettre les déplacments de la solution initiale dans la liste taboue
     for(i=0; i<n; i++){
        taboue[i][*((sol -> t)+i)] = 1;
        //printf("%d", taboue[i][*((sol -> t)+i)]);
     }

     //Affichage de la solution
     affiche_noeud(sol, 0);

     Pnoeud solc = (Pnoeud)malloc(sizeof(noeud)); //solc : pointeur sur  la solution courante
     solc -> t = (int*)malloc(n*sizeof(int));
     copie_noeud(solc, sol);//solc est initialisé aux valeurs de la solution initiale

     Pnoeud sols = (Pnoeud)malloc(sizeof(noeud)); //sols : pointeur sur  une solution du voisinage
     sols -> t = (int*)malloc(n*sizeof(int));

     deb = clock();//Ancre : début du traitement

     k = 0;
     while (k < nb_iter && (solc -> f) != 0){
         i = 0; trouve = 0;
         while (i < n && !trouve){//Le numéro / la ligne de la reine
               j = 0;
               copie1_noeud(sols, solc);//sols : doit contenir au départ la configuration de solc
               while (j < n && !trouve){//Valeurs possibles de l'emplacement de la reine (les colonnes)
                     if (j != *(solc -> t + i)){
                        *(sols -> t + i) = j;
                        sols -> f = f(sols -> t);
                        //Affiche_noeud(sols, nb_voisinage);
                        if (sols -> f < solc -> f){
                           if (!taboue[i][j]){
                               taboue[i][j] = 1;
                               trouve = 1; nb_voisinage++;
                               copie_noeud(solc, sols);
                           }
                        }
                     }
                     j++;
               }
               i++;
         }
         //Après avoir explorer un voisinage
         if (!trouve){//On n'a pas trouvé de meilleure solution. Ce qui veut dire qu'on a atteint un optimum local
            //Traitement de la diversification
            i = 0; existe = 0;
            while (i < n && !existe){
                while (j < n && !existe){
                    if (taboue[i][j] = 0){
                        (*((solc -> t)+i))=j;
                        existe = 1;
                    }
                }
            }
            if (!existe){//Tous les déplacements possibles sont taboux
                //Initialisation du max distance à la première valeur
                (*((sols -> t)+0))=;
                distance(solc, sols);
                for(i=0; i<n; i++){
                    for(j=0; j<n; j++){
                        (*((sols -> t)+i))=j;
                        distance(solc, sols);


                    }
                }
            }
         }
         k++;
     }

     fin = clock();//ancre : fin du traitement

     //afficher la solution finale
     affiche_noeud(solc, nb_voisinage);

     /*
     //libérer l'espace mémoire occupé par le noeud développé
     free(p -> t);
     free(p);
     */

     delta = (float) (fin-deb)/CLOCKS_PER_SEC;
     printf(" \n Le temps d'exécution: %f \n",delta);
     printf(" \n Le nombre de voisinages parcourus : %d \n", nb_voisinage);


     getch();
}


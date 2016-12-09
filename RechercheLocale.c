#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<conio.h>

int n; //le nombre de reine 

//d�claration d'un �l�ment de l'arborescence
typedef struct ne *Pnoeud; 
typedef struct ne{
        int *t;//vecteur : indice ligne = n� de reine, �l�ment = N� de colonne reine 
        int f;// valeur de la fonction objective  
}noeud;

//Proc�dure g�n�rant une solution al�atoire
int* solution (){
     srand(clock());
     int* t = (int*) malloc(sizeof(int)*n);
     int i;
     for(i = 0 ; i < n ; i++){ 
     *(t+i) = rand() % n + 0;
     }
     return t;
}

//Proc�dure retournant la valeur de la fonction objective �tant donn� une solution
int f (int *t){// t est le vecteur du noeud courant 
    int libre=1, i, c, lr, cr, nb_reine_menace = 0; //libre : variable bool�enne (0 = menac�e, 1 = libre)| i, k : indice 
    // pour parcourir le vecteur t| c : est un temporaire pour la colonne de la reine courante
    // lr : la ligne de la reine courante (on cherche � savoir si elle est menac�e ou pas)
    // cr : la colonne de la reine courante 
    // nb_reine_menace : le nombre de reines qui sont menac�es 
    for (lr = 0 ; lr < n ; lr++){
        cr = *(t+lr);
        libre = 1;
        i = 0;
        while (i < n && libre){
              if (i!=lr){// si la ligne ne correspond pas � la ligne de la reine courante (on ne va pas tester si elle se menace elle-m�me)
                  c=*(t+i);
                  if((cr==c) || (fabs(lr-i) == fabs(cr-c))){// ce test v�rifie les conditions suivantes :
                             //- cr == c : s'il existe une autre reine sur la m�me colonne
                             //- fabs(lr-i) == fabs(cr-c) : s'il existe une autre reine sur les digonale montante/descendante. fabs : valeur absolue
                             // RMQ : la condition : une seule reine par ligne, est trivial, car une seule reine est plac�e par ligne. (mod�lisation par vecteur de l'�chiquier)   
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


//Proc�dure copier un noeud dans un noeud 
void copie_noeud(Pnoeud sol1, Pnoeud sol2){//copie le contenu du pointeur sol2 vers le pointeur sol1
     int i;int* ts1 = sol1 -> t; int* ts2 = sol2 -> t;
     for (i = 0 ; i < n ; i++)
         ts1[i] = ts2[i];
     sol1 -> f = sol2 -> f;           
}

//Proc�dure copier un noeud dans un noeud sans la valeur de f
void copie1_noeud(Pnoeud sol1, Pnoeud sol2){//copie le contenu du pointeur sol2 vers le pointeur sol1
     int i;int* ts1 = sol1 -> t; int* ts2 = sol2 -> t;
     for (i = 0 ; i < n ; i++)
         ts1[i] = ts2[i];    
}


//m�thode affichage d'un noeud
void affiche_noeud(Pnoeud p, int numV){
     // p : pointeur sur le noeud (solution) courant
     // numV : le num�ro du voisinage de la solution 
     int i;
     printf("  La solution du voisinage N� %d \n", numV);
     for(i = 0 ; i < n ; i++)
     printf("  T[ %d ] = %d ", i, (*((p->t)+i)));
     printf("\n \n");
     printf("  Valeur de la fonction objective : %d ", p->f);
     printf("\n \n");
}

main (){  
     
     int i, j, k, l, trouve = 0, nb_voisinage = 1;int * tp, nb_iter = 50;//variables temporaires
     //trouve : prend la valeur 1 si on trouve un meilleur optimum local, 0 sinon 
     
     clock_t deb,fin; //variables ancres pour d�finir le temps d'ex�cution
     float delta;
     
     printf("Entrez la taille de votre �chiquier (n) : \n");
     scanf("%d",&n);
     
                  /*    ---------------Traitement-----------------   */
             //Politique suivie : 
             //- Recherche locale : intensification seulement - optimum local (1�re et seule solution 
             //  � partir de laquelle on inf�re d'autres depuis son voisinage, etc.) 
             //- Voisinage � point : premi�re solution meilleure par rapport � la solution meilleure fix� au voisinage prec�dent, 
             //  prochaine � d�velopper (chercher dans son voisinage) 
     Pnoeud sol = (Pnoeud)malloc(sizeof(noeud)); //sol : pointeur sur  la solution initiale
     sol -> t = solution();
     sol -> f = f(sol -> t);
     
     //affichage de la solution
     affiche_noeud(sol, 0);
     
     Pnoeud solc = (Pnoeud)malloc(sizeof(noeud)); //solc : pointeur sur  la solution courante
     solc -> t = (int*)malloc(n*sizeof(int));
     copie_noeud(solc, sol);//solc est initialis� aux valeurs de la solution initiale
     
     Pnoeud sols = (Pnoeud)malloc(sizeof(noeud)); //sols : pointeur sur  une solution du voisinage
     sols -> t = (int*)malloc(n*sizeof(int));
     
     deb = clock();//ancre : d�but du traitement 
     
     k = 0;
     while (k < nb_iter && (solc -> f) != 0){
         i = 0; trouve = 0;
         while (i < n && !trouve){//le num�ro / la ligne de la reine
               j = 0;
               copie1_noeud(sols, solc);//sols : doit contenir au d�part la configuration de solc 
               while (j < n && !trouve){//valeurs possibles de l'emplacement de la reine (les colonnes)
                     if (j != *(solc -> t + i)){
                        *(sols -> t + i) = j; 
                        sols -> f = f(sols -> t);
                        //affiche_noeud(sols, nb_voisinage);
                        if (sols -> f < solc -> f){ 
                           trouve = 1; nb_voisinage++;
                           copie_noeud(solc, sols);
                        }
                     }
                     j++;
               }
               i++;
         }
         k++;
     }
     
     fin = clock();//ancre : fin du traitement
     
     //afficher la solution finale
     affiche_noeud(solc, nb_voisinage);
     
     /*    
     //lib�rer l'espace m�moire occup� par le noeud d�velopp� 
     free(p -> t);
     free(p);
     */
     
     delta = (float) (fin-deb)/CLOCKS_PER_SEC; 
     printf(" \n Le temps d'ex�cution: %f \n",delta);
     printf(" \n Le nombre de voisinages parcourus : %d \n", nb_voisinage);
     
     
     getch();
}
       

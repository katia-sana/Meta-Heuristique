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

//déclaration de la liste Pop : contient les noeuds générés à développer triés dans l'ordre croissant selon la valeur de f(noeud)
typedef  struct no *Liste;
typedef  struct no{
         Pnoeud elt;
         Liste svt;
}noeud1;

//créer un élément de la liste Pop
Liste creer_elt(){
    Liste L= (Liste)malloc(sizeof(noeud1));
    if ( !L){
        printf("erreur d'allocation\n");
        exit(-1);
    }
    return(L);
}

//Prcédure d'ajout d'un noeud à la position adéquate selon la valeur de f(noeud) à la liste Pop
void ajout(Liste *Pop, Liste  *Q_Pop, Pnoeud E){
    Liste p, q, nouv = creer_elt();//p : pointeur sur l'élément précédant l'élément courant, q : poiteur sur l'élément courant.
    int Fe, Fq;//Fe : la valeur de la fonction f pour le noeud *E, Fq : la valeur de la fonction f pour le noeud *q.
    //début traitement
    nouv -> elt = E;

    if (*Pop != NULL){//si la liste Pop n'est pas vide
        p = *Pop;
        q = *Pop;
        Fe = E -> f;
        Fq = q -> elt -> f;

        //Boucle while pour determiner la position et se fera l'insertion
        while ((Fe >= Fq) && (q != *Q_Pop)){
              p = q;
              q = q -> svt;
              Fq = q -> elt -> f;
        }
        if (q != *Q_Pop){//si la condition de sortie de la boucle est Fe < Fq seulement
              nouv -> svt = q;
              if (q == *Pop)//si l'insertion se fait à la première position, il faut mettre à jour Pop
                 *Pop = nouv;
              else
                 p -> svt = nouv;
        }
        else {// si la condition de sortie de la boucle est q == *Q_Pop
              if (Fe < Fq){// si la position adéquate est celle avant le dernier élément
                 nouv -> svt = q;
                 if (*Pop != *Q_Pop){//si la liste ne contient qu'un seul élément
                     p -> svt = nouv;
                 }
                 else{
                     *Pop = nouv;
                 }
              }
              else {//  si la position adéquate est celle après le dernier élément
                 q -> svt = nouv;
                 nouv -> svt = NULL;
                 *Q_Pop = nouv; //MAJ du dernier élément (Q_Pop) de la liste Pop
              }
        }
    }
    else {//si liste Pop est vide
        nouv -> svt = NULL;
        *Pop = nouv;
        *Q_Pop = *Pop;
    }
}

//Procédure suppression d'un élément de la liste
void supprime(Liste Pred, Liste P){
     Pred -> svt = P -> svt;
     P -> svt = NULL;
     free(P);
}

//Procédure Libérer espace
void libere(Pnoeud p){
     //libérer l'espace mémoire occupé par l'individu
     free(p -> t);
     free(p);
}

//Procédure remplacement : supprime les individus en queue de liste à partir de la position taille_pop (les individus de basse qualité)
void remplacement(Liste Pop, Liste* Q_Pop, int taille_pop){
     int i = 1;
     Liste temp;
     //préparer la procédure de remplacement
     //en positionnant la tête sur le premier individu à supprimer
     //soit à la position taille_pop
     while (i < taille_pop){
           Pop = Pop -> svt;
           i++;
     }
     //Mettre à jour Q_Pop et mettre null dans le svt du futur dernier élément
     temp = Pop -> svt;
     *Q_Pop = Pop;
     Pop -> svt = NULL;
     Pop = temp;
     //supprimer tant qu'il reste des éléments
     while (Pop != NULL){
          temp = Pop;
          Pop = Pop -> svt;
          libere(temp -> elt);
          free(temp);
     }

}

//Procédure : retourne une valeur comprise dans l'intervalle [0-n[
int zero_to_n (){
    int number;
    number = rand() % n + 0;
    return number;
}

//Procédure : retourne une valeur comprise dans l'intervalle [1-n[
int one_to_n(){
    int number;
    number = rand() % (n-1) + 1;
    return number;
}

//Procédure générant une solution aléatoire
Pnoeud solution (){
     srand(clock());
     Pnoeud s = (Pnoeud)malloc(sizeof(noeud));
     s -> t = (int*) malloc(sizeof(int)*n);
     int i;
     for (i = 0 ; i < n ; i++){
         *(s -> t + i) = rand() % n + 0;
     }
     s -> f = f(s -> t);
     return s;
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
              }
              i++;
        }
    }
    return nb_reine_menace;
}

//Procédure de croisement
void croisement(Pnoeud s1, Pnoeud s2, int pc, Pnoeud *s12, Pnoeud *s21){
     int i;
     //Allocation mémoire pour les noeuds fils
     (*s12) = (Pnoeud)malloc(sizeof(noeud));
     (*s12) -> t= (int*)malloc(n*sizeof(int));
     (*s21) = (Pnoeud)malloc(sizeof(noeud));
     (*s21) -> t= (int*)malloc(n*sizeof(int));

     //traitement
     for (i=0; i<n; i++){
         if (i < pc){
               *((*s12)->t + i)=*(s1 -> t + i);
               *((*s21)->t + i)=*(s2 -> t + i);
         }
         else {
               *((*s12)->t + i)=*(s2 -> t + i);
               *((*s21)->t + i)=*(s1 -> t + i);
         }
     }
     //calculer la valeur de f(s)
     (*s12) -> f = f((*s12) -> t);
     (*s21) -> f = f((*s21) -> t);
}

//Procédure de mutation
void mutation(Pnoeud s){
     int nb_modif =(int) ((zero_to_n())/2)+1, i, indice_modif; //nb_modif : nombre de cases à modifier (jamais égal à 0
                                                               //         et ne dépasse pas la moitié du nombre de case du tableau)
                                                               //indice_modif :  indice de la case à modifier
     for (i=0; i<nb_modif; i++){
         indice_modif = zero_to_n();
         *(s -> t + indice_modif) = zero_to_n();
     }
     s -> f = f(s -> t);//mettre à jour la valeur de la fonction objective

}

//création de la population initiale
void initialisation(Liste* Pop, Liste* Q_Pop, int taille_pop){
     int i; Pnoeud E;
     //créer une solution : premier individu de la population
     (*Pop) = creer_elt();
     (*Pop) -> elt = solution();
     (*Pop) -> svt = NULL;

     //initialiser la queue de liste par le valeur de la tête Pop
     (*Q_Pop) = (*Pop);
     //printf("solution 0 dans liste Pop \n");
     i = 1; //initialiser à 1 car une solution est déjà générée
     while (i < taille_pop){
         E = solution();
         ajout(Pop, Q_Pop, E);
         //printf("solution %d dans liste Pop \n",i);
         i++;
     }
}

//méthode affichage d'un noeud
void affiche_noeud(Pnoeud p){
     // p : pointeur sur le noeud (solution) courant
     int i;
     printf("  La solution la plus optimale trouvée :\n");
     for(i = 0 ; i < n ; i++)
     printf("  T[ %d ] = %d ", i, (*((p->t)+i)));
     printf("\n \n");
     printf("  Valeur de la fonction objective : %d ", p->f);
     printf("\n \n");
}

main (){

     int i, j, k, l, pointc, Tc, Tm;//Variables temporaires
     //trouve : prend la valeur 1 si on trouve un meilleur optimum local, 0 sinon
     //Tc et Tm sont les Taux de croisement et d mutation respectivement.

     Liste Pop, Q_Pop, P1, P2, Pred;//Pop : est la tête de la liste contenant la population
                              //Q_Pop : est la queue de la liste
                              //P1 et P2 sont les pointeurs sur les parents du prochain croisement
                              //Pred : Sauvegarde le précédent pour la gestion de la mutation

     Pnoeud TempFils[10000];
     int nTF = 0;

     Pnoeud s1, s2, sm; //fils issus d'un croisement / mutation

     srand(clock());//initialisé la racine du rand

     clock_t deb,fin; //Variables ancres pour définir le temps d'exécution
     float delta;

     printf("Entrez la taille de votre échiquier (n) : \n");
     scanf("%d",&n);

     int taille_pop = 2*n;//taille de la population initiale
     //Sachant qu'un croisement requiert de deux individus donc son taux sera calculé à partir de 50% de la population
     int taille_pop75 = (int)(taille_pop*75/100)/2;
     int taille_pop25 = (int)taille_pop*25/100;

     int nb_iter = 100*n;//nombre d'itération (de génération)

     deb = clock();//Ancre : début du traitement

     initialisation(&Pop, &Q_Pop, taille_pop);

     i = 0;
     while ((i < nb_iter) && (Pop -> elt -> f != 0)){
           //réinitialiser les paramètres d'exécution
           P1 = Pop; P2 = Pop -> svt; nTF = 0;
           //Croisement
           j = 0; Tc = rand() % (taille_pop75-1) + 1;
           while (j < Tc){
                 //Définir un point de croisement aléatoirement
                 pointc = one_to_n();
                 //Selectionner les deux meilleurs individus (ceux en tête de liste car éléments triés)
                 croisement(P1 -> elt, P2 -> elt, pointc, &s1, &s2);
                 //affiche_noeud(s1);affiche_noeud(s2);
                 TempFils[nTF] = s1; nTF++; TempFils[nTF] = s2; nTF++;
                 //préparer les prochains parents à croiser
                 Pred = P2;
                 P1 = P2 -> svt; P2 = P1 -> svt;

                 j++;
           }
           k = 0; Tm = rand() % (taille_pop25-1) + 1;
           while (k < Tm){
                 //mutation du prochain individu de la population
                 mutation(P1 -> elt);
                 sm = P1 -> elt;
                 //supprimer l'élément et le rajouter de nouveau pour qu'il soit à la bonne place
                 supprime(Pred, P1);
                 ajout(&Pop, &Q_Pop, sm);
                 //préparer la prochaine mutation
                 Pred = Pred -> svt; P1 = Pred -> svt;

                 k++;
           }

           //Ajouter les fils issus des croisements effectués
           for (l=0; l<nTF; l++){
               ajout(&Pop, &Q_Pop, TempFils[l]);
           }

           //Remplacement : suppression des solutions de basse qualité soit les éléments à patir de la position taille_pop +1
           remplacement(Pop, &Q_Pop, taille_pop);

           i++;
     }

     //Afficher la solution la plus optimale soit la tête de la liste Pop
     affiche_noeud(Pop -> elt);
     float TE = (float)((Pop -> elt) -> f)/n;
     printf(" Le taux d'erreurs est égal à : %f \n", TE);
     fin = clock();//ancre : fin du traitement

     delta = (float) (fin-deb)/CLOCKS_PER_SEC;
     printf(" \n Le temps d'exécution: %f \n",delta);

     getch();
}

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

//d�claration de la liste Pop : contient les noeuds g�n�r�s � d�velopper tri�s dans l'ordre croissant selon la valeur de f(noeud)
typedef  struct no *Liste;
typedef  struct no{
         Pnoeud elt;
         Liste svt;
}noeud1;

//cr�er un �l�ment de la liste Pop
Liste creer_elt(){
    Liste L= (Liste)malloc(sizeof(noeud1));
    if ( !L){
        printf("erreur d'allocation\n");
        exit(-1);
    }
    return(L);
}

//Prc�dure d'ajout d'un noeud � la position ad�quate selon la valeur de f(noeud) � la liste Pop
void ajout(Liste *Pop, Liste  *Q_Pop, Pnoeud E){
    Liste p, q, nouv = creer_elt();//p : pointeur sur l'�l�ment pr�c�dant l'�l�ment courant, q : poiteur sur l'�l�ment courant.
    int Fe, Fq;//Fe : la valeur de la fonction f pour le noeud *E, Fq : la valeur de la fonction f pour le noeud *q.
    //d�but traitement
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
              if (q == *Pop)//si l'insertion se fait � la premi�re position, il faut mettre � jour Pop
                 *Pop = nouv;
              else
                 p -> svt = nouv;
        }
        else {// si la condition de sortie de la boucle est q == *Q_Pop
              if (Fe < Fq){// si la position ad�quate est celle avant le dernier �l�ment
                 nouv -> svt = q;
                 if (*Pop != *Q_Pop){//si la liste ne contient qu'un seul �l�ment
                     p -> svt = nouv;
                 }
                 else{
                     *Pop = nouv;
                 }
              }
              else {//  si la position ad�quate est celle apr�s le dernier �l�ment
                 q -> svt = nouv;
                 nouv -> svt = NULL;
                 *Q_Pop = nouv; //MAJ du dernier �l�ment (Q_Pop) de la liste Pop
              }
        }
    }
    else {//si liste Pop est vide
        nouv -> svt = NULL;
        *Pop = nouv;
        *Q_Pop = *Pop;
    }
}

//Proc�dure suppression d'un �l�ment de la liste
void supprime(Liste Pred, Liste P){
     Pred -> svt = P -> svt;
     P -> svt = NULL;
     free(P);
}

//Proc�dure Lib�rer espace
void libere(Pnoeud p){
     //lib�rer l'espace m�moire occup� par l'individu
     free(p -> t);
     free(p);
}

//Proc�dure remplacement : supprime les individus en queue de liste � partir de la position taille_pop (les individus de basse qualit�)
void remplacement(Liste Pop, Liste* Q_Pop, int taille_pop){
     int i = 1;
     Liste temp;
     //pr�parer la proc�dure de remplacement
     //en positionnant la t�te sur le premier individu � supprimer
     //soit � la position taille_pop
     while (i < taille_pop){
           Pop = Pop -> svt;
           i++;
     }
     //Mettre � jour Q_Pop et mettre null dans le svt du futur dernier �l�ment
     temp = Pop -> svt;
     *Q_Pop = Pop;
     Pop -> svt = NULL;
     Pop = temp;
     //supprimer tant qu'il reste des �l�ments
     while (Pop != NULL){
          temp = Pop;
          Pop = Pop -> svt;
          libere(temp -> elt);
          free(temp);
     }

}

//Proc�dure : retourne une valeur comprise dans l'intervalle [0-n[
int zero_to_n (){
    int number;
    number = rand() % n + 0;
    return number;
}

//Proc�dure : retourne une valeur comprise dans l'intervalle [1-n[
int one_to_n(){
    int number;
    number = rand() % (n-1) + 1;
    return number;
}

//Proc�dure g�n�rant une solution al�atoire
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
              }
              i++;
        }
    }
    return nb_reine_menace;
}

//Proc�dure de croisement
void croisement(Pnoeud s1, Pnoeud s2, int pc, Pnoeud *s12, Pnoeud *s21){
     int i;
     //Allocation m�moire pour les noeuds fils
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

//Proc�dure de mutation
void mutation(Pnoeud s){
     int nb_modif =(int) ((zero_to_n())/2)+1, i, indice_modif; //nb_modif : nombre de cases � modifier (jamais �gal � 0
                                                               //         et ne d�passe pas la moiti� du nombre de case du tableau)
                                                               //indice_modif :  indice de la case � modifier
     for (i=0; i<nb_modif; i++){
         indice_modif = zero_to_n();
         *(s -> t + indice_modif) = zero_to_n();
     }
     s -> f = f(s -> t);//mettre � jour la valeur de la fonction objective

}

//cr�ation de la population initiale
void initialisation(Liste* Pop, Liste* Q_Pop, int taille_pop){
     int i; Pnoeud E;
     //cr�er une solution : premier individu de la population
     (*Pop) = creer_elt();
     (*Pop) -> elt = solution();
     (*Pop) -> svt = NULL;

     //initialiser la queue de liste par le valeur de la t�te Pop
     (*Q_Pop) = (*Pop);
     //printf("solution 0 dans liste Pop \n");
     i = 1; //initialiser � 1 car une solution est d�j� g�n�r�e
     while (i < taille_pop){
         E = solution();
         ajout(Pop, Q_Pop, E);
         //printf("solution %d dans liste Pop \n",i);
         i++;
     }
}

//m�thode affichage d'un noeud
void affiche_noeud(Pnoeud p){
     // p : pointeur sur le noeud (solution) courant
     int i;
     printf("  La solution la plus optimale trouv�e :\n");
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

     Liste Pop, Q_Pop, P1, P2, Pred;//Pop : est la t�te de la liste contenant la population
                              //Q_Pop : est la queue de la liste
                              //P1 et P2 sont les pointeurs sur les parents du prochain croisement
                              //Pred : Sauvegarde le pr�c�dent pour la gestion de la mutation

     Pnoeud TempFils[10000];
     int nTF = 0;

     Pnoeud s1, s2, sm; //fils issus d'un croisement / mutation

     srand(clock());//initialis� la racine du rand

     clock_t deb,fin; //Variables ancres pour d�finir le temps d'ex�cution
     float delta;

     printf("Entrez la taille de votre �chiquier (n) : \n");
     scanf("%d",&n);

     int taille_pop = 2*n;//taille de la population initiale
     //Sachant qu'un croisement requiert de deux individus donc son taux sera calcul� � partir de 50% de la population
     int taille_pop75 = (int)(taille_pop*75/100)/2;
     int taille_pop25 = (int)taille_pop*25/100;

     int nb_iter = 100*n;//nombre d'it�ration (de g�n�ration)

     deb = clock();//Ancre : d�but du traitement

     initialisation(&Pop, &Q_Pop, taille_pop);

     i = 0;
     while ((i < nb_iter) && (Pop -> elt -> f != 0)){
           //r�initialiser les param�tres d'ex�cution
           P1 = Pop; P2 = Pop -> svt; nTF = 0;
           //Croisement
           j = 0; Tc = rand() % (taille_pop75-1) + 1;
           while (j < Tc){
                 //D�finir un point de croisement al�atoirement
                 pointc = one_to_n();
                 //Selectionner les deux meilleurs individus (ceux en t�te de liste car �l�ments tri�s)
                 croisement(P1 -> elt, P2 -> elt, pointc, &s1, &s2);
                 //affiche_noeud(s1);affiche_noeud(s2);
                 TempFils[nTF] = s1; nTF++; TempFils[nTF] = s2; nTF++;
                 //pr�parer les prochains parents � croiser
                 Pred = P2;
                 P1 = P2 -> svt; P2 = P1 -> svt;

                 j++;
           }
           k = 0; Tm = rand() % (taille_pop25-1) + 1;
           while (k < Tm){
                 //mutation du prochain individu de la population
                 mutation(P1 -> elt);
                 sm = P1 -> elt;
                 //supprimer l'�l�ment et le rajouter de nouveau pour qu'il soit � la bonne place
                 supprime(Pred, P1);
                 ajout(&Pop, &Q_Pop, sm);
                 //pr�parer la prochaine mutation
                 Pred = Pred -> svt; P1 = Pred -> svt;

                 k++;
           }

           //Ajouter les fils issus des croisements effectu�s
           for (l=0; l<nTF; l++){
               ajout(&Pop, &Q_Pop, TempFils[l]);
           }

           //Remplacement : suppression des solutions de basse qualit� soit les �l�ments � patir de la position taille_pop +1
           remplacement(Pop, &Q_Pop, taille_pop);

           i++;
     }

     //Afficher la solution la plus optimale soit la t�te de la liste Pop
     affiche_noeud(Pop -> elt);
     float TE = (float)((Pop -> elt) -> f)/n;
     printf(" Le taux d'erreurs est �gal � : %f \n", TE);
     fin = clock();//ancre : fin du traitement

     delta = (float) (fin-deb)/CLOCKS_PER_SEC;
     printf(" \n Le temps d'ex�cution: %f \n",delta);

     getch();
}

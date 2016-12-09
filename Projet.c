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
        int niveau;//le niveau du noeud dans l'arborescence 
        Pnoeud *Tfils;// le vecteur de pointeurs sur les fils 
        Pnoeud pere;// le pointeur sur le noeud prec�dent 
}noeud;

//d�claration de la liste Open
typedef  struct no *Liste;  
typedef  struct no{ 
         Pnoeud elt;
         Liste svt;
}noeud1;
        
//cr�er un �l�ment de la liste Open
Liste creer_elt(){  
    Liste L= (Liste)malloc(sizeof(noeud1));
    if ( !L){ 
        printf("erreur d'allocation\n"); 
        exit(-1);
    } 
    return(L);
}

//Proc�dure de suppression de la t�te de liste 
void sup_tete(Liste *tete){    
     Liste temp = *tete;  
     *tete = (*tete) -> svt;  
     free(temp); 
}

//Prc�dure ajouter � la fin de la liste Open
void ajout_queue(Liste  *der, Pnoeud E){ 
    Liste nouv = creer_elt();
    nouv -> elt = E;
    nouv -> svt = (*der) -> svt;
    (*der) -> svt = nouv;
    (*der) = nouv;
}


//Libre
int Libre (int *t, int niv, int cl){// t est le vecteur du noeud courant 
    // niv est le niveau du noeud courant (dans l'arborescence)
    // c le N� de colonne dans laquelle on souhaite placer une reine
    int libre=1, i=0, c; //libre variable bool�enne (0 = menac�e, 1 = libre)| i : indice 
    // pour parcourir le vecteur t| c est un temporaire pour la colonne courante
    while (i < niv){
          c=t[i];
          if((cl==c) || (fabs(niv-i) == fabs(cl-c)))// ce test v�rifie les conditions suivantes :
                     //- cl == c : s'il existe une autre reine sur la m�me colonne
                     //- fabs(niv-i) == fabs(cl-c) : s'il existe une autre reine sur les digonale montante/descendante. fabs : valeur absolue
                     // RMQ : la condition : une seule reine par ligne, est trivial, car une seule reine est plac�e par ligne.
                     
               libre=0;
          else 
               i++; 
    }
    return libre;
}
//placer_noeud 
Pnoeud Placer (Pnoeud p, int numf){//p : le pointeur sur le noeud g�n�rateur (le noeud p�re), numf : le num�ro du fils [0 .. n-1]
       int i;//variable temporaire
       //�tape 1 : cr�er le noeud fils
       p -> Tfils[numf] = (Pnoeud)malloc(sizeof(noeud));
       //�tape 2 : remplir le noeud fils avec les donn�es correspondantes 
       Pnoeud fils = p -> Tfils[numf];//pour faciliter la manipulation
       fils -> niveau = (p -> niveau) + 1;// mettre la valeur du niveau (niveau = niveau + 1)
       fils -> t = (int*)malloc((fils -> niveau)*sizeof(int));//on alloue la taille n�cessaire selon le niveau (ex : niveau 2 -> 2 reines plac�es -> 2 cases
       for (i = 0 ; i < p -> niveau ; i++){
           fils -> t[i] = p -> t[i]; //h�ritage des valeurs du tableau t du p�re
       }
       fils -> t[p -> niveau] = numf; //le num�ro du fils : numf, correspond � la colonne o� on compte placer la nouvelle reine
       //�tape 3 : faire le chainage avec le p�re 
       
                 
}

main (){   
     
     int i, j, k, l, n_niv, nb, cpt;//variables temporaires
     
     Printf("Entrez la taille de votre �chiquier (n) :\n");
     scanf("%d",&n);
     
     //traitement  
     Pnoeud fils, p = (Pnoeud)malloc(sizeof(noeud)); //cr�ation du noeud initial
     p -> t = NULL;
     p -> niveau = 0;
     p -> Tfils = malloc(n*sizeof(Pnoeud));
     p -> pere = NULL;
     
     //cr�ation de la liste Open
     Liste Q_Open, Open = creer_elt();
     Open -> elt = p;
     Open -> svt = NULL;
     Q_Open = Open;//initialis� la queue de liste � la t�te de liste Open
     
     //D�veloppement de l'arbre de recherche, m�thode : Largeur D'abord
     cpt = 1;
     for (n_niv = 0 ; n_niv <= n ; n_niv++){ /*Pour chaque niveau*/
         nb = cpt;//initialis� aux nombre de noeuds du niveau qu'on va traiter 
         cpt = 0;
         for (l = 0 ; l < nb ; l++){ /*Pour chaque noeud du niveau*/
             p = Open -> elt;// D�finir le p (le noeud de l'arborescence) courant 
             sup_tete(&Open);
             for (k = 0 ; k < n ; k++){
                 if (Libre (p -> t, p -> niveau, k)){ 
                    cpt++;// Compte le nombre de noeud cr�er dans un niveau
                    fils = Placer(p, k);//placer le noeud (le cr�er)
                    ajout_queue(&Q_Open, fils);//rajouter le fils cr�e � la liste open
                 }
             }
         }
     }
     getch();
}
       

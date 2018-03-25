#ifndef __TOMO_H__
#define __TOMO_H__
#include <stdio.h>


//La structure de donnees 
typedef struct inst{
	int n,m;
	int ** matrice;				/*matrice[i][j] = 0 -> case libre
												  1 -> case blanche
												  2 -> case noir */
	int * taille_seq_l;
	int * taille_seq_c;
	int ** seq_l;
	int ** seq_c;
	int *** tt_ligne;			/*tt_ligne[j][l] = -1 -> non visite
												    0 -> faux
												    1 -> vrai */
												    
	int *** tt_colonne;			/*idem que tt_ligne*/
}Instance;

//Allocation dynamique  et initialisation de la structure a partir des donnees dans le fichier f
Instance * init_instance(FILE * f);


//Affichage uniquement de la matrice de l'instance sur le terminal
void afficher_instance(Instance * ins);


//remettre toutes les cases de la matrice de l'instance à libre (0) pour un autre coloriage
void remettre_instance_libre(Instance * ins);


/*allocation dynamique et initialisation de la matrice TT 
pour le vecteur ligne d'indice (ligne-1)*/
void allouer_matrice_tt_ligne(int ligne, Instance * ins);


/*allocation dynamique et initialisation de la matrice TT 
pour le vecteur colonne d'indice (colonne - 1)*/
void allouer_matrice_tt_colonne(int colonne, Instance * ins);


/*remettre les cases de la matrice TT du vecteur ligne
d'indice (ligne -1) a non visite (-1)*/ 
void remettre_tt_ligne_non_visite(int ligne, Instance * ins);


/*remettre les cases de la matrice TT du vecteur colonne
d'indice (colonne -1) a non visite (-1)*/ 
void remettre_tt_colonne_non_visite(int colonne, Instance * ins);


//teste si la sequence de coloration est respecté sur la ligne de la matrice d'indice i
//renvoie 1 si vrai, 0 si faux
int compare_seq_ligne(int i, Instance * ins);


//teste si la sequence de coloration est respecté sur la colonne de la matrice d'indice j
//renvoie 1 si vrai, 0 si faux
int compare_seq_col(int j, Instance * ins);


//la fonction Enumeration_Rec(k,c) defini dans la partie theorique
//renvoie 1 si vrai, 0 si faux
int enumeration_rec(int k, int c, Instance * ins);


/*teste si aucune case entre j1 et j2, de la ligne de la matrice d'indice indice_ligne,
n'est egale à val*/
//renvoie 1 si vrai, 0 si faux
int testSiAucun_ligne(int indice_ligne, Instance * ins, int j1, int j2, int val);


/*la fonction TestVecteur_Rec(V, j, l, TT) defini dans la partie theorique,
appliquée à une ligne de la matrice d'indice indice_ligne*/
//renvoie 1 si vrai, 0 si faux
int testVecteurLigne_rec(int indice_ligne, Instance * ins, int j, int l, int ** tt);


/*teste si aucune case entre j1 et j2, de la colonne de la matrice d'indice indice_colonne,
n'est egale à val*/
//renvoie 1 si vrai, 0 si faux
int testSiAucun_colonne(int indice_colonne, Instance * ins, int j1, int j2, int val);


/*la fonction TestVecteur_Rec(V, j, l, TT) defini dans la partie theorique,
appliquée à une colonne de la matrice d'indice indice_colonne*/
//renvoie 1 si vrai, 0 si faux
int testVecteurColonne_rec(int indice_colonne, Instance * ins, int j, int l, int ** tt);


//la fonction PropagLigne defini dans la partie pratique, applique a la ligne d'indice i de l'instance
//renvoie 1 si vrai, 0 si faux
int propag_ligne(int i, int * marque, int * nb, Instance * ins);


//la fonction PropagColonne defini dans la partie pratique, applique a la colonne d'indice j de l'instance
//renvoie 1 si vrai, 0 si faux
int propag_colonne(int j, int * marque, int * nb, Instance * ins);


//la fonction Propagation defini dans la partie pratique
int propagation(Instance * ins);


//calcule le pourcentage de case colorie (=1 ou =2) dans la matrice de l'instance
float pourcentage_coloriage(Instance * ins);


//liberation dynamique de la structure
void liberer_instances(Instance ** ins);

#endif

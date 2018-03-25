#include "tomo.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//Allocation dynamique  et initialisation de la structure a partir des donnees dans le fichier f
Instance * init_instance(FILE * f){
	
	Instance * ins = (Instance *)malloc(sizeof(Instance));
	
	int n,m;
	
	/*lecture de la premiere ligne du fichier contenant n et m*/
	fscanf(f, " %d %d", &n, &m);
	
	ins->m = m;
	ins->n = n;
	
	//initialisation de la matrice libre (= 0) au debut
	ins -> matrice = (int **)malloc(n*sizeof(int *));
	int i, j;
	for(i=0; i<n; i++){
		ins->matrice[i] = (int *)malloc(m*sizeof(int));
		for(j=0; j<m; j++){
			ins->matrice[i][j] = 0;
		}
	}
	
	ins->taille_seq_l = (int *)malloc(n*sizeof(int));
	ins->seq_l = (int **)malloc(n * sizeof(int *));
	
	//lecture dans le fichier des sequences de lignes L
	int nb;
	for(i=0; i<n; i++){
		
		/*lecture de la taille de la sequence Li*/
		fscanf(f, " %d", &nb);
		ins->taille_seq_l[i] = nb;
		
		ins->seq_l[i] = (int *)malloc(nb * sizeof(int));
		
		/*lecture de Li*/
		for(j=0; j<nb; j++){
			fscanf(f, " %d", &(ins->seq_l[i][j]));
		}
	}
	
	ins->taille_seq_c = (int *)malloc(m * sizeof(int));
	ins->seq_c = (int **)malloc(m * sizeof(int *));
	
	//lecture des sequences de colonnes C
	for(i=0; i<m; i++){
		
		/*lecture de la taille de la sequence Ci*/
		fscanf(f, " %d", &nb);
		ins->taille_seq_c[i] = nb;
		
		ins->seq_c[i] = (int *)malloc(nb * sizeof(int));
		
		/*lecture de Ci*/
		for(j=0; j<nb; j++){
			fscanf(f, " %d", &(ins->seq_c[i][j]));
		}
	}
	
	/*allocation des tt_ligne et tt_colonne SANS allouer les matrices TT des vecteurs lignes et colonnes*/
	/* on allouera les TT a chaque fois qu'on le demande pour une ligne ou une colonne precise de la matrice
	 * de l'instance, a l'aide des fonctions allouer_matrice_tt_ligne et allouer_matrice_tt_colonne*/
	ins->tt_ligne = (int ***)malloc(n * sizeof(int **));
	ins->tt_colonne = (int ***)malloc(m * sizeof(int **));
	
	for(i=0; i<n; i++){
		ins->tt_ligne[i] = NULL;
	}
	
	for(i=0; i<m; i++){
		ins->tt_colonne[i] = NULL;
	}
		
	
	return ins;
}



//Affichage uniquement de la matrice de l'instance sur le terminal
void afficher_instance(Instance * ins){
	
	int i,j;
	printf("========== MATRICE ==========\n");
	for(i = 0; i< (ins->n); i++){
		for(j=0; j<(ins->m); j++){
			printf("%d | ", ins->matrice[i][j]);
		}
		printf("\n");
	}
	printf("========== FIN MATRICE ==========\n\n");
}



//remettre toutes les cases de la matrice de l'instance à libre (0) pour un autre coloriage
void remettre_instance_libre(Instance * ins){
	int i,j;
	for(i=0; i<(ins->n); i++){
		for(j=0; j<(ins->m);j++){
			ins->matrice[i][j] = 0;
		}
	}
}



/*allocation dynamique et initialisation de la matrice TT 
pour le vecteur ligne d'indice ligne -1*/
void allouer_matrice_tt_ligne(int ligne, Instance * ins){
	
	if(ins->tt_ligne[ligne-1] != NULL){ return;}	/*si la matrice est deja alloue, on fait rien*/
	
	int j, k;
	ins->tt_ligne[ligne-1] = (int **)malloc(ins->m * sizeof(int *));
	for(j=0; j< ins->m; j++){
		ins->tt_ligne[ligne-1][j] = (int *)malloc(ins->taille_seq_l[ligne-1] * sizeof(int));
		for(k=0; k< ins->taille_seq_l[ligne-1]; k++){
			ins->tt_ligne[ligne-1][j][k] = -1;		/*les cases de TT sont initialise a "non visite" (= -1)*/
		}
	}
}



/*allocation dynamique et initialisation de la matrice TT 
pour le vecteur colonne d'indice (colonne - 1)*/
void allouer_matrice_tt_colonne(int colonne, Instance * ins){
	
	if(ins->tt_colonne[colonne-1] != NULL){ return;}	/*si la matrice est deja alloue, on fait rien*/
	
	int j, k;
	ins->tt_colonne[colonne-1] = (int **)malloc(ins->n * sizeof(int *));
	for(j=0; j< ins->n; j++){
		ins->tt_colonne[colonne-1][j] = (int *)malloc(ins->taille_seq_c[colonne-1] * sizeof(int));
		for(k=0; k< ins->taille_seq_c[colonne-1]; k++){
			ins->tt_colonne[colonne-1][j][k] = -1;	/*les cases de TT sont initialise a "non visite" (= -1)*/
		}
	}
}



/*remettre les cases de la matrice TT du vecteur ligne
d'indice (ligne -1) a non visite (-1)*/ 
void remettre_tt_ligne_non_visite(int ligne, Instance * ins){
	if(ins->tt_ligne[ligne-1] == NULL){ return;}
	int i,j;
	for(i =0; i< (ins->m); i++){
		for(j=0; j< ins->taille_seq_l[ligne-1]; j++){
			ins->tt_ligne[ligne-1][i][j] = -1;
		}
	}
}



/*remettre les cases de la matrice TT du vecteur colonne
d'indice (colonne -1) a non visite (-1)*/ 
void remettre_tt_colonne_non_visite(int colonne, Instance * ins){
	if(ins->tt_colonne[colonne-1] == NULL){ return;}
	int i,j;
	for(i =0; i< (ins->n); i++){
		for(j=0; j< ins->taille_seq_c[colonne-1]; j++){
			ins->tt_colonne[colonne-1][i][j] = -1;
		}
	}
}



//teste si la sequence de coloration est respecté sur la ligne de la matrice d'indice i
//renvoie 1 si vrai, 0 si faux
int compare_seq_ligne(int i, Instance * ins){
	
	if(ins->m == 1){ return 1;} /*si l'instance est un vecteur colonne, on a rien a verifie*/
	
	int * ligne = ins->seq_l[i];
	int indice_matrice = 0;
	
	/*debut: tant qu'il y a des cases blanches, on avance*/
	while((indice_matrice < ins->m)&&(ins->matrice[i][indice_matrice] == 1)){ indice_matrice++;}
	
	/*si on est arrive a la fin de la ligne avec que des cases blanches, on teste s'il
	 * y a des sequences à vérifier*/
	if(indice_matrice == ins->m){
		if(ins->taille_seq_l[i] != 0){ return 0; }
		else{ return 1; }
	}
	
	/*si on a atteint une case noir alors qu'il y a pas de séquences à respecter (l =0), on retourne faux*/
	if(ins->taille_seq_l[i] == 0){ return 0;}
	
	int cpt_seq = 0;
	int cpt_case;
	
	/*on commence la verification des blocs de la sequence*/ 
	while((indice_matrice < ins-> m) && (cpt_seq < ins->taille_seq_l[i])){
		
		cpt_case = ligne[cpt_seq];
		while((indice_matrice < ins-> m)&&(cpt_case>0)){
			if(ins->matrice[i][indice_matrice] == 1){ return 0;}
			else{
				cpt_case--;
				indice_matrice++;
			}
		}
		
		/*si on a atteint la fin de la ligne alors que la sequence n'est pas fini, on renvoie faux*/
		if(cpt_case >0){
			return 0;
		}
		
		/*si on a atteint la fin de la ligne*/
		if(indice_matrice == ins->m){
			
			/*et si il reste encore des blocs à vérifier, on renvoie faux*/
			if(cpt_seq < ins->taille_seq_l[i] - 1){ return 0; }
			
			/*sinon, on renvoie vrai*/
			else{ return 1; }
		}
		
		/*si il y a une case noir tout de suite après le premier bloc vérifié, on revoie faux*/
		if(ins->matrice[i][indice_matrice] == 2){ return 0;}

		/*sinon, on resaute tous les cases blanches*/
		while((indice_matrice < ins->m)&&(ins->matrice[i][indice_matrice] == 1)){ indice_matrice++;}

		cpt_seq++;
		
	}
	
	/*si on a atteint la fin de la ligne, mais pas la fin de la séquence*/
	if(cpt_seq < ins->taille_seq_l[i]){ return 0;}
	
	/*on teste s'il reste encore des cases noirs sur la ligne alors que la séquence est terminée*/
	while(indice_matrice < ins->m){
		if(ins->matrice[i][indice_matrice] == 2){ return 0; }
		else{ indice_matrice++; }
	}
	
	return 1;
}



//teste si la sequence de coloration est respecté sur la colonne de la matrice d'indice j
//renvoie 1 si vrai, 0 si faux
int compare_seq_col(int j, Instance * ins){
	
	if(ins->n == 1){return 1;}		/*si l'instance est un vecteur ligne, on a rien a verifie*/
	
	int * colonne = ins->seq_c[j];
	int indice_matrice = 0;

	/*debut: tant qu'il y a des cases blanches, on avance*/
	while((indice_matrice < ins->n)&&(ins->matrice[indice_matrice][j] == 1)){ indice_matrice++;}
	
	
	/*si on est arrive a la fin de la colonne avec que des cases blanches, on teste s'il
	 * y a des sequences à vérifier*/
	if(indice_matrice == ins->n){
		if(ins->taille_seq_c[j] != 0){ return 0; }
		else{ return 1; }
	}
	
	/*si on a atteint une case noir alors qu'il y a pas de séquences à respecter (l =0), on retourne faux*/
	if(ins->taille_seq_c[j] == 0){ return 0;}
	
	int cpt_seq = 0;
	int cpt_case;

	/*on commence la verification des blocs de la sequence*/ 
	while((indice_matrice < ins-> n) && (cpt_seq < ins->taille_seq_c[j])){
		
		cpt_case = colonne[cpt_seq];
		while((indice_matrice < ins-> n)&&(cpt_case>0)){
			if(ins->matrice[indice_matrice][j] == 1){ return 0;}
			else{
				cpt_case--;
				indice_matrice++;
			}
		}
		
		/*si on a atteint la fin de la colonne alors que la sequence n'est pas fini, on renvoie faux*/
		if(cpt_case >0){
			return 0;
		}
		
		/*si on a atteint la fin de la colonne*/
		if(indice_matrice == ins->n){
			
			/*et si il reste encore des blocs à vérifier, on renvoie faux*/
			if(cpt_seq < ins->taille_seq_c[j] - 1){ return 0; }
			
			/*sinon, on renvoie vrai*/
			else{ return 1; }
		}
		
		
		/*si il y a une case noir tout de suite après le premier bloc vérifié, on revoie faux*/
		if(ins->matrice[indice_matrice][j] == 2){ return 0;}


		/*sinon, on resaute tous les cases blanches*/
		while((indice_matrice < (ins->n))&&(ins->matrice[indice_matrice][j] == 1)){ indice_matrice++;}
		cpt_seq++;
		
	}
	
	/*si on a atteint la fin de la colonne, mais pas la fin de la séquence*/
	if(cpt_seq < ins->taille_seq_c[j]){ return 0;}
	
	/*on teste s'il reste encore des cases noirs sur la colonne alors que la séquence est terminée*/
	while(indice_matrice < ins->n){
		if(ins->matrice[indice_matrice][j] == 2){ return 0; }
		else{ indice_matrice++; }
	}
	
	return 1;
}



//la fonction Enumeration_Rec(k,c) defini dans la partie theorique
//renvoie 1 si vrai, 0 si faux
int enumeration_rec(int k, int c, Instance * ins){
	int ok;
	int raz;
	int i,j;
	i = floor(k*1.0 / ins->m);
	j = k % ins->m;
	
	if(ins->matrice[i][j] == 0){
		ins->matrice[i][j] = c;
		raz = 1;
	}
	else{
		if(ins->matrice[i][j] != c){ return 0;}
		else{ raz = 0; }
	}
	
	ok = 1;
	
	if(i == (ins->n)-1) { ok = compare_seq_col(j, ins);}
	
	if(ok && (j == (ins->m)-1)){ ok = compare_seq_ligne(i, ins); }
	if(ok){
		if((i == (ins->n)-1) && (j == (ins->m)-1)){ return 1; }
		ok = enumeration_rec(k+1, 1, ins) || enumeration_rec(k+1, 2, ins);
	}
	
	if((!ok) && (raz)){ ins->matrice[i][j] = 0; }
	return ok;
}



/*teste si aucune case entre j1 et j2, de la ligne de la matrice d'indice indice_ligne,
n'est egale à val*/
//renvoie 1 si vrai, 0 si faux
int testSiAucun_ligne(int indice_ligne, Instance * ins, int j1, int j2, int val){
	int j;
	for(j=j1; j<=j2; j++){
		if(ins->matrice[indice_ligne][j] == val){
			return 0;
		}
	}
	return 1;
}



/*la fonction TestVecteur_Rec(V, j, l, TT) defini dans la partie theorique,
appliquée à une ligne de la matrice d'indice indice_ligne*/
//renvoie 1 si vrai, 0 si faux
int testVecteurLigne_rec(int indice_ligne, Instance * ins, int j, int l, int ** tt){
	int c1;
	int c2;
	
	if(ins->m == 1){ return 1;}		/*si l'instance est un vecteur colonne, on a rien a verifié*/
	
	if(l==0){
		return testSiAucun_ligne(indice_ligne, ins, 0, j, 2);
	}
	
	
	if((l==1)&&(j==(ins->seq_l[indice_ligne][l-1])-1)){
		return testSiAucun_ligne(indice_ligne, ins, 0, j, 1);
	}
	
	
	if(j <= (ins->seq_l[indice_ligne][l-1])-1) { return 0; }
	
	if(tt[j][l-1] != -1) { return tt[j][l-1]; }
	
	if(ins->matrice[indice_ligne][j] == 2){
		c1 = 0;
	}
	else{
		c1 = testVecteurLigne_rec(indice_ligne, ins, j-1, l, tt);
	}
	
	if(!(testSiAucun_ligne(indice_ligne, ins, j-((ins->seq_l[indice_ligne][l-1])-1), j, 1))){
		c2 = 0;
	}
	else{
		if(ins->matrice[indice_ligne][j - (ins->seq_l[indice_ligne][l-1])] == 2){
			c2 = 0;
		}
		else{
			c2 = testVecteurLigne_rec(indice_ligne, ins, j-(ins->seq_l[indice_ligne][l-1])-1, l-1, tt);
		}
	}
	tt[j][l-1] = c1 || c2;
	return tt[j][l-1];
}



/*teste si aucune case entre j1 et j2, de la colonne de la matrice d'indice indice_colonne,
n'est egale à val*/
//renvoie 1 si vrai, 0 si faux
int testSiAucun_colonne(int indice_colonne, Instance * ins, int j1, int j2, int val){
	int i;
	for(i=j1; i<=j2; i++){
		if(ins->matrice[i][indice_colonne] == val){
			return 0;
		}
	}
	return 1;
}



/*la fonction TestVecteur_Rec(V, j, l, TT) defini dans la partie theorique,
appliquée à une colonne de la matrice d'indice indice_colonne*/
//renvoie 1 si vrai, 0 si faux
int testVecteurColonne_rec(int indice_colonne, Instance * ins, int j, int l, int ** tt){
	int c1;
	int c2;
	
	if(ins->n == 1){ return 1;}		/*si l'instance est un vecteur ligne on n'a rien a verifie*/
	
	if(l==0){
		return testSiAucun_colonne(indice_colonne, ins, 0, j, 2);
	}
	
	if((l==1)&&(j==(ins->seq_c[indice_colonne][l-1])-1)){
		return testSiAucun_colonne(indice_colonne, ins, 0, j, 1);
	}
	
	if(j <= (ins->seq_c[indice_colonne][l-1])-1) { return 0; }
	

	if(tt[j][l-1] != -1) { return tt[j][l-1]; }
	
	
	if(ins->matrice[j][indice_colonne] == 2){
		c1 = 0;
	}
	else{
		c1 = testVecteurColonne_rec(indice_colonne, ins, j-1, l, tt);
	}
	
	if(!(testSiAucun_colonne(indice_colonne, ins, j-((ins->seq_c[indice_colonne][l-1])-1), j, 1))){
		c2 = 0;
	}
	else{
		if(ins->matrice[j - (ins->seq_c[indice_colonne][l-1])][indice_colonne] == 2){
			c2 = 0;
		}
		else{
			c2 = testVecteurColonne_rec(indice_colonne, ins, j-(ins->seq_c[indice_colonne][l-1])-1, l-1, tt);
		}
	}
	tt[j][l-1] = c1 || c2;
	return tt[j][l-1];
}



//la fonction PropagLigne defini dans la partie pratique, applique a la ligne d'indice i de l'instance
//renvoie 1 si vrai, 0 si faux
int propag_ligne(int i, int * marque, int * nb, Instance * ins){
	int j;
	int c1, c2;
	int cptcolor = 0;
	*nb=0;
	
	for(j=0; j< (ins->m); j++){
		if(ins->matrice[i][j] == 0){
			
			ins->matrice[i][j] = 1;
			/*on alloue la matrice TT du vecteur ligne d'indice i si elle n'a pas encore été allouée*/
			allouer_matrice_tt_ligne(i+1, ins);
			c1 = testVecteurLigne_rec(i, ins, (ins->m)-1, (ins->taille_seq_l)[i], ins->tt_ligne[i]);
			/*on remet les cases de TT a "non visite" afin de faire le deuxieme appel a la fonction
			 * testVecteurLigne_rec*/
			remettre_tt_ligne_non_visite(i+1, ins);
			
			
			ins->matrice[i][j] = 2;
			c2 = testVecteurLigne_rec(i, ins, (ins->m)-1, (ins->taille_seq_l)[i], ins->tt_ligne[i]);
			remettre_tt_ligne_non_visite(i+1, ins);
			
			ins->matrice[i][j] = 0;
			
			if((!c1)&&(!c2)){ return 0; }
			
			if(c1 && (!c2)){
				ins->matrice[i][j] = 1;
				cptcolor++;
				if(! marque[j]){
					marque[j] = 1;
					(*nb)++;
				}
			}
			
			if((!c1) && c2){
				ins->matrice[i][j] = 2;
				cptcolor++;
				if(! marque[j]){
					marque[j] = 1;
					(*nb)++;
				}
			}
			
		}
	}
	return 1;
}



//la fonction PropagColonne defini dans la partie pratique, applique a la colonne d'indice j de l'instance
//renvoie 1 si vrai, 0 si faux
int propag_colonne(int j, int * marque, int * nb, Instance * ins){
	int i;
	int c1, c2;
	int cptcolor = 0;
	*nb=0;
	
	for(i=0; i< (ins->n); i++){
		if(ins->matrice[i][j] == 0){
			
			ins->matrice[i][j] = 1;
			/*on alloue la matrice TT du vecteur colonne d'indice j si elle n'a pas encore été allouée*/
			allouer_matrice_tt_colonne(j+1, ins);
			c1 = testVecteurColonne_rec(j, ins, (ins->n) -1, (ins->taille_seq_c)[j], ins->tt_colonne[j]);
			/*on remet les cases de TT a "non visite" afin de faire le deuxieme appel a la fonction
			 * testVecteurColonne_rec*/
			remettre_tt_colonne_non_visite(j+1, ins);
			
			
			ins->matrice[i][j] = 2;
			c2 = testVecteurColonne_rec(j, ins, (ins->n) -1, (ins->taille_seq_c)[j], ins->tt_colonne[j]);
			remettre_tt_colonne_non_visite(j+1, ins);
			
			ins->matrice[i][j] = 0;
			
			if((!c1)&&(!c2)){ return 0; }
			if(c1 && (!c2)){
				ins->matrice[i][j] = 1;
				cptcolor++;
				if(! marque[i]){
					marque[i] = 1;
					(*nb)++;
				}
			}
			
			if((!c1) && c2){
				ins->matrice[i][j] = 2;
				cptcolor++;
				if(! marque[i]){
					marque[i] = 1;
					(*nb)++;
				}
			}
			
		}
	}
	return 1;
}



//la fonction Propagation defini dans la partie pratique
int propagation(Instance * ins){
	int marqueL[ins->n];
	int marqueC[ins->m];
	int nbmL, nbmC, nb=0;
	int i,j;
	int ok = 1;
	
	for(i=0; i< (ins->n); i++){
		marqueL[i] = 1;		/*marqueL[i] = vrai*/
	}
	
	for(j=0; j< (ins->m); j++){
		marqueC[j] = 1;		/*marqueC[j] = vrai*/
	}
	
	nbmL = ins->n;
	nbmC = ins->m;
	
	while((ok)&&((nbmL != 0)||(nbmC != 0))){
		i = 0;
		while((ok)&&(i<(ins->n))){
			if(marqueL[i]){
				ok = propag_ligne(i, marqueC, &nb, ins);
				nbmC = nbmC + nb;
				marqueL[i] = 0;
				nbmL--;
			}
			i++;
		}
		
		
		j =0;
		while((ok)&&(j < (ins->m))){
			if(marqueC[j]){
				ok = propag_colonne(j, marqueL, &nb, ins);
				nbmL = nbmL + nb;
				marqueC[j] = 0;
				nbmC--;
			}
			j++;
		}
	}
	return ok;
}




//calcule le pourcentage de case colorie (=1 ou =2) dans la matrice de l'instance
float pourcentage_coloriage(Instance * ins){
	int i,j;
	int nb = 0;
	for(i=0; i< ins->n; i++){
		for(j=0; j< ins->m; j++){
			if(ins->matrice[i][j] != 0){
				nb++;
			}
		}
	}

	return ((nb*100.0) / ((ins->n)*(ins->m)*1.0));
}
			


//liberation dynamique de la structure
void liberer_instances(Instance ** ins){
	if(*ins == NULL){ return;}
	
	int i, j;
	
	for(i=0; i< (*ins)->n; i++){
		if((*ins)->tt_ligne[i] != NULL){
			for(j=0; j< (*ins)->m; j++){
					free((*ins)->tt_ligne[i][j]);
			}
			free((*ins)->tt_ligne[i]);
		}
	}
	
	for(i=0; i<(*ins)->m; i++){
		if((*ins)->tt_colonne[i] != NULL){
			for(j=0; j<(*ins)->n; j++){
				free((*ins)->tt_colonne[i][j]);
			}
			free((*ins)->tt_colonne[i]);
		}
	}
	
	
	free((*ins)->tt_ligne);
	free((*ins)->tt_colonne);
	
	free((*ins)->taille_seq_l);
	free((*ins)->taille_seq_c);
	
	
	for(i=0; i< (*ins)->n; i++){
		free((*ins)->seq_l[i]);
		free((*ins)->matrice[i]);
	}
	
	free((*ins)->seq_l);
	free((*ins)->matrice);
	
	for(i=0; i< (*ins)->m; i++){
		free((*ins)->seq_c[i]);
	}
	
	free((*ins)->seq_c);
	
	free(*ins);
	*ins = NULL;
}



	
	
	
		

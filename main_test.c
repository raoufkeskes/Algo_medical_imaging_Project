#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "tomo.h"


clock_t temps_initial;
clock_t temps_final;
double temps_cpu;

void menu(){
	printf("\n\n------MENU------\n");
	printf("0 pour tester la fonction enumeration_rec et retourner le temps d'execution\n");
	printf("1 pour tester la fonction testVecteurLigne_rec et retourner le temps d'execution\n");
	printf("2 pour tester la fonction testVecteurColonne_rec et retourner le temps d'execution\n");
	printf("3 pour tester la fonction propagation et retourner le temps d'execution\n");
	printf("4 pour enchainer propagation et enumeration_rec; et retourner le temps d'execution total\n");
	printf("5 pour sortir\n");
	printf("------FIN MENU------\n\n\n");
}

int main(int argc, char ** argv){
	if(argc != 2){
		printf("Il faut passer en parametre le nom du fichier .tom contenant l'instance a considerer dans le programme\n");
		return 0;
	}
	
	FILE * f = fopen(argv[1], "r");
	if(f==NULL){
		printf("Le fichier %s n'existe pas\n ", argv[1]);
		return 0;
	}
	
	Instance * instance = init_instance(f);
	afficher_instance(instance);
	printf("BIENVENUE DANS VOTRE MENU\n\n\n");
	sleep(1);
	
	int choix;
	int retour;
	char c;
	int res;
	int ligne, colonne;
	
	while(1){
		do{
		
				menu();
			
				printf("Saisissez votre choix : ");
			
				retour = scanf(" %d", &choix);
		
		
				if(!retour){
					printf("\nErreur de saisie! le(s) caractere(s) ne correspond(ent) pas a un entier\n\n");
					/*afin de securiser l'entree d'un entier au clavier, on vide le buffer du flux stdin, au cas ou l'utilisateur
					* saisie autre qu'un entier, et donc la variable retour sera egale a 0, et on reste dans la boucle du menu en
					* continuant a lire le caractere saisie jusqu'a qu'il soit bien un entier*/
					do{
						c = getchar();
					}while(c && (c != '\n')); 
				
				
					choix = -1;
				}
		
		}while((choix<0)||(choix>5));
	

		switch (choix) {
		
			//tester la fonction enumeration_rec
			case 0:
				temps_initial = clock();
				res = enumeration_rec(0, 1, instance) || enumeration_rec(0, 2, instance);
				temps_final = clock();
				temps_cpu = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
				printf("l'appel a retourné : %d; \n temps d'exécution = %f\n", res, temps_cpu);
				afficher_instance(instance);
				remettre_instance_libre(instance);
				break;
				
				
			//tester la fonction testVecteurLigne_rec
			case 1:
				if(instance->n==1){ ligne = 1; }
				else{
					do{
			
						printf("Saisissez la ligne de la matrice sur laquelle tester testVecteurLigne_rec : ");
			
						retour = scanf(" %d", &ligne);
		
		
						if(!retour){
							printf("\nErreur de saisie! le(s) caractere(s) ne correspond(ent) pas a un entier\n\n");
							do{
								c = getchar();
							}while(c && (c != '\n')); 
				
				
							ligne = -1;
						}
		
					}while((ligne<=0)||(ligne > (instance->n)));
				}
				
				allouer_matrice_tt_ligne(ligne, instance);
				temps_initial = clock();
				printf("instance->taille_seq_l[%d] = %d\n", ligne, (instance->taille_seq_l)[ligne-1]);
				res = testVecteurLigne_rec(ligne-1, instance, instance->m -1, (instance->taille_seq_l)[ligne-1], instance->tt_ligne[ligne-1]);
				temps_final = clock();
				temps_cpu = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
				printf("l'appel a retourné : %d; \n temps d'exécution = %f\n", res, temps_cpu);
				break;
				
				
			
			//tester la fonction testVecteurColonne_rec
			case 2:
				if(instance->m==1){ colonne = 1; }
				else{
					do{
			
						printf("Saisissez la colonne de la matrice sur laquelle tester testVecteurColonne_rec : ");
			
						retour = scanf(" %d", &colonne);
		
		
						if(!retour){
							printf("\nErreur de saisie! le(s) caractere(s) ne correspond(ent) pas a un entier\n\n");
							do{
								c = getchar();
							}while(c && (c != '\n')); 
				
				
							colonne = -1;
						}
		
					}while((colonne<=0)||(colonne > (instance->m)));
				}
				
				allouer_matrice_tt_colonne(colonne, instance);
				temps_initial = clock();
				printf("instance->taille_seq_c[%d] = %d\n", colonne, (instance->taille_seq_c)[colonne-1]);
				res = testVecteurColonne_rec(colonne-1, instance, instance->n -1, (instance->taille_seq_c)[colonne-1], instance->tt_colonne[colonne-1]);
				temps_final = clock();
				temps_cpu = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
				printf("l'appel a retourné : %d; \n temps d'exécution = %f\n", res, temps_cpu);
				break;
				
			
			
			//tester la fonction propagation	
			case 3:
				temps_initial = clock();
				res = propagation(instance);
				temps_final = clock();
				temps_cpu = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
				printf("l'appel a retourné : %d; \n temps d'exécution = %f\n", res, temps_cpu);
				printf("Pourcentage des cases colories = %f\n\n", pourcentage_coloriage(instance));
				afficher_instance(instance);
				remettre_instance_libre(instance);
				break;
				
			
			//enchainer propagation et enumeration_rec	
			case 4:
				temps_initial = clock();
				res = propagation(instance);
				res = enumeration_rec(0, 1, instance) || enumeration_rec(0, 2,instance);
				temps_final = clock();
				temps_cpu = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
				printf("l'appel a retourné : %d; \n temps d'exécution = %f\n", res, temps_cpu);
				afficher_instance(instance);
				remettre_instance_libre(instance);
				break;
			
			
			//sortir
			case 5:
				liberer_instances(&instance);
				printf("A BIENTOT\n\n");
				return 0;
				break;
				
		}
	}
	
	return 0;
}

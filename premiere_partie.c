#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Traducteur.h"

void extraction_nombre_ligne(char NomFichier[], int* nb, int* max_caractere_ligne){
    FILE *fichier;
    fichier = fopen(NomFichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        return;
    }
    char c = getc(fichier);
    int cpt=0;
    while(c != EOF){
        if (c == '\n'){
            (*nb) ++;
            if (cpt > *max_caractere_ligne){
                *max_caractere_ligne = cpt;
            }
            cpt = 0;
        }
        else{
            cpt++;
        }
        c = getc(fichier);
    }
    (*nb)++;
    fclose(fichier);
}

int extraction_fichier2(char NomFichier[], char* ligne_contenue[], int* max_caractere_ligne, int* nombre_ligne) {
    FILE *fichier;

    long taille;

    fichier = fopen(NomFichier, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        return 0;
    }

    fseek(fichier, 0, SEEK_END);
    taille = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    if (taille == 0) {
        fclose(fichier);
        return 0;
    }

    char* contenu_complet = malloc(taille + 1);
    if (contenu_complet == NULL) {
        fclose(fichier);
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        return 0;
    }

    int taillereel = fread(contenu_complet, sizeof(char), taille, fichier);
    contenu_complet[taillereel] = '\0'; 

    fclose(fichier);
    const char delim[]="\n";
    int i = 0;
    char *word = strtok(contenu_complet, delim);
    while(word!=NULL && i< *nombre_ligne) {
        ligne_contenue[i]= strdup(word);
        i++;
        word = strtok(NULL, delim);
    }
    *nombre_ligne = i;
    free(contenu_complet);
    return 1;
}


int ecriture(char contenu[][9], int* nb_ligne){
    FILE* fichier;

    fichier= fopen("hexa.txt","w");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    int i;

    for(i=0;i<*nb_ligne-1;i++){
        fwrite(contenu[i], sizeof(char) , 8 , fichier);
        fputc('\n', fichier);
    }
    
    fwrite(contenu[*nb_ligne - 1], sizeof(char) , 8 , fichier);

    fclose(fichier);
    return 0;
    
}


int fonction_principale(char nom_fichier[]){
    int nombre_ligne=0, max_caractere_ligne=0;

    //char** contenu = extraction_fichier("test.txt", &nombre_ligne);
    extraction_nombre_ligne(nom_fichier, &nombre_ligne,&max_caractere_ligne);
    char* contenu[nombre_ligne];
    
    extraction_fichier2(nom_fichier, contenu,&max_caractere_ligne,&nombre_ligne);
    char hexa[nombre_ligne][9];

    int condition = fonction_global_traducteur(contenu, &nombre_ligne, hexa);
    if (condition ==0){
        ecriture(hexa, &nombre_ligne);
        return 0;
    }
    else{
        return 1;
    }
}
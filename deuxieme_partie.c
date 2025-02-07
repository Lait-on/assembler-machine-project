#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fonctions_assembleurs.h"

// initialisation des bits d'état et de PC comme des variables globales

int PC=0, C= 0, N=0, Z=0;

//mise en memoire du programme par ligne

void convertisseur_Hexa_to_octet(char Hexa[], char memoire[], int i){
    long valeur = strtol(Hexa, NULL, 16);
    memoire[4*i]= ((valeur & 0xFF000000)>>24) & 0xFF;
    memoire[4*i+1]= ((valeur & 0x00FF0000)>>16) & 0xFF;
    memoire[4*i+2]= ((valeur & 0x0000FF00)>>8) & 0xFF;
    memoire[4*i+3]= (valeur & 0x000000FF) & 0xFF;
}

// recuperation des informations de la ligne en memoire a executee

int recuperation_decimal(char memoire[], short decimal[]){
    
    decimal[0]= (memoire[PC]& 0xF8) >> 3;
    
    decimal[1]= ((memoire[PC] & 0x7) << 2 ) | ((memoire[PC+1] & 0xC0) >> 6);
    
    decimal[2]= (memoire[PC+1] & 0x3E) >> 1;
   
    decimal[3]= memoire[PC+1] & 0x1;

    decimal[4]= (memoire[PC + 2] << 8) | (memoire[PC +3] & 0xFF);   

    return 0;
}

//recuperation des informations du fichier machine puis mise en memoire ligne par ligne

void stockage_information_ligne(char memoire[]){
    FILE* fichier = fopen("hexa.txt", "r");
    char hexa[10];
    int i=0;
    while (fread(hexa,sizeof(char), 9,fichier)){
        hexa[10]='\0';
        convertisseur_Hexa_to_octet(hexa, memoire,i++);
    }
    fclose(fichier);
}

//programme principale pour executer le programme ecrit dans le fichier machine

int execution(){
    // initialisation de la machine (memoire/registre)
    char memoire[65536];
    short registre[32];

    //mise en memoire de notre programme
    stockage_information_ligne(memoire);
    
    //initialisation des conditions de notre boucle, du registre 0 toujours egale a 0 et de la graine d'aleatoire
    int condition_fin=1;
    registre[0]=0;
    srand(time(NULL));
    
    //boucle d'execution
    while (condition_fin)
    {
        //recuperation des informations de la ligne pointee par PC
        short decimal[5]= {0,0,0,0,0};
        if (recuperation_decimal(memoire,decimal) ==1){
            printf("Erreur memoire !!");
            return 1;
        }
        


        //execution des instructions de saut
        if(decimal[0] >= 20 && decimal[0] <= 26){
            if (decimal[0] == 20){
                jmp(decimal);
            }
            else if(decimal[0] == 21){
                jzs(decimal);
            }
            else if(decimal[0] == 22){
                jzc(decimal);
            }
            else if(decimal[0] == 23){
                jcs(decimal);
            }
            else if(decimal[0] == 24){
                jcc(decimal);
            }
            else if(decimal[0] == 25){
                jns(decimal);
            }
            else if(decimal[0] == 26){
                jnc(decimal);
            }
        }
        //execution des autres instructions
        else{
            if(decimal[0] == 0){
                add(registre, decimal);
            }
            else if(decimal[0] == 1){
                sub(registre, decimal);
            }
            else if(decimal[0] == 2){
                mul(registre, decimal);
            }
            else if(decimal[0] == 3){
                DIV(registre, decimal);
            }
            else if(decimal[0] == 4){
                AND(registre, decimal);
            }
            else if(decimal[0] == 5){
                OR(registre, decimal);
            }
            else if(decimal[0] == 6){
                XOR(registre, decimal);
            }
            else if(decimal[0] == 7){
                shl(registre, decimal);
            }
            else if(decimal[0] == 10){
                ldb(registre, memoire, decimal);
            }
            else if(decimal[0] == 11){
                ldw(registre, memoire, decimal);
            }
            else if(decimal[0] == 12){
                stb(registre, memoire, decimal);
            }
            else if(decimal[0] == 13){
                stw(registre, memoire ,decimal);
            }
            else if(decimal[0] == 27){
                in(registre, decimal);
            }
            else if(decimal[0] == 28){
                out(registre, decimal);
            }
            else if(decimal[0] == 29){
                rnd(registre, decimal);
            }
            else if(decimal[0] == 31){
                //instruction hlt, on fini la boucle 
                condition_fin = 0;
            }
            else{
                return 1;
            }
            //si l'instruction n'est pas un saut, on pointe vers la ligne suivante
            PC +=4;
        }
        //registre 0 toujours a 0 meme apres changement
        registre[0]=0;
    }
    
    //le programme s'est bien execute
    return 0;
    
}




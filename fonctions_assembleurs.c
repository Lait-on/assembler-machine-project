#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// initialisation des bits d'état et de PC comme des variables globales de fichier deuxieme_partie.c

extern int PC,Z,N,C;  

// execution des instructions arithmétiques et logiques

void add(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));


    int valeur = (registre[rn] & 0xFFFF) + (S & 0xFFFF);
    


    Z = valeur == 0;
    N = (valeur>>15)& 1;
    C = (valeur >>16) & 1;


    registre[rd] = (N) << 15 | (valeur & 0x7FFF);
    
}

void sub(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));

    
    int valeur = (registre[rn] & 0xFFFF) - (S & 0xFFFF);
    


    Z = valeur == 0;
    N = (valeur>>15)& 1;
    C = (valeur >>16) & 1;


    registre[rd] = (N) << 15 | (valeur & 0x7FFF);
    
}

void mul(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));
    
    
    int valeur= registre[rn] * S;

    Z = valeur == 0;
    N = valeur>>31;
    C = (valeur >>15) & 1;
    
    registre[rd] = valeur & 0xFFFF;
}

// les fonctions suivantes sont en majuscule car les fonctions existent deja en C dans les bibliotheques classiques

void DIV(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));


    int valeur = registre[rn] / S;
    

    Z = valeur == 0;
    N = valeur>>31;
    C = (valeur >>15) & 1;

    registre[rd] = N << 15 | (valeur & 0x7FFF);
}

void AND(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));
    

    registre[rd] = registre[rn] & (S & 0xFFFF);
    

    Z = registre[rd] == 0;
    N = registre[rd] >>15;
    C = 0;
}

void OR(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));
    

    registre[rd] = registre[rn] | (S & 0xFFFF);


    Z = registre[rd] == 0;
    N = registre[rd] >>15;
    C = 0;
}

void XOR(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));
    

    registre[rd] = registre[rn] ^ (S & 0xFFFF);
    

    Z = registre[rd] == 0;
    N = registre[rd] >>15;
    C = 0;
}

void shl(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));
    
    if(S>=0){
        registre[rd] = (registre[rn] << (S-1)) & 0xFFFF;
        C = (registre[rd] & 0x8000)>>15;
        registre[rd] = registre[rd] << 1;
    }
    else{
        registre[rd] = (registre[rn] >> (-S-1)) & 0xFFFF;
        C = registre[rd] & 0x1;
        registre[rd] = registre[rd] >> 1;
    }
    
    
    Z = registre[rd]==0;
    
    N = registre[rd] >>15;
    
}



// execution des instructions de transfert


void ldb(short registre[], char memoire[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));

    registre[rd] = (memoire[registre[rn]+S] & 0xFF) | ((memoire[registre[rn]+S]&0x80)* 0xFF00);

    Z = registre[rd] == 0;
    N = registre[rd] >>15;
    C = 0;
}

void ldw(short registre[], char memoire[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));

    registre[rd] = (memoire[rregistre[rn]+S] & 0xFF) | (memoire[registre[rn]+S+1] << 8);

    Z = registre[rd] == 0;
    N = registre[rd] >>15;
    C = 0;
}

void stb(short registre[], char memoire[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));

    memoire[registre[rd] +S]= registre[rn] & 0xFF;

    Z = memoire[registre[rd] +S] == 0;
    N = memoire[registre[rd] +S] >>7;
    C = 0;
}

void stw(short registre[], char memoire[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));

    memoire[registre[rd] +S]= registre[rn] & 0xFF;
    memoire[registre[rd] +S+1]= (registre[rn] & 0xFF00) >> 8;

    Z = registre[rn] == 0;
    N = registre[rn] >>15;
    C = 0;
}



// execution des instructions de sauts

void jmp(short decimal[]){

    PC = decimal[4];
}

void jzs(short decimal[]){  
    if (Z == 1){
        PC = decimal[4];
    } 
    else{
        PC = PC + 4;
    }
}

void jzc(short decimal[]){
    if (Z == 0){
        PC = decimal[4];
    } 
    else{
        PC +=4;
    }
}

void jcs(short decimal[]){
    if (C == 1){
        PC = decimal[4];
    } 
    else{
        PC +=4;
    }
}

void jcc(short decimal[]){
    if (C == 0){
        PC = decimal[4];
    } 
    else{
        PC +=4;
    }
}

void jns(short decimal[]){
    if (N == 1){
        PC = decimal[4];
    } 
    else{
        PC +=4;
    }
}

void jnc(short decimal[]){
    if (N == 0){
        PC = decimal[4];
    } 
    else{
        PC +=4;
    }
}



// execution entree-sortie

void in(short registre[], short decimal[]){
    int valeur;
    short rd = decimal[1];
    printf("Inserer une valeur:\n");
    scanf("%d",&valeur);

    C = 0;
    N = valeur<0;
    Z = valeur==0;
    registre[rd] = ((N) * 32768) | (valeur & 32767); 

}

void out(short registre[], short decimal[]){
    short rd=decimal[1];
    printf("%d\n", registre[rd]);

    C = 0;
    N = registre[rd] < 0;
    Z =registre[rd] == 0;
}



// execution d'instructions diverses

void rnd(short registre[], short decimal[]){
    short rd = decimal[1];
    short rn = decimal[2];
    short S = (decimal[4] * decimal[3]) | (registre[decimal[4]] * (1-decimal[3]));

    int valeur = rn + ( rand() % (S - rn));  

    Z = valeur == 0;

    N = valeur>>31;
    
    C = (valeur & 0x7FFFFFFF) > 0x7FFF; 

    registre[rd] = N << 15 | (valeur & 0x7FFF);
}


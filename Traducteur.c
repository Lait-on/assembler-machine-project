#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_mot(char* mot[], int condition_etiquette){
    int i= 0;
    while (mot[i]!=NULL){
        free(mot[i]);
        i++;
    }
}

int is_hexa_or_dec(char mot[], int dec[]){
    
    if (mot[1]=='h'){
        for(int i=2; i<strlen(mot);i++){
            if ((mot[i] < '0' || mot[i] > '9') && (mot[i]<'A' || mot[i]>'F')){
                return 0;
            }

        }
        char* fin_trad;
        long decimal = strtol(mot+2, &fin_trad, 16);

        if (*fin_trad != '\0' || (decimal<-32768 || decimal>32767)) {
            return 0;
        }
        dec[3]=1;
        dec[4]= (int)decimal; 
    }
    else{
        for(int i=1; i<strlen(mot);i++){
            if ((mot[i] < '0' || mot[i] > '9') && mot[i]!='-'){
                return 0;
            }

        }
        int decimal= atoi(mot+1);
        if (decimal==0 || (decimal<-32768 || decimal>32767)){
            return 0;
        }
        dec[3]=1;
        dec[4]= decimal; 
    }
    

    return 1;
}

int separation_contenu(char ligne[],char* mot[]){
    const char delim[] = ": ,\t";
    int i=0;
    char *word = strtok(ligne, delim);

    while (i<6) {
        if (word != NULL) {
        mot[i] = strdup(word);
        word = strtok(NULL, delim);
        }
        else{
            mot[i]=NULL;
        }
        
        i++;
    }

    return 0;
}

int trad_registre_to_dec(char registre[]){
    if (registre[0] !='r'){
        return -1;
    }
    if (strlen(registre) == 2 && (registre[1]<='9' && registre[1]>= '0')){
        
        return atoi(registre+1);
    }
    else if (strlen(registre) == 3 && (registre[1]<='9' && registre[1]>= '0') && (registre[2]<='9' && registre[2]>= '0')){
        
        int entier = atoi(registre+1);
        if (entier <32 && entier>=0){
            return entier;
        }
        else{
            return -1;
        }

    }
    else{
        return -1;
    }

}

int fct(int dec[],char* mot[],int condition_deux_points){
    int indice = 1 + condition_deux_points;

    dec[1] = trad_registre_to_dec(mot[indice]);
    if (dec[1]==-1){
        return 1;
    }
  
    indice++;

    dec[2] = trad_registre_to_dec(mot[indice]);
    if (dec[2]==-1){
        return 1;
    }

    indice++;

    if (mot[indice][0]=='r'){
        dec[3]=0;
        dec[4] = trad_registre_to_dec(mot[indice]);
        if (dec[4]==-1){
            return 1;
        }
    }
    else{
            if(mot[indice][0]=='#' && mot[indice][1]=='0'){
                dec[3]=1;
                dec[4]=0;
            }
            else if(mot[indice][0]!= '#' || is_hexa_or_dec(mot[indice],dec)==0){
                return 1;
            }
        }

    indice++;
    if(mot[indice]!=NULL){
        return 1;
    }

    return 0;
}

int fct2(int dec[], char* mot[], int condition_deux_points){
    if (mot[3 + condition_deux_points] != NULL){
        return 1;
    }

    const char delim[] = "()";

    if (dec[0] == 10 ||  dec[0] == 11){
        dec[1]= trad_registre_to_dec(mot[1+condition_deux_points]);
        if (dec[1]==-1){
            return 1; 
        }
        char *argument = strtok(mot[2+condition_deux_points], delim);

        dec[2]= trad_registre_to_dec(argument);
        if (dec[2]==-1){
            return 1; 
        }

        argument = strtok(NULL, delim);

        if(argument[0]=='r'){
            dec[3]=0;
            dec[4]= trad_registre_to_dec(argument);
            if (dec[4]==-1){
            return 1; 
            }
        }
        else{
            if(argument[0]=='#' && argument[1]=='0'){
                dec[3]=1;
                dec[4]=0;
            }
            else if(argument[0]!= '#' || is_hexa_or_dec(argument,dec)==0){
                return 1;
            }
        }

    }
    else{
        dec[2]= trad_registre_to_dec(mot[2+condition_deux_points]);
        if (dec[2]==-1){
            return 1; 
        }
        char *argument = strtok(mot[1+condition_deux_points], delim);

        dec[1]= trad_registre_to_dec(argument);
        if (dec[1]==-1){
            return 1; 
        }

        argument = strtok(NULL, delim);

        if(argument[0]=='r'){
            dec[3]=0;
            dec[4]= trad_registre_to_dec(argument);
            if (dec[4]==-1){
            return 1; 
            }
        }
        else{
            if(argument[0]=='#' && argument[1]=='0'){
                dec[3]=1;
                dec[4]=0;
            }
            else if(argument[0]!= '#' || is_hexa_or_dec(argument,dec)==0){
                return 1;
            }
        }
    }
    return 0;
}

int fctinout(int dec[], char* mot[], int condition_deux_points){
    if (mot[2+condition_deux_points]!=NULL){
        return 1;
    }
    dec[1]= trad_registre_to_dec(mot[1+condition_deux_points]);
    if (dec[1]==-1){
        return 1;
    }
    return 0;
}

int trad_txt_to_dec(char contenu[], char* etiquettes[],char* stockage_etiquette[],int dec[], int numero_ligne){
    
    int condition_deux_points=0;

    if (strchr(contenu,':')!= NULL){
        if( *strchr(contenu,':')== contenu[0]){
            return 1;
        }
        else if( strchr(contenu,':') == strrchr(contenu,':') ){
        condition_deux_points=1;
        }
        else{
            return 1;
        }
    }
   

    char* mot[6];

    separation_contenu(contenu,mot);    

    if (condition_deux_points == 1){
        etiquettes[numero_ligne] =strdup(mot[0]);
         
    }
    else{
        etiquettes[numero_ligne] = NULL;
    }

    if (strcmp(mot[0+condition_deux_points], "add") == 0) {
        dec[0] = 0;
    } 
    else if (strcmp(mot[0+condition_deux_points], "sub") == 0) {
        dec[0] = 1;
    } 
    else if (strcmp(mot[0+condition_deux_points], "mul") == 0) {
        dec[0] = 2;
    } 
    else if (strcmp(mot[0+condition_deux_points], "div") == 0) {
        dec[0] = 3;
    } 
    else if (strcmp(mot[0+condition_deux_points], "and") == 0) {
        dec[0] = 4;
    } 
    else if (strcmp(mot[0+condition_deux_points], "or") == 0) {
        dec[0] = 5;
    } 
    else if (strcmp(mot[0+condition_deux_points], "xor") == 0) {
        dec[0] = 6;
    } 
    else if (strcmp(mot[0+condition_deux_points], "shl") == 0) {
        dec[0] = 7;
    } 
    else if (strcmp(mot[0+condition_deux_points], "ldb") == 0) {
        dec[0] = 10;
    } 
    else if (strcmp(mot[0+condition_deux_points], "ldw") == 0) {
        dec[0] = 11;
    } 
    else if (strcmp(mot[0+condition_deux_points], "stb") == 0) {
        dec[0] = 12;
    } 
    else if (strcmp(mot[0+condition_deux_points], "stw") == 0) {
        dec[0] = 13;
    } 
    else if (strcmp(mot[0+condition_deux_points], "jmp") == 0) {
        dec[0] = 20;
    } 
    else if (strcmp(mot[0+condition_deux_points], "jzs") == 0) {
        dec[0] = 21;
    } 
    else if (strcmp(mot[0+condition_deux_points], "jzc") == 0) {
        dec[0] = 22;
    } 
    else if (strcmp(mot[0+condition_deux_points], "jcs") == 0) {
        dec[0] = 23;
    } 
    else if (strcmp(mot[0+condition_deux_points], "jcc") == 0) {
        dec[0] = 24;
    }
    else if (strcmp(mot[0+condition_deux_points], "jns") == 0) {
        dec[0] = 25;
    }
    else if (strcmp(mot[0+condition_deux_points], "jnc") == 0) {
        dec[0] = 26;
    } 
    else if (strcmp(mot[0+condition_deux_points], "in") == 0) {
        dec[0] = 27;
    } 
    else if (strcmp(mot[0+condition_deux_points], "out") == 0) {
        dec[0] = 28;
    } 
    else if (strcmp(mot[0+condition_deux_points], "rnd") == 0) {
        dec[0] = 29;
    } 
    else if (strcmp(mot[0+condition_deux_points], "hlt") == 0) {
        dec[0] = 31;
    } 
    else {
        free_mot(mot,condition_deux_points);
        return 1; 
    }

    if (dec[0]<=7 || dec[0]==29){
        if (fct(dec,mot,condition_deux_points)==1){
            free_mot(mot,condition_deux_points);
            return 1;
        }
    }
    else if(dec[0]<=13){
        if (fct2(dec,mot,condition_deux_points)==1){
            free_mot(mot,condition_deux_points);
            return 1;
        }
    }
    else if(dec[0]<=26){
        int i;
        for(i=1;i<5;i++){
            dec[i]=0;
        }
        if (mot[1+condition_deux_points]!= NULL && (mot[2+condition_deux_points]== NULL)){
            
            stockage_etiquette[numero_ligne]=strdup(mot[1+condition_deux_points]);
            
        }
        else{
            free_mot(mot,condition_deux_points);
            return 1;
        }
    }
    else if(dec[0]<=28){
        if (fctinout(dec,mot,condition_deux_points)==1){
            free_mot(mot,condition_deux_points);
            return 1;
        }
    }
    else{
        int i;
        if(condition_deux_points==1){
            if(mot[2]!=NULL || mot[3]!=NULL || mot[4]!=NULL || mot[5]!=NULL ) {
                free_mot(mot,condition_deux_points);
                return 1;
            }
        }
        else{
            if(mot[1]!=NULL || mot[2]!=NULL || mot[3]!=NULL || mot[4]!=NULL || mot[5]!=NULL ) {
                free_mot(mot,condition_deux_points);
                return 1;
            }
        }
       for(i=1;i<5;i++){
            dec[i]=0;
        }
    }
    free_mot(mot,condition_deux_points);
    return 0;
}

void trad_decim_to_hexa(int dec[], char hexa[]){
    int result,i;
    char hexa_temp_debut[9];
    char hexa_temp_fin[5];

    result = dec[0];

    result=result<<5;
    result += dec[1];

    result=result<<5;
    result += dec[2];

    result=result<<1;
    result += dec[3];

    sprintf(hexa_temp_debut, "%X", result);
    sprintf(hexa_temp_fin, "%X", (unsigned short)dec[4]);

    int n=strlen(hexa_temp_fin);
    if (n<4){
        for(i=0;i<4-n;i++){
            strcat(hexa_temp_debut,"0");
        }
    }

    strcat(hexa_temp_debut,hexa_temp_fin);

    if (strlen(hexa_temp_debut)<8){
        int n= strlen(hexa_temp_debut);
        for (i=0; i< 8-n;i++){
            hexa[i]='0';
        }
        hexa[i]='\0';
        strcat(hexa,hexa_temp_debut);
    }
    else{
        strcpy(hexa,hexa_temp_debut);
    }

}

int decimal_jump(int dec[], char* etiquettes[], char* stockage_etiquettes[], int nombre_ligne, int numero_ligne){
    int i,condition_generale=0;
    
    if (stockage_etiquettes[numero_ligne][0]=='r'){
            dec[4] = trad_registre_to_dec(stockage_etiquettes[numero_ligne]);
            if (dec[4]!= -1 && dec[4]%4==0){
                condition_generale=0;
            }
            else{
                condition_generale=1;
            }
            
    }
    else if(stockage_etiquettes[numero_ligne][0]=='#'){
        if (stockage_etiquettes[numero_ligne][0] == '0'){
            dec[3]=1;
            dec[4]=0;
            condition_generale=1;}
        else{
            if(is_hexa_or_dec(stockage_etiquettes[numero_ligne],dec) && dec[4]%4==0){
                condition_generale=1;
            }
            else{
                condition_generale = 0;
            }
        }
        
    }
    
    if (condition_generale == 0){
        for(i=0; i<nombre_ligne ;i++){
            if (etiquettes[i] != NULL && strcmp(etiquettes[i],"")!=0){
                if(strcmp(etiquettes[i], stockage_etiquettes[numero_ligne])==0){
                    dec[3]=1;
                    dec[4]= i * 4;
                    condition_generale=1;
                    free(stockage_etiquettes[numero_ligne]);
                    break;
                }
            }
        }
    }
    if (condition_generale ==0){
        return 1;
    }

    return 0;
}

int fonction_global_traducteur(char* contenu[], int *nombre_ligne, char contenu_hexa[][9]){
    int condition=0,i,j,c ;
    char* stockage_etiquettes[*nombre_ligne]; 
    char* etiquettes[*nombre_ligne];

    int attente[*nombre_ligne];
    int attente_dec[*nombre_ligne][5];
    int compteur=0;

    for(i=0;i<*nombre_ligne;i++){
        int dec[5]= {0,0,0,0,0};
        c = trad_txt_to_dec(contenu[i], etiquettes, stockage_etiquettes,dec,i);
        
        free(contenu[i]);
        if (dec[0]>=20 && dec[0]<=26){
            for (j=0; j<5;j++){
            attente_dec[i][j]=dec[j];
            }
            
            attente[compteur] = i;
            compteur++;
        }
        else{

            char hexa[9]={'\0'};
            trad_decim_to_hexa(dec,hexa);
            if (c >= 1){
                printf("Erreur ligne %d \n", i);
                condition+=1;
            }
            else{
                strcpy(contenu_hexa[i],hexa);
            }
        }

    }
    
    if (compteur >0){
        
        for (i=0;i<compteur;i++){
            
            
            c = decimal_jump(attente_dec[attente[i]],etiquettes,stockage_etiquettes,*nombre_ligne,attente[i]);
            if (c==1){
                printf("Erreur ligne %d \n", attente[i]);
                condition++;
                continue;
            }
            
            char hexa[9]={'\0'};
            
            trad_decim_to_hexa(attente_dec[attente[i]],hexa);
            
            strcpy(contenu_hexa[attente[i]],hexa);
            
        }
        for(i=0;i<*nombre_ligne;i++){
            if(etiquettes[i]!=NULL){
                free(etiquettes[i]);
            }
        }
    }
    if (condition>=1){
        return 1;
    }

    return 0;
}




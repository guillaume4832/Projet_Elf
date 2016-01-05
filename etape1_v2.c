#include <stdio.h>
#include <stdlib.h>

char* readFileBytes(const char *name)
{
    FILE *fl = fopen(name, "r");
    fseek(fl, 0, SEEK_END);
    long len = ftell(fl);
    char *ret = malloc(len);
    fseek(fl, 0, SEEK_SET);
    fread(ret, 1, len, fl);
    fclose(fl);
    return ret;
}

int main(int argc, char * argv[]){
    int isValid = 1;
    char* fileBytes = readFileBytes(argv[1]);
    /* Lecture des bits 'magiques' ELF */
    if(fileBytes[0] == 127 && fileBytes[1] == 69 && fileBytes[2] == 76 && fileBytes[3] == 70){
        printf("[*] Fichier ELF Reconnu\n");
    }
    else{
        printf("[E] Le fichier founi n'est pas un fichier ELF\n");
        isValid = 0;
    }
    int i = 4;
    /* Lecture de la classe */
    if(isValid == 1){
        if(fileBytes[i] == 1){
            printf("[*] Classe: 32 bits ELFCLASS32\n");
        }
        else if(fileBytes[i] == 2){
            printf("[*] Classe: 64 bits ELFCLASS64\n");
        }
        else{
            printf("[E] Erreur de lecture de la classe du fichier ELF\n");
            isValid = 0;
        }
    }
    i++;

    /* Lecture de l'encodage des données processeur */
    if(isValid == 1){
        if(fileBytes[i] == 1){
            printf("[*] EI_DATA: ELFDATA2LSB Little endian\n");
        }
        else if(fileBytes[i] == 2){
            printf("[*] EI_DATA : ELFDATA2MSB Big endian\n");
        }
        else{
            printf("[E] Erreur de lecture de l'encodage EI_DATA ELF\n");
            isValid = 0;
        }
    }
    i++;

    /* Lecture de la version du header */
    if(isValid == 1){
        if(fileBytes[i] != 0){
            printf("[*] Version du header : %d\n",fileBytes[i]);
        }
        else{
            printf("[E] Version du header invalide\n");
            isValid = 0;
        }
    }
    i++;

    /* Lecture de l'os ABI */

    if(isValid == 1){
        if(fileBytes[i] == 0){
            printf("[*] OS/ABI : Unix System V\n");
        }
        else if(fileBytes[i] == 1){
            printf("[*] OS/ABI : HP-UX\n");
        }
        else if(fileBytes[i] == 2){
            printf("[*] OS/ABI : NetBSD\n");
        }
        else if(fileBytes[i] == 3){
            printf("[*] OS/ABI : Linux\n");
        }
        else if(fileBytes[i] == 6){
            printf("[*] OS/ABI : Sun Solaris\n");
        }
        else if(fileBytes[i] == 7){
            printf("[*] OS/ABI : IBM AIX\n");
        }
        else if(fileBytes[i] == 8){
            printf("[*] OS/ABI : SGI Irix\n");
        }
        else if(fileBytes[i] == 9){
            printf("[*] OS/ABI : FreeBSD\n");
        }
        else if(fileBytes[i] == 10){
            printf("[*] OS/ABI : Compaq TRU64\n");
        }
        else if(fileBytes[i] == 11){
            printf("[*] OS/ABI : Novell Modesto\n");
        }
        else if(fileBytes[i] == 12){
            printf("[*] OS/ABI : OpenBSD\n");
        }
        else if(fileBytes[i] == 64){
            printf("[*] OS/ABI : ARL EABI\n");
        }
        else if(fileBytes[i] == 97){
            printf("[*] OS/ABI : ARM\n");
        }
        else if(fileBytes[i] == 255){
            printf("[*] OS/ABI : Standalone\n");
        }
        else{
            printf("[E] OS/ABI : Inconnu\n");
            printf("%d\n",fileBytes[i]);
            isValid = 0;
        }

    }
    i++;
    /* Vérification de l'espace réservé EI_PAD */
    for(i;i<16;i++){
        if(fileBytes[i] != 0){
            printf("[E] Bits de reserves EI_PAD non réservés à 0");
            isValid = 0;
        }
    }
    i++;






    return 0;
}

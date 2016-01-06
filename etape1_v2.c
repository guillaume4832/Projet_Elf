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
    int is64 = 0;
    unsigned char* fileBytes = readFileBytes(argv[1]);
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
            is64 = 1;
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
            printf("[W] OS/ABI : Inconnu\n");
            printf("%d\n",fileBytes[i]);
            isValid = 0;
        }

    }
    i++;

    /* Lecture de la version de l'ABI */
    if(isValid == 1){
        printf("[*] Version de l'ABI : %d\n",fileBytes[i]);
    }
    i++;

    /* Vérification de l'espace réservé EI_PAD */
    if(isValid == 1){
        for(i;i<15;i++){
            if(fileBytes[i] != 0){
                printf("[E] Bits de reserves EI_PAD non réservés à 0");
                isValid = 0;
                break;
            }
        }
    }
    i++;

    /* Lecture du type de l'objet */
    if(isValid == 1){
        if(fileBytes[i] == 0){
            printf("[*] Pas de type de fichier\n");
        }
        else if(fileBytes[i] == 1){
            printf("[*] Objet de type : Repositionable (Relocatable)\n");
        }
        else if(fileBytes[i] == 2){
            printf("[*] Objet de type : Executable\n");
        }
        else if(fileBytes[i] == 3){
            printf("[*] Objet de type : Partagé\n");
        }
        else if(fileBytes[i] == 4){
            printf("[*] Objet de type : Core\n");
        }
        else{
            printf("[W] Type inconnu\n");
        }
    }
    i+=2;

    /* Lecture de la machine cible */
    if(isValid == 1){
        if(fileBytes[i] == 0){
            printf("[*] Aucune machine cible\n");
        }
        else if(fileBytes[i] == 2){
            printf("[*] Machine cible : SPARC\n");
        }
        else if(fileBytes[i] == 3){
            printf("[*] Machine cible : Intel 80386\n");
        }
        else if(fileBytes[i] == 4){
            printf("[*] Machine cible : Motorola 68000\n");
        }
        else if(fileBytes[i] == 7){
            printf("[*] Machine cible : Intel i860\n");
        }
        else if(fileBytes[i] == 8){
            printf("[*] Machine cible : MIPS I\n");
        }
        else if(fileBytes[i] == 19){
            printf("[*] Machine cible : Intel i960\n");
        }
        else if(fileBytes[i] == 20){
            printf("[*] Machine cible : PowerPC\n");
        }
        else if(fileBytes[i] == 40){
            printf("[*] Machine cible : ARM\n");
        }
        else if(fileBytes[i] == 50){
            printf("[*] Machine cible : Intel IA64\n");
        }
        else if(fileBytes[i] == 62){
            printf("[*] Machine cible : x64\n");
        }
        else{
            printf("[W] Machine cible non reconnue\n");
        }
    }
    i+=2;

    /* Lecture de la version du fichier */
    if(isValid == 1){
        if(fileBytes[i] == 0){
            printf("[E] Version invalide\n");
            isValid = 0;
        }
        else if(fileBytes[i] == 1){
            printf("[*] Version 0x1\n");
        }
        else{
            printf("[W] Version inconnue\n");
        }
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        printf("[*] Adresse du point d'entrée : 0x%x\n",sum);
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        printf("[*] Début des en-têtes de programme : %d\n",sum);
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        printf("[*] Début des en-têtes de section : %d\n",sum);
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        printf("[*] Fanions : 0x%x\n",sum);
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        printf("[*] Taille de cet en-tête : %d\n",sum);
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        printf("[*] Taille de l'en-tête du programme : %d\n",sum);
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        printf("[*] Nombre d'en-tête du programme : %d\n",sum);
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        printf("[*] Taille des en-têtes de section : %d\n",sum);
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        printf("[*] Nombre d'en-tête de section : %d\n",sum);
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        printf("[*] Table d'indexes des chaînes d'en-tête de section : %d\n",sum);
    }
    i+=2;


    return 0;
}

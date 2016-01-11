#include "elfsection.h"


void readSection(int numsection,char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr){
    int i;
    char* name = malloc(sizeof(char)*75);
    unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
    int addrStrTable = shdr[header.e_shstrndx].sh_offset;// Récupération de l'adresse de la string table
    
    int n = 0;
    int j = addrStrTable + shdr[numsection].sh_name;

    while (fileBytes[j] != 0)
    {
        name[n] = fileBytes[j];
        j++;
        n++;

    }
    name[n] = 0;
    printf("\nVidange hexadécimale de la section < %s > :\n",name);

    int addr = shdr[numsection].sh_offset; // Adresse de la section
    ///printf("%x", addr);
    
    int size = shdr[numsection].sh_size; // Taille de la section
    ///printf("%d", size);
    char * tampon = malloc(sizeof(char)*16);
    int idxTab = 1;
    int startLine = 1;
    int adrHex = 0;

    for(i=0;i<size;i++){
        if(i%4 == 3)
        {
            printf("%02x ",fileBytes[addr+i]);
            idxTab++;
        }
        else{
            if(startLine == 1){
                startLine = 0;
                printf("  0x%08x ",adrHex);
                adrHex+=16;
            }
            printf("%02x",fileBytes[addr+i]);
        }

        tampon[i%16] = fileBytes[addr+i];
        if(tampon[i%16] < 33 || tampon[i%16] > 126){
            tampon[i%16] = '.';
        }
        if(i%16 == 15){
            printf("\t %s",tampon);
            tampon[0] = 0;
            printf("\n");
            startLine = 1;
            idxTab = 1;
        }
    }
    tampon[i%16] = 0;
    int k =0;
    for(k;k<i%8;k++){
        printf(" ");
    }
    if(size%8==0)
        idxTab--;
    for(k=0;k<4-idxTab;k++){

        printf("         ");
    }
    printf("\t %s",tampon);
    printf("\n\n");

}

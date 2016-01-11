#include "elfsymtable.h"

int getIndSectionSymtab(Elf32_Ehdr header,Elf32_Shdr* shdr) {
	int i;
	for(i=0;i<header.e_shnum;i++){
		if (shdr[i].sh_type == 2) {
			return i;
		}
	}
	return 0;
}


void readSymTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr, Elf32_Sym* sym, int isVerbose){
    if(isVerbose == 1){
        printf("\n");
        printf("  Num:    Valeur Tail   Type     Lien   Ndx Nom\n");
    }

    unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
	int isBigEndian = header.e_ident[EI_DATA]-1;
    int i;
    int addrSymTable;
    int addrStrTable;
    int size;
    for(i=0;i<header.e_shnum;i++){
        if(shdr[i].sh_type == 2){
            addrSymTable = shdr[i].sh_offset;
            size = shdr[i].sh_size;
        }
        if(i != header.e_shstrndx && shdr[i].sh_type == 3){
            addrStrTable = shdr[i].sh_offset;
        }
    }

    int k = addrSymTable;
    int numSymb = 0;

    while(k<size+addrSymTable){
        if(isVerbose == 1)
            printf("   %2d:",numSymb);
        numSymb++;
        /* Lecture de st_Name */

        int first = fileBytes[k];
        int second = fileBytes[k+1];
        int third = fileBytes[k+2];
        int fourth = fileBytes[k+3];
		int name;
		if(isBigEndian == 0)
            name = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            name = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;

        int addrStrName = addrStrTable+name;
		sym[numSymb].st_name = addrStrName;

        char * nameString = malloc(sizeof(char)*75);
        int j = 0;
        while(fileBytes[addrStrName] != 0){
            nameString[j] = fileBytes[addrStrName];
            addrStrName++;
            j++;
        }

        /* Nom du Symbol */
        nameString[j] = 0;

        k+=4;

        first = fileBytes[k];
        second = fileBytes[k+1];
        third = fileBytes[k+2];
        fourth = fileBytes[k+3];
        /* Valeur du symbol */
        int value;
        if(isBigEndian == 0)
            value = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            value = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
        if(isVerbose == 1)
            printf("  %08x",value);
		sym[numSymb].st_value = value;
        k+=4;

        first = fileBytes[k];
        second = fileBytes[k+1];
        third = fileBytes[k+2];
        fourth = fileBytes[k+3];

        /* Taille du symbol */
        int size;
        if(isBigEndian == 0)
            size = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            size = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
        if(isVerbose == 1)
            printf("   %2d",size);
		sym[numSymb].st_size = size;
        k+=4;

        int info = fileBytes[k];
		sym[numSymb].st_info = info;
        int bind = ELF32_ST_BIND(info);
        int type = ELF32_ST_TYPE(info);
        if(isVerbose == 1){
            printf("   ");
            switch(type){
              case 0:
                printf("NOTYPE ");
                break;
              case 1:
                printf("OBJECT ");
                break;
              case 2:
                printf("FUNC   ");
                break;
              case 3:
                printf("SECTION");
                break;
              case 4:
                printf("FILE   ");
                break;
              case 13:
                printf("LOPROC ");
                break;
              case 15:
                printf("HIPROC ");
                break;
              default:
                printf("UNKNOWN");
            }
            printf("  ");
            switch(bind){
              case 0:
                printf("LOCAL ");
                break;
              case 1:
                printf("GLOBAL");
                break;
              case 2:
                printf("WEAK  ");
                break;
              case 13:
                printf("LOPROC");
                break;
              case 15:
                printf("HIPROC");
                break;
              default:
                printf("UKN   ");
                break;
            }
        }
        k++;

        int other = fileBytes[k];

        k++;

        first = fileBytes[k];
        second = fileBytes[k+1];
        int shndx;
        if(isBigEndian == 0)
            shndx = first + second *16*16;
        else
            shndx = first *16*16 + second;
		sym[numSymb].st_shndx = shndx;
		sym[numSymb].st_other = 0;
        if(isVerbose == 1){
            printf("\t");
            switch(shndx){
              case SHN_UNDEF:
                printf("UND");
                break;
              case SHN_ABS:
                printf("ABS");
                break;
              default:
                printf("%3d",shndx);
                break;
            }

            printf(" %s",nameString);

            printf("\n");
        }
        k+=2;

    }
    if(isVerbose == 1)
        printf("\n");
}

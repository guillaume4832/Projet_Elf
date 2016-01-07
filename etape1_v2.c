#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

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


char* nom_type(Elf32_Word sh_type)
{
  switch (sh_type)
    {
    case SHT_NULL:		 	return "NULL";
    case SHT_PROGBITS:		return "PROGBITS";
    case SHT_SYMTAB:		return "SYMTAB";
    case SHT_STRTAB:		return "STRTAB";
    case SHT_RELA:			return "RELA";
    case SHT_HASH:			return "HASH";
    case SHT_DYNAMIC:		return "DYNAMIC";
    case SHT_NOTE:			return "NOTE";
    case SHT_NOBITS:		return "NOBITS";
    case SHT_REL:			return "REL";
    case SHT_SHLIB:			return "SHLIB";
    case SHT_DYNSYM:		return "DYNSYM";
    case SHT_INIT_ARRAY:	return "INIT_ARRAY";
    case SHT_FINI_ARRAY:	return "FINI_ARRAY";
    case SHT_PREINIT_ARRAY:	return "PREINIT_ARRAY";
    case SHT_GROUP:			return "GROUP";
    case SHT_SYMTAB_SHNDX:	return "SYMTAB SECTION INDICIES";
    case SHT_GNU_verdef:	return "VERDEF";
    case SHT_GNU_verneed:	return "VERNEED";
    case SHT_GNU_versym:	return "VERSYM";
    case 0x6ffffff0:		return "VERSYM";
    case 0x6ffffffc:		return "VERDEF";
    case 0x7ffffffd:		return "AUXILIARY";
    case 0x7fffffff:		return "FILTER";
    case SHT_GNU_LIBLIST:	return "GNU_LIBLIST";

	default: return "";
}
}


char* nom_flags(unsigned int flags) {

	char* val = malloc(sizeof(char)*9);

	int p;
	for(p=0;p<9;p++) {
			val[p]='\0';
	}
	int r;
	int i = 1;
	int b = 0;
	while (flags != 0)
	{
		r = flags%2;
        flags = flags/2;
        b = b+r*i;
        i = i*10;
	}



	if (b & 00000001)
	{
		val[0]='W';
		if (b & 00000010)
		{
		val[1]='A';
			if (b & 00000100)
			{
			val[2]='X';
			}
		}
		else if (b & 00000100)
			{
			val[1]='X';
			}
	}

	else if (b & 00000010)
	{
		val[0]='A';
		if (b & 00000100)
		{
		val[1]='X';
		}
	}

	else if (b & 00000100) {
		val[0]='X';
		}

	if(b!=0 && val[0]=='\0') {
	val = "U";

	}

	return val;


}



Elf32_Ehdr readHeader(char * filePath, int isVerbose){
    Elf32_Ehdr header;
    int isValid = 1;
    int is64 = 0;
    unsigned char* fileBytes = readFileBytes(filePath);
    /* Lecture des bits 'magiques' ELF */
    if(fileBytes[0] == 127 && fileBytes[1] == 69 && fileBytes[2] == 76 && fileBytes[3] == 70){
        if(isVerbose == 1)
            printf("[*] Fichier ELF Reconnu\n");
        header.e_ident[EI_MAG0] = fileBytes[0];
        header.e_ident[EI_MAG1] = fileBytes[1];
        header.e_ident[EI_MAG2] = fileBytes[2];
        header.e_ident[EI_MAG3] = fileBytes[3];
    }
    else{
        if(isVerbose == 1)
            printf("[E] Le fichier founi n'est pas un fichier ELF\n");
        isValid = 0;
    }
    int i = 4;
    /* Lecture de la classe */
    if(isValid == 1){
        if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] Classe: 32 bits ELFCLASS32\n");

        }
        else if(fileBytes[i] == 2){
            if(isVerbose == 1)
                printf("[*] Classe: 64 bits ELFCLASS64\n");
            is64 = 1;
        }
        else{
            if(isVerbose == 1)
                printf("[E] Erreur de lecture de la classe du fichier ELF\n");
            isValid = 0;
        }
        header.e_ident[EI_CLASS] = fileBytes[i];
    }
    i++;

    /* Lecture de l'encodage des données processeur */
    if(isValid == 1){
        if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] EI_DATA: ELFDATA2LSB Little endian\n");
        }
        else if(fileBytes[i] == 2){
            if(isVerbose == 1)
                printf("[*] EI_DATA : ELFDATA2MSB Big endian\n");
        }
        else{
            if(isVerbose == 1)
                printf("[E] Erreur de lecture de l'encodage EI_DATA ELF\n");
            isValid = 0;
        }
        header.e_ident[EI_DATA] = fileBytes[i];
    }
    i++;

    /* Lecture de la version du header */
    if(isValid == 1){
        if(fileBytes[i] != 0){
            if(isVerbose == 1)
                printf("[*] Version du header : %d\n",fileBytes[i]);
        }
        else{
            if(isVerbose == 1)
                printf("[E] Version du header invalide\n");
            isValid = 0;
        }
        header.e_ident[EI_VERSION] = fileBytes[i];
    }
    i++;

    /* Lecture de l'os ABI */

    if(isValid == 1){
        if(fileBytes[i] == 0){
            if(isVerbose == 1)
                printf("[*] OS/ABI : Unix System V\n");
        }
        else if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] OS/ABI : HP-UX\n");
        }
        else if(fileBytes[i] == 2){
            if(isVerbose == 1)
                printf("[*] OS/ABI : NetBSD\n");
        }
        else if(fileBytes[i] == 3){
            if(isVerbose == 1)
                printf("[*] OS/ABI : Linux\n");
        }
        else if(fileBytes[i] == 6){
            if(isVerbose == 1)
                printf("[*] OS/ABI : Sun Solaris\n");
        }
        else if(fileBytes[i] == 7){
            if(isVerbose == 1)
                printf("[*] OS/ABI : IBM AIX\n");
        }
        else if(fileBytes[i] == 8){
            if(isVerbose == 1)
                printf("[*] OS/ABI : SGI Irix\n");
        }
        else if(fileBytes[i] == 9){
            if(isVerbose == 1)
                printf("[*] OS/ABI : FreeBSD\n");
        }
        else if(fileBytes[i] == 10){
            if(isVerbose == 1)
                printf("[*] OS/ABI : Compaq TRU64\n");
        }
        else if(fileBytes[i] == 11){
            if(isVerbose == 1)
                printf("[*] OS/ABI : Novell Modesto\n");
        }
        else if(fileBytes[i] == 12){
            if(isVerbose == 1)
                printf("[*] OS/ABI : OpenBSD\n");
        }
        else if(fileBytes[i] == 64){
            if(isVerbose == 1)
                printf("[*] OS/ABI : ARL EABI\n");
        }
        else if(fileBytes[i] == 97){
            if(isVerbose == 1)
                printf("[*] OS/ABI : ARM\n");
        }
        else if(fileBytes[i] == 255){
            if(isVerbose == 1)
                printf("[*] OS/ABI : Standalone\n");
        }
        else{
            if(isVerbose == 1){
                printf("[W] OS/ABI : Inconnu\n");
                printf("%d\n",fileBytes[i]);
            }
            isValid = 0;
        }
        header.e_ident[EI_OSABI] = fileBytes[i];
    }
    i++;

    /* Lecture de la version de l'ABI */
    if(isValid == 1){
        if(isVerbose == 1)
            printf("[*] Version de l'ABI : %d\n",fileBytes[i]);
        header.e_ident[EI_ABIVERSION] = fileBytes[i];
    }
    i++;

    /* Vérification de l'espace réservé EI_PAD */
    if(isValid == 1){
        for(i;i<15;i++){
            if(fileBytes[i] != 0){
                if(isVerbose == 1)
                    printf("[E] Bits de reserves EI_PAD non réservés à 0");
                isValid = 0;
                break;
            }
        }
    }
    i++;

    /* Lecture du type de l'objet */
    if(isValid == 1){
        if(isVerbose == 1){
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
        header.e_type = fileBytes[i];
    }
    i+=2;

    /* Lecture de la machine cible */
    if(isValid == 1){
        if(isVerbose == 1){
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
        header.e_machine = fileBytes[i];
    }
    i+=2;

    /* Lecture de la version du fichier */
    if(isValid == 1){
        if(fileBytes[i] == 0){
            if(isVerbose == 1)
                printf("[E] Version invalide\n");
            isValid = 0;
        }
        else if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] Version 0x1\n");
        }
        else{
            if(isVerbose == 1)
                printf("[W] Version inconnue\n");
        }
        header.e_version = fileBytes[i];
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        if(isVerbose == 1)
            printf("[*] Adresse du point d'entrée : 0x%x\n",sum);
        header.e_entry = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        if(isVerbose == 1)
            printf("[*] Début des en-têtes de programme : %d\n",sum);
        header.e_phoff = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        if(isVerbose == 1)
            printf("[*] Début des en-têtes de section : %d\n",sum);
        header.e_shoff = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        if(isVerbose == 1)
            printf("[*] Fanions : 0x%x\n",sum);
        header.e_flags = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        if(isVerbose == 1)
            printf("[*] Taille de cet en-tête : %d\n",sum);
        header.e_ehsize = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        if(isVerbose == 1)
            printf("[*] Taille de l'en-tête du programme : %d\n",sum);
        header.e_phentsize = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        if(isVerbose == 1)
            printf("[*] Nombre d'en-tête du programme : %d\n",sum);
        header.e_phnum = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        if(isVerbose == 1)
            printf("[*] Taille des en-têtes de section : %d\n",sum);
        header.e_shentsize = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        if(isVerbose == 1)
            printf("[*] Nombre d'en-tête de section : %d\n",sum);
        header.e_shnum = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum = first + second *16*16;
        if(isVerbose == 1)
            printf("[*] Table d'indexes des chaînes d'en-tête de section : %d\n",sum);
        header.e_shstrndx = sum;
    }
    i+=2;


    return header;
}

void readSheader(char * filePath, Elf32_Ehdr header,Elf32_Shdr* shdr,int isVerbose) {
	int i;
	unsigned char* fileBytes = readFileBytes(filePath);
	unsigned int addrStrTable;
	for(i=0;i<header.e_shnum;i++){
		int j = (unsigned int)header.e_shoff+i*(unsigned int)header.e_shentsize;
		/* Lecture des indices de nom de section */
		unsigned int first = fileBytes[j];
        unsigned int second = fileBytes[j+1];
        unsigned int third = fileBytes[j+2];
        unsigned int fourth = fileBytes[j+3];
        unsigned int sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;

		shdr[i].sh_name = sum;

		j+=4;
		/* Lecture de la définition de la sémantique de la section */
		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_type = sum;

		j+=4;

		/* Lecture des flags */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_flags = sum;

		j+=4;

		/* Lecture de l'adresse à laquelle le premier octect de la section doit se trouver, sinon 0 */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_addr = sum;

		j+=4;

		/* Lecture du décalage du premier octect de la section par rapport au début du fichier */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_offset = sum;

		if(i == header.e_shstrndx){
			addrStrTable = sum;
		}

		j+=4;

		/* Lecture de la taille de la section en octet */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_size = sum;



		j+=4;

		/* Lecture du lien vers un indice de la table des entêtes de section*/

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_link = sum;

		j+=4;

		/* Lecture des informations complémentaires */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_info = sum;

		j+=4;

		/* Lecture de addralign */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_addralign = sum;

		j+=4;

		/* Lecture de entsize */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		shdr[i].sh_entsize = sum;

		j+=4;


	}
    if(isVerbose == 1)
	   printf("  [Nr]  Nom\t\t        Type\t    Adr    \tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n");

	for(i=0;i<header.e_shnum;i++){


		// addrStrTable
		char* name = malloc(sizeof(char)*75);
		int n = 0;
		int j = addrStrTable + shdr[i].sh_name;

		while (fileBytes[j] != 0x00)
		{

			name[n] = fileBytes[j];
			j++;
			n++;

		}
		name[n]=0;


		if(isVerbose == 1)
			printf("  [%2d] %-25s%-12s%08d\t%06x\t%06x\t%02x\t%-1.04s\t%d\t%d\t%d\n",i,name,nom_type(shdr[i].sh_type),shdr[i].sh_addr,shdr[i].sh_offset,shdr[i].sh_size,shdr[i].sh_entsize,nom_flags(shdr[i].sh_flags),shdr[i].sh_link,shdr[i].sh_info,shdr[i].sh_addralign);
	}
	if(isVerbose == 1)
		printf (("Clé des fanions:\n W (écriture), A (allocation), X (exécution), U (Valeur Inconnue)\n"));

}

void readSection(int numsection,char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr){
    int i;
    char* name = malloc(sizeof(char)*75);
    unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
    int addrStrTable = shdr[header.e_shstrndx].sh_offset; // Récupération de l'adresse de la string table
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
    int size = shdr[numsection].sh_size; // Taille de la section
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

void readSymTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr){
    printf("\n");

    printf("  Num:    Valeur Tail   Type     Lien   Ndx Nom\n");

    unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
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
        printf("   %2d:",numSymb);
        numSymb++;
        /* Lecture de st_Name */

        int first = fileBytes[k];
        int second = fileBytes[k+1];
        int third = fileBytes[k+2];
        int fourth = fileBytes[k+3];
        int name = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;

        int addrStrName = addrStrTable+name;
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
        int value = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        printf("  %08x",value);

        k+=4;

        first = fileBytes[k];
        second = fileBytes[k+1];
        third = fileBytes[k+2];
        fourth = fileBytes[k+3];
        /* Valeur du symbol */
        int size = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        printf("   %2d",size);

        k+=4;

        int info = fileBytes[k];
        int bind = ELF32_ST_BIND(info);
        int type = ELF32_ST_TYPE(info);
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

        k++;

        int other = fileBytes[k];

        k++;

        first = fileBytes[k];
        second = fileBytes[k+1];
        int shndx = first + second *16*16;
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
        k+=2;
    }
    printf("\n");
}


int main(int argc, char * argv[]){
    /* Lecture du header */
    Elf32_Ehdr header = readHeader(argv[1],0);
    /* Lecture des headers de sections */
    Elf32_Shdr shdr[header.e_shnum];
    readSheader(argv[1],header,shdr,0);
    /* Lecture d'une section (contenu) */
    //readSection(18,argv[1],header,shdr);
    /* Lecture de la table des symboles */
    readSymTable(argv[1],header,shdr);

    return 0;
}

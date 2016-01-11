#include "elfsectionheader.h"


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
	case 0x70000001:		return "ARM_EXITDX";
	case 0x70000002:		return "ARM_PREEMPTMAP";
	case 0x70000003:		return "ARM_ATTRIBUTES";
	case 0x70000004:		return "ARM_DEBUGOVERLAY";
	case 0x70000005:		return "ARM_OVERLAYSECTION";
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

void readSheader(char * filePath, Elf32_Ehdr header,Elf32_Shdr* shdr,int isVerbose) {
    if(isVerbose == 1)
	   printf("Lecture des headers de sections\n");

	int i;
	unsigned char* fileBytes = readFileBytes(filePath);
	unsigned int addrStrTable;

    int isBigEndian = header.e_ident[EI_DATA]-1;



	for(i=0;i<header.e_shnum;i++){
		int j = (unsigned int)header.e_shoff+i*(unsigned int)header.e_shentsize;
		/* Lecture des indices de nom de section */
		unsigned int first = fileBytes[j];
        unsigned int second = fileBytes[j+1];
        unsigned int third = fileBytes[j+2];
        unsigned int fourth = fileBytes[j+3];
		unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;

		shdr[i].sh_name = sum;

		j+=4;
		/* Lecture de la définition de la sémantique de la section */
		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
        if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_type = sum;

		j+=4;

		/* Lecture des flags */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_flags = sum;

		j+=4;

		/* Lecture de l'adresse à laquelle le premier octect de la section doit se trouver, sinon 0 */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_addr = sum;

		j+=4;

		/* Lecture du décalage du premier octect de la section par rapport au début du fichier */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
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
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_size = sum;



		j+=4;

		/* Lecture du lien vers un indice de la table des entêtes de section*/

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_link = sum;

		j+=4;

		/* Lecture des informations complémentaires */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_info = sum;

		j+=4;

		/* Lecture de addralign */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
		shdr[i].sh_addralign = sum;

		j+=4;

		/* Lecture de entsize */

		first = fileBytes[j];
        second = fileBytes[j+1];
        third = fileBytes[j+2];
        fourth = fileBytes[j+3];
		if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
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

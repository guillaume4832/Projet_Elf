#include "elfdeleterel.h"
#include <string.h>


char* delRelTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr){



	char* nomfichier = malloc(sizeof(char)*75);
	unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
	int size = -1;
	int i,k;
	int compteur = 0;
	unsigned char poidsFort, poidsFaible,poidsFort2, poidsFaible2,poidsMoyen1,poidsMoyen2;
	int isBigEndian = header.e_ident[EI_DATA]-1;
	for(i=0;i<header.e_shnum;i++) {

		if(shdr[i].sh_type != 9) {


		}
		else {
			if(size == -1){
				size = shdr[i].sh_offset -3;
				
			}
			compteur++;
		}
	}
	int new_shnum = header.e_shnum-compteur;
	int new_shstrndx = header.e_shstrndx-compteur;

	if (is_big_endian) {
		poidsFort = new_shnum >> 8;
		poidsFaible = (new_shnum << 8) >> 8;
		poidsFort2 = new_shstrndx >> 8;
		poidsFaible2 = (new_shstrndx << 8) >> 8;
	}
	else{
		poidsFort = (new_shnum << 8) >> 8;
		poidsFaible = new_shnum >> 8;
		poidsFort2 = (new_shstrndx << 8) >> 8;
		poidsFaible2 = new_shstrndx >> 8;
	}

	if (isBigEndian) {
		fileBytes[48] = poidsFort;
		fileBytes[49] = poidsFaible;
		fileBytes[50] = poidsFort2;
		fileBytes[51] = poidsFaible2;
	}
	else {
		fileBytes[48] = poidsFaible;
		fileBytes[49] = poidsFort;
		fileBytes[51] = poidsFort2;
		fileBytes[50] = poidsFaible2;
	}

	// Mise en forme du nom de fichier de sortie
	
	char* chemin = filePath; // Récupération du chemin du fichier
	chemin[strlen(chemin)-2]= 0; // Suppression de la partie ".o"
	strcat(chemin, "_DELREL.o"); // Ajout au nom de fichier "_DELREL.o"



	void * file_to_write = fopen(chemin, "w");

    if(file_to_write != NULL){
        int i = 0;
		int j;
		int boolean = 1;
		int compteurcompteur = 0;
		int compteur40 = 0;
		int symtabOk = 0;
		int symtabOk2 = 0;
		int strtabOk = 0;
        for(i;i<size;i++){

            if(i>=header.e_shoff) {
				compteurcompteur++;
				if(compteurcompteur==25 && shdr[compteur40].sh_link != 0 && shdr[compteur40].sh_type != 9 && symtabOk == 0){
					symtabOk = 1;
					unsigned int newval = shdr[compteur40].sh_link - compteur;
					if (is_big_endian) {
						poidsFort = newval >> 24;
						poidsFaible = (newval << 24) >> 24;
						poidsMoyen1 = (newval << 8) >> 24;
						poidsMoyen2 = (newval << 16) >> 24;
					}
					else{
						poidsFort = (newval << 24) >> 24;
						poidsFaible = newval >> 24;
						poidsMoyen1 = (newval << 16) >> 24;
						poidsMoyen2 = (newval << 8) >> 24;
					}


					if (isBigEndian) {
						fileBytes[i] = poidsFort;
						fileBytes[i+1] = poidsMoyen1;
						fileBytes[i+2] = poidsMoyen2;
						fileBytes[i+3] = poidsFaible;
					}
					else {
						fileBytes[i] = poidsFaible;
						fileBytes[i+1] = poidsMoyen2;
						fileBytes[i+2] = poidsMoyen1;
						fileBytes[i+3] = poidsFort;
					}
				}
				if(compteurcompteur == 17 && shdr[compteur40].sh_link != 0 && shdr[compteur40].sh_type != 9 && symtabOk2 == 0){
					symtabOk2 = 1;
					unsigned int newval = shdr[compteur40].sh_offset - header.e_shentsize * compteur;
					if (is_big_endian) {
						poidsFort = newval >> 24;
						poidsFaible = (newval << 24) >> 24;
						poidsMoyen1 = (newval << 8) >> 24;
						poidsMoyen2 = (newval << 16) >> 24;
					}
					else{
						poidsFort = (newval << 24) >> 24;
						poidsFaible = newval >> 24;
						poidsMoyen1 = (newval << 16) >> 24;
						poidsMoyen2 = (newval << 8) >> 24;
					}


					if (isBigEndian) {
						fileBytes[i] = poidsFort;
						fileBytes[i+1] = poidsMoyen1;
						fileBytes[i+2] = poidsMoyen2;
						fileBytes[i+3] = poidsFaible;
					}
					else {
						fileBytes[i] = poidsFaible;
						fileBytes[i+1] = poidsMoyen2;
						fileBytes[i+2] = poidsMoyen1;
						fileBytes[i+3] = poidsFort;
					}
				}
				if(compteurcompteur == 17 && shdr[compteur40].sh_type == 3 && symtabOk == 1 && strtabOk == 0)
				{
					strtabOk = 1;
					unsigned int newval = shdr[compteur40].sh_offset - header.e_shentsize * compteur;
					if (is_big_endian) {
						poidsFort = newval >> 24;
						poidsFaible = (newval << 24) >> 24;
						poidsMoyen1 = (newval << 8) >> 24;
						poidsMoyen2 = (newval << 16) >> 24;
					}
					else{
						poidsFort = (newval << 24) >> 24;
						poidsFaible = newval >> 24;
						poidsMoyen1 = (newval << 16) >> 24;
						poidsMoyen2 = (newval << 8) >> 24;
					}


					if (isBigEndian) {
						fileBytes[i] = poidsFort;
						fileBytes[i+1] = poidsMoyen1;
						fileBytes[i+2] = poidsMoyen2;
						fileBytes[i+3] = poidsFaible;
					}
					else {
						fileBytes[i] = poidsFaible;
						fileBytes[i+1] = poidsMoyen2;
						fileBytes[i+2] = poidsMoyen1;
						fileBytes[i+3] = poidsFort;
					}
				}
				if(compteurcompteur==40){
					compteurcompteur=0;
					compteur40++;
				}

				if(shdr[compteur40].sh_type == 9) {
					boolean = 0;
				}

			}
			if(boolean){
				fwrite(&fileBytes[i], 1, sizeof(fileBytes[i]), file_to_write);
			}
			boolean = 1;
        }
        fclose(file_to_write);

    }
    else{
        fclose(file_to_write);
    }

return chemin;
}

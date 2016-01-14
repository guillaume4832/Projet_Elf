#include "elfrelocateSymb.h"
# define ELF_RELOC_ERR -1
# define DO_ARM_ABS32(S, A, T)	(S + A) | T
# define DO_ARM_ABS16(S, A)	(S + A)
# define DO_ARM_ABS12(S, A)	(S + A)
# define DO_ARM_ABS8(S, A)	(S + A)
# define DO_ARM_CALL(S, A, T, P)	((S + A) | T) - P
# define DO_ARM_JUMP24(S, A, T, P)	((S + A) | T) - P

char *elfrelocatesymb(char * filePath, Elf32_Ehdr header,Elf32_Ehdr headerNew, Elf32_Rel **rel, Elf32_Shdr *shdr,Elf32_Shdr *shdrNew, Elf32_Sym *symOLD, Elf32_Sym *sym) {

	unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier	
	int compteur = 1;
	int symval = 0;
	int symndx = 0;
	int i,j,k,T,first,second,third,fourth,sum, ref, addresse, symtabTaille,temp;
	int isBigEndian = header.e_ident[EI_DATA]-1;
	unsigned int newval;
	unsigned char poidsFort, poidsFaible,poidsFort2, poidsFaible2,poidsMoyen1,poidsMoyen2;

	for(k=0;k<header.e_shnum;k++) {
			if (shdr[k].sh_type == 2) {
				symtabTaille = shdr[k].sh_info;
				
			}	
	}
	
	for(i=0;i<header.e_shnum;i++) {
		
		if(shdr[i].sh_type == 9) {
			j = 0;
			if(shdr[i].sh_entsize != 0) {
			for(j=0;j<(shdr[i].sh_size/shdr[i].sh_entsize);j++) {
			
		//*ref = shdr[i].sh_offset + rel[compteur][j].r_offset;
			

		if(ELF32_R_SYM(rel[compteur][j].r_info) != SHN_UNDEF) {

		
		symval = sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_value;
		symndx = sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_shndx;
		
		if(symval == ELF_RELOC_ERR) { 
			printf("Erreur\n");
		}
		}
		
		
		

		switch(ELF32_R_TYPE(rel[compteur][j].r_info)) {
		
		case R_ARM_NONE:
			// Pas de réimplantation
			break;

		case R_ARM_ABS32:
			T = 0;
			first = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset];
			second = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+1];
			third = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+2];
			fourth = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+3];
			if(isBigEndian == 0)
		        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		    else
		        sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;

			newval = DO_ARM_ABS32(symval,sum,T);
			
			break;

		case R_ARM_ABS16:

			first = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset];
			second = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+1];
			if(isBigEndian == 0)
		        sum = first + second *16*16;
		    else
		        sum = first *16*16*16*16*16*16 + second * 16*16*16*16;
			newval = DO_ARM_ABS16(symval,sum);
			break;

			/*case R_ARM_ABS12:
			newval = DO_ARM_ABS12(symval, shdr[symndx].sh_offset + rel[compteur][j].r_offset);
			newval = ABS(newval) & 0xFFF;
			break;*/

			case R_ARM_ABS8:

			first = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset];
			if(isBigEndian == 0)
		        sum = first;
		    else
		        sum = first *16*16*16*16*16*16;
			newval = DO_ARM_ABS8(symval,sum);

			break;
			
			case R_ARM_JUMP24:
			if(ELF32_ST_TYPE(sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_info) == STT_FUNC) {
				T=1;
		}
			else{
				T=0;
			}

			first = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset];
			second = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+1];
			third = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+2];
			fourth = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+3];
			if(isBigEndian == 0)
		        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		    else
		        sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
			temp=sum & 0xFF000000;
			sum = (sum & 0x00FFFFFF) << 2;
			newval = DO_ARM_JUMP24(symval,sum,T,shdr[symndx].sh_offset + rel[compteur][j].r_offset);
			newval = (newval & 0x03FFFFFE) >> 2;
			newval = (newval & 0x00FFFFFF) | temp;
			break;

			case R_ARM_CALL:
			if(ELF32_ST_TYPE(sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_info) == STT_FUNC) {
				T=1;
			}
			else{
				T=0;
			}

			first = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset];
			second = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+1];
			third = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+2];
			fourth = fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+3];
			if(isBigEndian == 0)
		        sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
		    else
		        sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
			temp=sum & 0xFF000000;
			sum = (sum & 0x00FFFFFF) << 2;
			newval = DO_ARM_CALL(symval,sum,T,shdr[symndx].sh_offset + rel[compteur][j].r_offset);
			newval = (newval & 0x03FFFFFE) >> 2;
			newval = (newval & 0x00FFFFFF) | temp;
			break;
			

		default:

			printf("Type de réimplantation non supportée (%d).\n", ELF32_R_TYPE(rel[compteur][j].r_info));			
			}
		
			
			if (is_big_endian) {
						poidsFort = (newval >> 24);
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
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset] = poidsFort;
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+1] = poidsMoyen1;
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+2] = poidsMoyen2;
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+3] = poidsFaible;
			}
			else {
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset] = poidsFaible;
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+1] = poidsMoyen2;
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+2] = poidsMoyen1;
						fileBytes[shdr[symndx].sh_offset + rel[compteur][j].r_offset+3] = poidsFort;
					}

			} 
			compteur++;
			}
		}
	} 
	int indice;
	int indice2=0;
	int size=-1;
	int max=1;
	int taille=-1;
	for(indice=0;indice<headerNew.e_shnum;indice++) {
		if(shdrNew[indice].sh_offset>max){
			max = shdrNew[indice].sh_offset;
			taille = shdrNew[indice].sh_size;
		}
	}
	size = max + taille;
	
	char* chemin = filePath; // Récupération du chemin du fichier
	chemin[strlen(chemin)-2]= 0; // Suppression de la partie ".o"
	strcat(chemin, "_RELOC_SYMB.o"); // Ajout au nom de fichier "_DELREL.o"


	void * file_to_write = fopen(chemin, "w");
    if(file_to_write != NULL){
	for(indice2;indice2<size;indice2++){
	fwrite(&fileBytes[indice2], 1, sizeof(fileBytes[indice2]), file_to_write);
	}

	}
	fclose(file_to_write);
	return chemin;

}

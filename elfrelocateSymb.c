#include "elfrelocateSymb.h"
# define ELF_RELOC_ERR -1
# define DO_386_32(S, A)	((S) + (A))
# define DO_386_PC32(S, A, P)	((S) + (A) - (P))


int elfrelocatesymb(char * filePath, Elf32_Ehdr header, Elf32_Rel **rel, Elf32_Shdr *shdr, Elf32_Sym *sym) {
			
	unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
	int compteur = 0;
	int symval = 0;
	int addresse;
	int ref;
	int i;
	int j;
	int k;
	int symtabTaille;
	int isBigEndian = header.e_ident[EI_DATA]-1;
	int newval;
	unsigned char poidsFort, poidsFaible,poidsFort2, poidsFaible2,poidsMoyen1,poidsMoyen2;

	for(k=0;k<header.e_shnum;k++) {
			if (shdr[i].sh_type == 2) {
				symtabTaille = shdr[i].sh_info;
			}	
	}

	for(i=0;i<header.e_shnum;i++) {

		if(shdr[i].sh_type != 9) {
			j = 0;
			while((rel[compteur][j].r_offset != 0)) {
		//*ref = shdr[i].sh_offset + rel[compteur][j].r_offset;
			

		if(ELF32_R_SYM(rel[compteur][j].r_info) != SHN_UNDEF) {

		
		symval = sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_value;
		
		
		if(symval == ELF_RELOC_ERR) { 
			return ELF_RELOC_ERR;
		}
		}
		
		switch(ELF32_R_TYPE(rel[compteur][j].r_info)) {
		case R_386_NONE:
			// Pas de réimplantation
			break;
		case R_386_32:
			newval = DO_386_32(symval, fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset]);
			break;
		case R_386_PC32:
			newval = DO_386_PC32(symval, fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset], shdr[i].sh_offset);
			break;
		default:

			printf("Type de réimplantation non supportée (%d).\n", ELF32_R_TYPE(rel[compteur][j].r_info));
			return ELF_RELOC_ERR;
		}
			
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
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset] = poidsFort;
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset+1] = poidsMoyen1;
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset+2] = poidsMoyen2;
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset+3] = poidsFaible;
			}
			else {
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset] = poidsFaible;
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset+1] = poidsMoyen2;
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset+2] = poidsMoyen1;
						fileBytes[shdr[i].sh_offset + rel[compteur][j].r_offset+3] = poidsFort;
					}
			

		compteur++;
		j++;
			}
		
		}
	}
	int indice;
	int indice2;
	int size=-1;
	for(indice=0;indice<header.e_shnum;indice++) {
		if(shdr[indice].sh_type == 9) {
			size = shdr[indice].sh_offset - 3;
		}
	}
	
	void * file_to_write = fopen(filePath, "w");
    if(file_to_write != NULL){
	for(indice2;indice2<size;indice2++){
	fwrite(&fileBytes[indice2], 1, sizeof(fileBytes[indice2]), file_to_write);
	}

	}
	fclose(file_to_write);
	





	return symval;
}

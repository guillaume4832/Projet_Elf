#include "elfrelocateSymb.h"

void elfmodifsymb(char * filePath, Elf32_Ehdr headerOLD,Elf32_Ehdr header, Elf32_Shdr *shdrOLD,Elf32_Shdr *shdr, Elf32_Sym *sym) {

unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier

int indexsymtab;
int k,i,j;
int compteur = 0;
int compteur2 = 0;
unsigned int newval,newvalAddr;
int isBigEndian = header.e_ident[EI_DATA]-1;
unsigned char poidsFort, poidsFaible,poidsFort2, poidsFaible2,poidsMoyen1,poidsMoyen2;
int indextab[header.e_shnum];

for(k=0;k<headerOLD.e_shnum;k++) {
	if (shdr[k].sh_type == 2) {
		indexsymtab = k;				
	}
	if(shdrOLD[k].sh_type == 9) {
	compteur++;
	}
	indextab[k]=compteur;
}



for(j=0;j<(shdr[indexsymtab].sh_size/shdr[indexsymtab].sh_entsize);j++){

	newval = sym[j+1].st_shndx - indextab[sym[j+1].st_shndx];


	if (is_big_endian) {
		poidsFort = newval >> 8;
		poidsFaible = (newval << 8) >> 8;
	}
	else{
		poidsFort = (newval << 8) >> 8;
		poidsFaible = newval >> 8;
	}

	if (isBigEndian) {
		fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+14] = poidsFort;

		fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+1+14] = poidsFaible;
		
	}
	else {
		fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+14] = poidsFaible;
		fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+1+14] = poidsFort;
	}

	newvalAddr = shdr[sym[j+1].st_shndx].sh_offset + sym[j+1].st_value;

	if (is_big_endian) {
						poidsFort = newvalAddr >> 24;
						poidsFaible = (newvalAddr << 24) >> 24;
						poidsMoyen1 = (newvalAddr << 8) >> 24;
						poidsMoyen2 = (newvalAddr << 16) >> 24;
					}
					else{
						poidsFort = (newvalAddr << 24) >> 24;
						poidsFaible = newvalAddr >> 24;
						poidsMoyen1 = (newvalAddr << 16) >> 24;
						poidsMoyen2 = (newvalAddr << 8) >> 24;
					}


					if (isBigEndian) {
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4] = poidsFort;
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4+1] = poidsMoyen1;
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4+2] = poidsMoyen2;
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4+3] = poidsFaible;
					}
					else {
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4] = poidsFaible;
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4+1] = poidsMoyen2;
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4+2] = poidsMoyen1;
						fileBytes[shdr[indexsymtab].sh_offset+(shdr[indexsymtab].sh_entsize*j)+4+3] = poidsFort;
					}


}

	int indice;
	int indice2=0;
	int size=-1;
	int max=1;
	int taille=-1;
	for(indice=0;indice<headerOLD.e_shnum;indice++) {
		if(shdrOLD[indice].sh_offset>max){
			max = shdrOLD[indice].sh_offset;
			taille = shdrOLD[indice].sh_size;
		}
	}
	size = max + taille;

	
	void * file_to_write = fopen("toto2.o", "w");
    if(file_to_write != NULL){
	for(indice2;indice2<size;indice2++){
	fwrite(&fileBytes[indice2], 1, sizeof(fileBytes[indice2]), file_to_write);
	}

}

}

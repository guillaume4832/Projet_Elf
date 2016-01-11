#include "elfdeleterel.h"


void delRelTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr){

	

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
	printf("COUCOU SIZE : %d\n", size);
	int new_shnum = header.e_shnum-compteur;
	int new_shstrndx = header.e_shstrndx-compteur;
	
	if (is_big_endian) {
		printf("CPT : %d\n",new_shnum);
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

	
	


	void * file_to_write = fopen("pouet.o", "w");
    if(file_to_write != NULL){
        int i = 0;
		int j;
		int boolean = 1;
		int compteurcompteur = 0;
		int compteur40 = 0;
		int symtabOk = 0;
        for(i;i<size;i++){
            
            if(i>=header.e_shoff) {
				compteurcompteur++;
				if(compteurcompteur==25 && shdr[compteur40].sh_link != 0 && shdr[compteur40].sh_type != 9 && symtabOk == 0){
					symtabOk = 1;
					unsigned int newval = shdr[compteur40].sh_link - compteur;
					printf("NEWVAL : %d\n", newval);
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
						printf("Pfo : %d ,Pm1 : %d,Pm2 : %d, Pfa : %d \n", poidsFort, poidsMoyen1, poidsMoyen2, poidsFaible);
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



}

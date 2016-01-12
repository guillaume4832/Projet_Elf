#include "elfrelocation.h"


void readRelTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr, Elf32_Rel* rel[],Elf32_Sym* sym,int isVerbose){

	unsigned char* fileBytes = readFileBytes(filePath); // Contenu du fichier
	int i;
	int j;
	int k;
	int compteur = 0;
	char* type = malloc(sizeof(char)*15);
	int addrStrTable = shdr[header.e_shstrndx].sh_offset;
	int isBigEndian = header.e_ident[EI_DATA]-1;
	for(i=0;i<header.e_shnum;i++) {

			if(shdr[i].sh_type == 9) {
			compteur++;


				char* name = malloc(sizeof(char)*75);
				int n = 0;
				int l = addrStrTable + shdr[i].sh_name;

				while (fileBytes[l] != 0x00)
				{

					name[n] = fileBytes[l];
					l++;
					n++;

				}
				name[n]=0;

				if(isVerbose == 1){
					printf("\nSection de relocalisation '%s' à l'adresse de décalage %x contient %d entrée(s):\n",name,shdr[i].sh_offset,shdr[i].sh_size/(4*2));
					printf("Décalage   Info      Type          Val.-sym  Noms-symboles\n");
				}
				k = (unsigned int)shdr[i].sh_offset;
				rel[compteur]=malloc(sizeof(Elf32_Rel)*shdr[i].sh_size/(4*2));
				for(j=0;j<(shdr[i].sh_size/(4*2));j++) {


					unsigned int first = fileBytes[k];
        			unsigned int second = fileBytes[k+1];
        			unsigned int third = fileBytes[k+2];
        			unsigned int fourth = fileBytes[k+3];
        			unsigned int sum;
					if(isBigEndian == 0)
			            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
			        else
			            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;


					rel[compteur][j].r_offset = sum;

					k+=4;

					first = fileBytes[k];
        			second = fileBytes[k+1];
        			third = fileBytes[k+2];
        			fourth = fileBytes[k+3];
					if(isBigEndian == 0)
			            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
			        else
			            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;

					rel[compteur][j].r_info = sum;
					k+=4;

					 switch(ELF32_R_TYPE(rel[compteur][j].r_info)){
         				 case 0:
           				 	type ="R_386_NONE";
           				 	break;
						 case 1:
						 	type="R_386_32";
						 	break;
						 case 2:
							type="R_386_PC32";
							break;
						 case 3:
							type="R_386_GOT32";
							break;
						 case 4:
							type="R_386_PLT32";
							break;
						 case 5:
							type="R_386_COPY";
							break;
						 case 6:
							type="R_386_GLOB_DAT";
							break;
						 case 7:
							type="R_386_JMP_SLOT";
							break;
						 case 8:
							type="R_386_RELATIVE";
							break;
						 case 9:
							type="R_386_GOTOFF";
							break;
						 case 10:
							type="R_386_GOTPC";
							break;
						 default:
							type="UNKNOWN";
					}



				int addrStrName = sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_name;

				char * nameString = malloc(sizeof(char)*75);
       			int w = 0;

        		while(fileBytes[addrStrName] != 0){
        		    nameString[w] = fileBytes[addrStrName];
         			addrStrName++;
        		    w++;
        		}
				 nameString[w] = 0;

					char* nomSymb = nameString;

					if (nomSymb[0] == 0) {



						char* name2 = malloc(sizeof(char)*75);
						int n = 0;
						int l = addrStrTable + shdr[sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_shndx].sh_name;


						while (fileBytes[l] != 0)
						{

							name2[n] = fileBytes[l];
							l++;
							n++;

						}
						name2[n]=0;
						nomSymb=name2;
					}

					int val = sym[ELF32_R_SYM(rel[compteur][j].r_info)+1].st_value;

					if(isVerbose == 1)
						printf("%08x   %08x  %-12s  %08x  %s\n",rel[compteur][j].r_offset,rel[compteur][j].r_info,type,val,nomSymb);
				}


			}
			
	}
if(isVerbose == 1)
	printf("\n");
}

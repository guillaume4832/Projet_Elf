#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "elfheader.h"
#include "elfsectionheader.h"
#include "elfsection.h"
#include "elfsymtable.h"
#include "elfrelocation.h"

void main(int argc, char * argv[]){
  // Lecture du header
	Elf32_Ehdr header = readHeader(argv[1],0);
  // Lecture des headers de sections
	Elf32_Shdr shdr[header.e_shnum];
	readSheader(argv[1],header,shdr,0);
  // Lecture d'une section
  // readSection(18,argv[1],header,shdr);
  // Lecture de la table des symboles
	
	int j = getIndSectionSymtab(header,shdr);
	Elf32_Sym sym[(shdr[j].sh_size)/(4+4+4+1+1+2)];
	readSymTable(argv[1],header,shdr,sym);
   

	int i;
	int count = 0;
	for(i=0;i<header.e_shnum;i++) {
		if (shdr[i].sh_type == 9 || 4) {
			count = count+1;
		}
	}

	if (count != 0) {
    Elf32_Rel* rel[count];
	readRelTable(argv[1],header,shdr,rel,sym,1);
	}

}

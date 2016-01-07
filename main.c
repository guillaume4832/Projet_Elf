#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "elfheader.h"
#include "elfsectionheader.h"
#include "elfsection.h"
#include "elfsymtable.h"

void main(int argc, char * argv[]){
  // Lecture du header
  Elf32_Ehdr header = readHeader(argv[1],1);
  // Lecture des headers de sections
  Elf32_Shdr shdr[header.e_shnum];
  readSheader(argv[1],header,shdr,1);
  // Lecture d'une section
  // readSection(18,argv[1],header,shdr);
  // Lecture de la table des symboles
   readSymTable(argv[1],header,shdr);


}

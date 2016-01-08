#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "filereader.h"

int getIndSectionSymtab(Elf32_Ehdr header,Elf32_Shdr* shdr);

void readSymTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr,Elf32_Sym* sym);



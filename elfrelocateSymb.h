#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "filereader.h"

void elfrelocatesymb(char * filePath, Elf32_Ehdr header, Elf32_Rel **rel, Elf32_Shdr *shdr,Elf32_Sym *symOLD,Elf32_Sym *sym);

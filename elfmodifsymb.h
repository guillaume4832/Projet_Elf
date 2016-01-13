
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "filereader.h"

void elfmodifsymb(char * filePath, Elf32_Ehdr header,Elf32_Ehdr headerOLD, Elf32_Shdr *shdr,Elf32_Shdr *shdrNEW, Elf32_Sym *sym);

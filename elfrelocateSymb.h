#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "filereader.h"

char* elfrelocatesymb(char * filePath, Elf32_Ehdr header,Elf32_Ehdr headerNew, Elf32_Rel **rel, Elf32_Shdr *shdr,Elf32_Shdr *shdrNew,Elf32_Sym *symOLD,Elf32_Sym *sym);

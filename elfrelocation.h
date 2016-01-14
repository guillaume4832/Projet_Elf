#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "filereader.h"


void readRelTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr, Elf32_Rel* rel[],Elf32_Sym* sym,int isVerbose);

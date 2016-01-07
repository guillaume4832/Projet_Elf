#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "filereader.h"

char* nom_type(Elf32_Word sh_type);
char* nom_flags(unsigned int flags);
void readSheader(char * filePath, Elf32_Ehdr header,Elf32_Shdr* shdr,int isVerbose);


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "filereader.h"

void delRelTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr);

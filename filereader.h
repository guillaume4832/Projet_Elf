#ifndef __FILE_READER__
#define __FILE_READER__
#include <stdio.h>
#include <stdlib.h>

char* readFileBytes(const char *name);
int writeFileBytes(char* fileBytes, char* fileName, int size);
#endif

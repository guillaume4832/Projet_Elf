#ifndef __FILE_READER__
#define __FILE_READER__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* readFileBytes(const char *name);
int writeFileBytes(char* fileBytes, char* fileName, int size);
int is_big_endian(void);
#endif

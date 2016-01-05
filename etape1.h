#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include <elf.h>

bool elf_check_file(Elf32_Ehdr *hdr);


void print_elf_header(Elf32_Ehdr elf_header);

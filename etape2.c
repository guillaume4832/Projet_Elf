#include "etape2.h"



void read_elf_header(int32_t fd, Elf32_Ehdr *elf_header)
{
	assert(elf_header != NULL);
	assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
	assert(read(fd, (void *)elf_header, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
}


void print_elf_section_table(Elf32_Ehdr elf_header, FILE* f)
{
//Charge tableau section header//////////////////////////////////////////////
fseek(f,elf_header.shoff,SEEK_SET);
Elf32_Shdr tabhdr[elf_header.shnum];

	for(int i=0;i<elf_header.e_shnum;i++)
	{
	fread(tabhdr[i],elf_header.e_shentsize,1,f);
///////////////////////////////////////////////////////////////////////


		/* Numéro de la section */
		printf("\nNuméro de la section\t= ");
		printf("%i", i+1);

		/* Nom de la section */
		printf("\nNom de la section\t= ");
		printf("%s", tabhdr[i].sh_name);

		/* Taille de la section*/

		printf("\nTaille de la section\t= ");
		printf("0x%x", tabhdr[i].sh_size);

		/* Type de la section*/
		printf("\nType de la section\t= ");
		switch(tabhdr[i].sh_type)
		{
			case SHT_NULL:
				printf("0");
				break;

			case SHT_PROGBITS:
				printf("1");
				break;

			case SHT_SYMTAB:
				printf("2");
				break;

			case SHT_STRTAB:
				printf("3");
				break;

			case SHT_RELA:
				printf("4");
				break;

			case SHT_HASH:
				printf("5");
				break;

			case SHT_DYNAMIC:
				printf("6");
				break;

			case SHT_NOTE:
				printf("7");
				break;

			case SHT_NOBITS:
				printf("8");
				break;

			case SHT_REL:
				printf("9");
				break;

			case SHT_SHLIB:
				printf("10");
				break;

			case SHT_DYNSYM:
				printf("11");
				break;

			case SHT_LOPROC:
				printf("0x70000000");
				break;

			case SHT_HIPROC:
				printf("0x7fffffff");
				break;

			case SHT_LOUSER:
				printf("0x80000000");
				break;

			case SHT_HIUSER:
				printf("0xffffffff");
				break;

			default:
				printf("Unknown (0x%x)", tabhdr[i].sh_name);
				break;
		}

		/* Flags de la section*/
		printf("\nFlags de la section\t= ");
		switch(tabhdr[i].sh_flags)
		{
			case 0:
				printf("No Flags");
				break;

			case SHF_WRITE:
				printf("0x1");
				break;

			case SHF_ALLOC:
				printf("0x2");
				break;

			case SHF_EXECINSTR:
				printf("0x4");
				break;

			case SHF_MASKPROC:
				printf("0xf0000000");
				break;

			default:
				printf("Unknown (0x%x)", tabhdr[i].sh_flags);
				break;
		}
	
}

int main() {


int32_t fd;
Elf32_Ehdr eh;
fd = open("bitabit.o", O_RDONLY|O_SYNC);
if(fd<0) {
		printf("Error Unable to open\n");
		return 0;
	}
	read_elf_header(fd, &eh);
	if(!is_ELF(eh)) {
		return 0;
	}
	

print_elf_section_table(eh,fd);



return 0;
}

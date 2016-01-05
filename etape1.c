#include "etape1.h"

bool elf_check_file(Elf32_Ehdr *hdr) {
	if(!hdr) return false;
	printf("%x",*hdr);
	printf("%x",hdr->e_ident[1]);
	printf("%x",hdr->e_ident[2]);
	//if(htonl(hdr->e_ident[EI_MAG0]) != ELFMAG0) {
	if(hdr->e_ident[0] != 0x7f) {
	
		printf("ELF Header EI_MAG0 incorrect.\n");
		return false;
	}
	if(htonl(hdr->e_ident[EI_MAG1]) != ELFMAG1) {
		printf("ELF Header EI_MAG1 incorrect.\n");
		return false;
	}
	if(htonl(hdr->e_ident[EI_MAG2]) != ELFMAG2) {
		printf("ELF Header EI_MAG2 incorrect.\n");
		return false;
	}
	if(htonl(hdr->e_ident[EI_MAG3]) != ELFMAG3) {
		printf("ELF Header EI_MAG3 incorrect.\n");
		return false;
	}
	return true;
}

bool elf_check_supported(Elf32_Ehdr *hdr) {
	if(!elf_check_file(hdr)) {
		printf("Invalid ELF File.\n");
		return false;
	}
	if(htonl(hdr->e_ident[EI_CLASS]) != ELFCLASS32) {
		printf("Unsupported ELF File Class.\n");
		return false;
	}
	if(htonl(hdr->e_ident[EI_DATA]) != ELFDATA2LSB) {
		printf("Unsupported ELF File byte order.\n");
		return false;
	}
	if(htonl(hdr->e_machine) != EM_386) {
		printf("Unsupported ELF File target.\n");
		return false;
	}
	if(htonl(hdr->e_ident[EI_VERSION]) != EV_CURRENT) {
		printf("Unsupported ELF File version.\n");
		return false;
	}
	if(htonl(hdr->e_type) != ET_REL && htonl(hdr->e_type) != ET_EXEC) {
		printf("Unsupported ELF File type.\n");
		return false;
	}
	return true;
}

void print_elf_header(Elf32_Ehdr *elf_header)
{

	/* Storage capacity class */
	printf("Storage class\t= ");
	switch(htonl(elf_header->e_ident[EI_CLASS]))
	{
		case ELFCLASS32:
			printf("32-bit objects\n");
			break;

		case ELFCLASS64:
			printf("64-bit objects\n");
			break;

		default:
			printf("INVALID CLASS\n");
			break;
	}

	/* Data Format */
	printf("Data format\t= ");
	switch(htonl(elf_header->e_ident[EI_DATA]))
	{
		case ELFDATA2LSB:
			printf("2's complement, little endian\n");
			break;

		case ELFDATA2MSB:
			printf("2's complement, big endian\n");
			break;

		default:
			printf("INVALID Format\n");
			break;
	}

	/* OS ABI */
	printf("OS ABI\t\t= ");
	switch(htonl(elf_header->e_ident[EI_OSABI]))
	{
		case ELFOSABI_SYSV:
			printf("UNIX System V ABI\n");
			break;

		case ELFOSABI_HPUX:
			printf("HP-UX\n");
			break;

		case ELFOSABI_NETBSD:
			printf("NetBSD\n");
			break;

		case ELFOSABI_LINUX:
			printf("Linux\n");
			break;

		case ELFOSABI_SOLARIS:
			printf("Sun Solaris\n");
			break;

		case ELFOSABI_AIX:
			printf("IBM AIX\n");
			break;

		case ELFOSABI_IRIX:
			printf("SGI Irix\n");
			break;

		case ELFOSABI_FREEBSD:
			printf("FreeBSD\n");
			break;

		case ELFOSABI_TRU64:
			printf("Compaq TRU64 UNIX\n");
			break;

		case ELFOSABI_MODESTO:
			printf("Novell Modesto\n");
			break;

		case ELFOSABI_OPENBSD:
			printf("OpenBSD\n");
			break;

		case ELFOSABI_ARM_AEABI:
			printf("ARM EABI\n");
			break;

		case ELFOSABI_ARM:
			printf("ARM\n");
			break;

		case ELFOSABI_STANDALONE:
			printf("Standalone (embedded) app\n");
			break;

		default:
			printf("Unknown (0x%x)\n", htonl(elf_header->e_ident[EI_OSABI]));
			break;
	}

	/* ELF filetype */
	printf("Filetype \t= ");
	switch(htonl(elf_header->e_type))
	{
		case ET_NONE:
			printf("N/A (0x0)\n");
			break;

		case ET_REL:
			printf("Relocatable\n");
			break;

		case ET_EXEC:
			printf("Executable\n");
			break;

		case ET_DYN:
			printf("Shared Object\n");
			break;
		default:
			printf("Unknown (0x%x)\n", elf_header->e_type);
			break;
	}

	/* ELF Machine-id */
	printf("Machine\t\t= ");
	switch(htonl(elf_header->e_machine))
	{
		case EM_NONE:
			printf("None (0x0)\n");
			break;

		case EM_386:
			printf("INTEL x86 (0x%x)\n", EM_386);
			break;

		case EM_ARM:
			printf("ARM (0x%x)\n", EM_ARM);
			break;
		default:
			printf("Machine\t= 0x%x\n", htonl(elf_header->e_machine));
			break;
	}

	/* Entry point */
	printf("Entry point\t= 0x%08x\n", htonl(elf_header->e_entry));

	/* ELF header size in bytes */
	printf("ELF header size\t= 0x%08x\n", htonl(elf_header->e_ehsize));

	/* Program Header */
	printf("\nProgram Header\t= ");
	printf("0x%08x\n", htonl(elf_header->e_phoff));		/* start */
	printf("\t\t  %d entries\n", htonl(elf_header->e_phnum));	/* num entry */
	printf("\t\t  %d bytes\n", htonl(elf_header->e_phentsize));	/* size/entry */

	/* Section header starts at */
	printf("\nSection Header\t= ");
	printf("0x%08x\n", htonl(elf_header->e_shoff));		/* start */
	printf("\t\t  %d entries\n", htonl(elf_header->e_shnum));	/* num entry */
	printf("\t\t  %d bytes\n", htonl(elf_header->e_shentsize));	/* size/entry */
	printf("\t\t  0x%08x (string table offset)\n", htonl(elf_header->e_shstrndx));

	/* File flags (Machine specific)*/
	printf("\nFile flags \t= 0x%08x\n", htonl(elf_header->e_flags));

	/* ELF file flags are machine specific.
	 * INTEL implements NO flags.
	 * ARM implements a few.
	 * Add support below to parse ELF file flags on ARM
	 */
	int32_t ef = htonl(elf_header->e_flags);
	printf("\t\t  ");

	if(ef & EF_ARM_RELEXEC)
		printf(",RELEXEC ");

	if(ef & EF_ARM_HASENTRY)
		printf(",HASENTRY ");

	if(ef & EF_ARM_INTERWORK)
		printf(",INTERWORK ");

	if(ef & EF_ARM_APCS_26)
		printf(",APCS_26 ");

	if(ef & EF_ARM_APCS_FLOAT)
		printf(",APCS_FLOAT ");

	if(ef & EF_ARM_PIC)
		printf(",PIC ");

	if(ef & EF_ARM_ALIGN8)
		printf(",ALIGN8 ");

	if(ef & EF_ARM_NEW_ABI)
		printf(",NEW_ABI ");

	if(ef & EF_ARM_OLD_ABI)
		printf(",OLD_ABI ");

	if(ef & EF_ARM_SOFT_FLOAT)
		printf(",SOFT_FLOAT ");

	if(ef & EF_ARM_VFP_FLOAT)
		printf(",VFP_FLOAT ");

	if(ef & EF_ARM_MAVERICK_FLOAT)
		printf(",MAVERICK_FLOAT ");

	printf("\n");

	/* MSB of flags conatins ARM EABI version */
	printf("ARM EABI\t= Version %d\n", (ef & EF_ARM_EABIMASK)>>24);

	printf("\n");	/* End of ELF header */

}

 
void *elf_load_file(void *file) {
	Elf32_Ehdr *hdr = (Elf32_Ehdr *)file;
	if(!elf_check_supported(hdr)) {
		printf("ELF File cannot be loaded.\n");
		return NULL;
	}
	print_elf_header(hdr);
	return NULL;
}

int main() {

FILE *f = NULL;


f = fopen("bitabit.o", "rb");

elf_load_file(f);

return 0;
}

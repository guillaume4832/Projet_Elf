#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <getopt.h>

#include "elfheader.h"
#include "elfsectionheader.h"
#include "elfsection.h"
#include "elfsymtable.h"
#include "elfrelocation.h"
#include "filereader.h"

int isnumber(const char*s) {
   char* e = NULL;
   (void) strtol(s, &e, 0);
   return e != NULL && *e == (char)0;
}

void main(int argc, char * argv[]){

	//Lecture des Options

    int opt;
	char *fileName;

	struct option longopts[] = {
		{ "headerVerbose", no_argument, NULL, 'h' },
		{ "sectionHeadersVerbose", no_argument, NULL, 'S' },
		{ "sectionDetails", required_argument, NULL, 'x' },
		{ "symboles", no_argument, NULL, 's' },
        { "relocation", no_argument, NULL, 'r'},
		{ "fileName", required_argument, NULL, 'f' },
		{ NULL, 0, NULL, 0 }
	};

	int verboseHeader = 0;
	int verboseSectionH = 0;
	int verboseSymboles = 0;
	int verboseRelocation = 0;
	int sectionDetails = -1;
	int hasFile = 0;
	while ((opt = getopt_long(argc, argv, "hSx:srf:", longopts, NULL)) != -1) {

		switch(opt) {
			case 'h':
				verboseHeader = 1;
				break;
			case 'S':
				verboseSectionH = 1;
				break;
			case 'x':
				if(isnumber(optarg))
				{
					sectionDetails = atoi(optarg);
				}
				else{
					unsigned char* fileBytes = readFileBytes(fileName);
					Elf32_Ehdr header = readHeader(fileName,verboseHeader);
					Elf32_Shdr shdr[header.e_shnum];
					readSheader(fileName,header,shdr,verboseSectionH);
					int i;
					int addrStrTable = shdr[header.e_shstrndx].sh_offset;
					for(i=0;i<header.e_shnum;i++){


						// addrStrTable
						char* name = malloc(sizeof(char)*75);
						int n = 0;
						int j = addrStrTable + shdr[i].sh_name;

						while (fileBytes[j] != 0x00)
						{

							name[n] = fileBytes[j];
							j++;
							n++;

						}
						name[n]=0;
						if(strcmp(optarg,name) == 0){
							sectionDetails = i;
							break;
						}
						if(i== header.e_shnum-1)
							printf("Section inconnue (-x)\n");
					}
				}
				break;
			case 's':
				verboseSymboles = 1;
				break;
			case 'r':
				verboseRelocation = 1;
				break;
			case 'f':
				fileName = optarg;
				hasFile = 1;
				break;
			default:
				fprintf(stderr, "Unrecognized option %c\n", opt);
				exit(1);
		}
	}
	if(hasFile == 0){
		printf("Veuillez préciser un nom de fichier avec l'option -f\n-f : Nom de fichier\n-h : Lecture du header\n-S : Lecture des en-têtes de sections\n-x <Nom/Numéro de section> : Détail d'une section\n-s : Lecture de la table des symboles\n-r : Lecture de la table de relocation\n");
		exit(1);
	}
	// Lecture du header
    Elf32_Ehdr header = readHeader(fileName,verboseHeader);
	// Lecture des headers de sections
	Elf32_Shdr shdr[header.e_shnum];
	readSheader(fileName,header,shdr,verboseSectionH);
	// Lecture d'une section
	if(sectionDetails != -1)
		readSection(sectionDetails,fileName,header,shdr);
	// Lecture de la table des symboles

	int j = getIndSectionSymtab(header,shdr);
	Elf32_Sym sym[(shdr[j].sh_size)/(4+4+4+1+1+2)];
	readSymTable(fileName,header,shdr,sym,verboseSymboles);

	int i;
	int count = 0;
	for(i=0;i<header.e_shnum;i++) {
		if (shdr[i].sh_type == 9 || 4) {
			count = count+1;
		}
	}

	if (count != 0) {
    Elf32_Rel* rel[count];
	readRelTable(fileName,header,shdr,rel,sym,verboseRelocation);
	}

}

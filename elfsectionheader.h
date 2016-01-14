/**
 * \file elfsectionheader.h
 * \author Amédée Fleau - Clément Heresaz - Guillaume Chaput - Lucas Albarede - Loïc Chevalier
 * \version 1
 * \date 12 Janvier 2016
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "filereader.h"

/**
 * \fn char* nom_type(Elf32_Word sh_type)
 * \brief Table de correspondance de renommage des globales.
 * \param Elf32_Word sh_type type de fichier Elf32 reçu en entrée.
 * \return char* - Retourne le pointeur utilisant l'étiquette modifiée.
 */

char* nom_type(Elf32_Word sh_type);

/**
 * \char* nom_flags(unsigned int flags)
 * \brief Reconnaît les fanions
 * \param Elf32_Word sh_type type de fichier Elf32 reçu en entrée.
 * \return val - Retourne l'acronyme des fanions.
 */

char* nom_flags(unsigned int flags);

/**
 * \void readSheader(char * filePath, Elf32_Ehdr header,Elf32_Shdr* shdr,int isVerbose)
 * \brief Lecture du header des sections.
 * \param Elf32_Word sh_type type de fichier Elf32 reçu en entrée.
 */

void readSheader(char * filePath, Elf32_Ehdr header,Elf32_Shdr* shdr,int isVerbose);

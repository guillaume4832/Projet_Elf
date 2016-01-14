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
 * \fn int getIndSectionSymtab(Elf32_Ehdr header,Elf32_Shdr* shdr)
 * \brief Fonction permettant de retrouver l'indice de la section symtab dans la table des en-têtes.
 * \param header l'en-tête du fichier Elf définie par *filePath.
 * \param shdr le tableau des en-têtes de sections.
 * \return i l'indice correspondant à la section symtab dans les en-têtes de section.
 */

int getIndSectionSymtab(Elf32_Ehdr header,Elf32_Shdr* shdr);

/**
 * \fn void readSymTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr, Elf32_Sym* sym, int isVerbose)
 * \brief Fonction d'affichage de la table des symboles.
 * \param *filePath le chemin du fichier entrée en paramètre.
 * \param header l'en-tête du fichier Elf définie par *filePath.
 * \param shdr le tableau des en-têtes de sections.
 * \param sym le tableau des symboles.
 * \param isVerbose Permet de cacher ou voir les informations de l'en-tête.
 */

void readSymTable(char * filePath,Elf32_Ehdr header,Elf32_Shdr* shdr,Elf32_Sym* sym, int isVerbose);

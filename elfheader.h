/**
 * \file elflheader.h
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
 * \fn Elf32_Ehdr readHeader(char * filePath, int isVerbose)
 * \brief Lecture de l'en-tête ELF d'un fichier.
 * \param * filePath Fichier entré en paramètre.
 * \param isVerbose Permet de cacher ou voir les informations de l'en-tête.
 * \return header - Retourne l'objet contenant les informations de l'en-tête.
 */

Elf32_Ehdr readHeader(char * filePath, int isVerbose);

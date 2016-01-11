/**
 * \file elfheader.c
 * \brief Programme de l'en-tête.
 * \author Amédée Fleau - Clément Heresaz - Guillaume Chaput - Lucas Albarede - Loïc Chevalier
 * \version 1
 * \date 11 Janvier 2016
 *
 * Programme permettant l'affichage de l'en-tête ELF des fichiers
 *
 */

#include "elfheader.h"

/**
 * \fn Elf32_Ehdr readHeader(char * filePath, int isVerbose)
 * \brief Lecture de l'en-tête ELF d'un fichier.
 * \param * filePath Fichier entré en paramètre.
 * \param isVerbose Permet de cacher ou voir les informations de l'en-tête.
 * \return header - Retourne l'objet contenant les informations de l'en-tête.
 */

Elf32_Ehdr readHeader(char * filePath, int isVerbose){
    Elf32_Ehdr header;
    int isValid = 1;
    int is64 = 0;
    int isBigEndian = 0;
    unsigned char* fileBytes = readFileBytes(filePath);
    /* Lecture des bits 'magiques' ELF */
    if(fileBytes[0] == 127 && fileBytes[1] == 69 && fileBytes[2] == 76 && fileBytes[3] == 70){
        if(isVerbose == 1)
            printf("[*] Fichier ELF Reconnu\n");
        header.e_ident[EI_MAG0] = fileBytes[0];
        header.e_ident[EI_MAG1] = fileBytes[1];
        header.e_ident[EI_MAG2] = fileBytes[2];
        header.e_ident[EI_MAG3] = fileBytes[3];
    }
    else{
        if(isVerbose == 1)
            printf("[E] Le fichier founi n'est pas un fichier ELF\n");
        isValid = 0;
    }
    int i = 4;
    /* Lecture de la classe */
    if(isValid == 1){
        if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] Classe:                                            32 bits ELFCLASS32\n");

        }
        else if(fileBytes[i] == 2){
            if(isVerbose == 1)
                printf("[*] Classe:                                            64 bits ELFCLASS64\n");
            is64 = 1;
        }
        else{
            if(isVerbose == 1)
                printf("[E] Erreur de lecture de la classe du fichier ELF\n");
            isValid = 0;
        }
        header.e_ident[EI_CLASS] = fileBytes[i];
    }
    i++;

    /* Lecture de l'encodage des données processeur */
    if(isValid == 1){
        if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] EI_DATA:                                           ELFDATA2LSB Little endian\n");
        }
        else if(fileBytes[i] == 2){
            if(isVerbose == 1)
                printf("[*] EI_DATA :                                          ELFDATA2MSB Big endian\n");
                isBigEndian = 1;
        }
        else{
            if(isVerbose == 1)
                printf("[E] Erreur de lecture de l'encodage EI_DATA ELF\n");
            isValid = 0;
        }
        header.e_ident[EI_DATA] = fileBytes[i];
    }
    i++;

    /* Lecture de la version du header */
    if(isValid == 1){
        if(fileBytes[i] != 0){
            if(isVerbose == 1)
                printf("[*] Version du header :                                %d\n",fileBytes[i]);
        }
        else{
            if(isVerbose == 1)
                printf("[E] Version du header invalide\n");
            isValid = 0;
        }
        header.e_ident[EI_VERSION] = fileBytes[i];
    }
    i++;

    /* Lecture de l'os ABI */

    if(isValid == 1){
        if(fileBytes[i] == 0){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           Unix System V\n");
        }
        else if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           HP-UX\n");
        }
        else if(fileBytes[i] == 2){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           NetBSD\n");
        }
        else if(fileBytes[i] == 3){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           Linux\n");
        }
        else if(fileBytes[i] == 6){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           Sun Solaris\n");
        }
        else if(fileBytes[i] == 7){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           IBM AIX\n");
        }
        else if(fileBytes[i] == 8){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           SGI Irix\n");
        }
        else if(fileBytes[i] == 9){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           FreeBSD\n");
        }
        else if(fileBytes[i] == 10){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           Compaq TRU64\n");
        }
        else if(fileBytes[i] == 11){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           Novell Modesto\n");
        }
        else if(fileBytes[i] == 12){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           OpenBSD\n");
        }
        else if(fileBytes[i] == 64){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           ARL EABI\n");
        }
        else if(fileBytes[i] == 97){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           ARM\n");
        }
        else if(fileBytes[i] == 255){
            if(isVerbose == 1)
                printf("[*] OS/ABI :                                           Standalone\n");
        }
        else{
            if(isVerbose == 1){
                printf("[W] OS/ABI :                                           Inconnu\n");
                printf("%d\n",fileBytes[i]);
            }
            isValid = 0;
        }
        header.e_ident[EI_OSABI] = fileBytes[i];
    }
    i++;

    /* Lecture de la version de l'ABI */
    if(isValid == 1){
        if(isVerbose == 1)
            printf("[*] Version de l'ABI :                                 %d\n",fileBytes[i]);
        header.e_ident[EI_ABIVERSION] = fileBytes[i];
    }
    if(isBigEndian == 1)
        i=17;
    if(isBigEndian == 0)
        i=16;

    /* Lecture du type de l'objet */
    if(isValid == 1){
        if(isVerbose == 1){
            if(fileBytes[i] == 0){
                printf("[*] Pas de type de fichier\n");
            }
            else if(fileBytes[i] == 1){
                printf("[*] Objet de type :                                    Repositionable (Relocatable)\n");
            }
            else if(fileBytes[i] == 2){
                printf("[*] Objet de type :                                    Executable\n");
            }
            else if(fileBytes[i] == 3){
                printf("[*] Objet de type :                                    Partagé\n");
            }
            else if(fileBytes[i] == 4){
                printf("[*] Objet de type :                                    Core\n");
            }
            else{
                printf("[W] Type inconnu\n");
            }
        }
        header.e_type = fileBytes[i];
    }


    /* Lecture de la machine cible */

    if(isBigEndian == 1)
        i = 19;
    else
        i = 18;

    if(isValid == 1){
        if(isVerbose == 1){
            if(fileBytes[i] == 0){
                printf("[*] Aucune machine cible\n");
            }
            else if(fileBytes[i] == 2){
                printf("[*] Machine cible :                                    SPARC\n");
            }
            else if(fileBytes[i] == 3){
                printf("[*] Machine cible :                                    Intel 80386\n");
            }
            else if(fileBytes[i] == 4){
                printf("[*] Machine cible :                                    Motorola 68000\n");
            }
            else if(fileBytes[i] == 7){
                printf("[*] Machine cible :                                    Intel i860\n");
            }
            else if(fileBytes[i] == 8){
                printf("[*] Machine cible :                                    MIPS I\n");
            }
            else if(fileBytes[i] == 19){
                printf("[*] Machine cible :                                    Intel i960\n");
            }
            else if(fileBytes[i] == 20){
                printf("[*] Machine cible :                                    PowerPC\n");
            }
            else if(fileBytes[i] == 40){
                printf("[*] Machine cible :                                    ARM\n");
            }
            else if(fileBytes[i] == 50){
                printf("[*] Machine cible :                                    Intel IA64\n");
            }
            else if(fileBytes[i] == 62){
                printf("[*] Machine cible :                                    x64\n");
            }
            else{
                printf("[W] Machine cible non reconnue\n");
            }
        }
        header.e_machine = fileBytes[i];
    }

    if(isBigEndian == 1)
        i = 23;
    else
        i = 20;

    /* Lecture de la version du fichier */
    if(isValid == 1){
        if(fileBytes[i] == 0){
            if(isVerbose == 1)
                printf("[E] Version invalide\n");
            isValid = 0;
        }
        else if(fileBytes[i] == 1){
            if(isVerbose == 1)
                printf("[*] Version :                                          0x1\n");
        }
        else{
            if(isVerbose == 1)
                printf("[W] Version inconnue\n");
        }
        header.e_version = fileBytes[i];
    }
    if(isBigEndian == 0)
        i+=4;
    else
        i+=1;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;

        if(isVerbose == 1)
            printf("[*] Adresse du point d'entrée :                        0x%x\n",sum);
        header.e_entry = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
        if(isVerbose == 1)
            printf("[*] Début des en-têtes de programme :                  %d\n",sum);
        header.e_phoff = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
        if(isVerbose == 1)
            printf("[*] Début des en-têtes de section :                    %d\n",sum);
        header.e_shoff = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int third = fileBytes[i+2];
        unsigned int fourth = fileBytes[i+3];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16 + third *16*16*16*16 + fourth *16*16*16*16*16*16;
        else
            sum = first *16*16*16*16*16*16 + second * 16*16*16*16 + third*16*16 + fourth;
        if(isVerbose == 1)
            printf("[*] Fanions :                                          0x%x\n",sum);
        header.e_flags = sum;
    }
    i+=4;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16;
        else
            sum = first *16*16 + second;
        if(isVerbose == 1)
            printf("[*] Taille de cet en-tête :                            %d\n",sum);
        header.e_ehsize = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16;
        else
            sum = first *16*16 + second;
        if(isVerbose == 1)
            printf("[*] Taille de l'en-tête du programme :                 %d\n",sum);
        header.e_phentsize = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16;
        else
            sum = first *16*16 + second;
        if(isVerbose == 1)
            printf("[*] Nombre d'en-tête du programme :                    %d\n",sum);
        header.e_phnum = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16;
        else
            sum = first *16*16 + second;
        if(isVerbose == 1)
            printf("[*] Taille des en-têtes de section :                   %d\n",sum);
        header.e_shentsize = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16;
        else
            sum = first *16*16 + second;
        if(isVerbose == 1)
            printf("[*] Nombre d'en-tête de section :                      %d\n",sum);
        header.e_shnum = sum;
    }
    i+=2;

    if(isValid == 1 && is64 == 0){
        unsigned int first = fileBytes[i];
        unsigned int second = fileBytes[i+1];
        unsigned int sum;
        if(isBigEndian == 0)
            sum = first + second *16*16;
        else
            sum = first *16*16 + second;
        if(isVerbose == 1)
            printf("[*] Table d'indexes des chaînes d'en-tête de section : %d\n",sum);
        header.e_shstrndx = sum;
    }
    i+=2;


    return header;
}

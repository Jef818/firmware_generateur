# Ce code est basé sur le firmware VESC et sur le rtOS ChibiOS

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)



Le rtOS ChibiOS et un système d'exploitation temps réel open source, logiciel libre sous licence GNU GPL v3

Le firmware VESC est un programme de contrôle moteur open source, logiciel libre sous licence GNU GPL v3

Sites internet des projets :

[https://vesc-project.com/](https://vesc-project.com/)

[http://www.chibios.org/dokuwiki/doku.php](http://www.chibios.org/dokuwiki/doku.php)

# Mode d'emploi

Pour apporter des modifications au programme de la génératrice, vous devez commencer par télécharger "firmware_generateur"

Avant toute chose, vérifier que la version matérielle de votre carte correspond à celle de la carte sélectionné dans "conf_general.h", par défaut il s'agit du vesc 6.

Dans le fichier "conf_general.h" : "#define HW_SOURCE" est définit comme "hw_60.c"

Dans le fichier "conf_general.h" : "#define HW_HEADER" est définit comme "hw_60.h"

Le code spécifique au fonctionnement de la génératrice ce trouve dans "app_generateur.c" Pour permettre son exécution, il faut qu'il soit désigné comme application à utiliser :

Dans le fichier "conf_general.h" : "#define APP_CUSTOM_TO_USE" doit être définit comme "app_generateur.c"

En cas de modification du code, il est nécessaire de le compiler de nouveau en exécutant la fonction "Makefile" (voir guide de mise en oeuvre), cela créera un dossier "build" dans lequel se trouvera un fichier "BLDC_4_ChibiOS.bin", il s'agit du programme compilé téléversable dans le STM32.



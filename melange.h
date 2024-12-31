#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamique de mémoire
#include <stdio.h>
#include <string.h> // Permet la fonction "memset"
#include "PR.h"
#include "utilitaires.h"
#include "antoine.h"

double trouveACompose(double A1, double A2, double k12);
BMelange* trouveBMelange(titresMelange* titre, varPR* globalesEspece1, varPR* globalesEspece2, char* localisation); // Localisation permet d'afficher l'emplacement de l'erreur sous forme de string en cas de probleme d'allocation memoire
AMelange* trouveAMelange(titresMelange* titre, varPR* globalesEspece1, varPR* globalesEspece2, double k12, char* localisation);
double trouvePhiMelange(varPR* globaleEspece, double Z, double Bi, double B, double A, double A_i, double A2, double titre_i, double titre2, double k12);
void titresEnFonctionDeK(titresMelange *titre, double K1, double K2);
void actualiserTitres(titresMelange *titre, titresMelange *titreSuivant);
double ecartMoyenTitres(titresMelange *titre, titresMelange *titreSuivant);
titresMelange* trouveTitres(varPR* globaleEspece1, varPR* globaleEspece2, double P, double T, double k12, antoineStruct* coeffsAntoine1, antoineStruct* coeffsAntoine2, double critere);



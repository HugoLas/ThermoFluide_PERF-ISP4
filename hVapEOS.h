#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamique de mémoire
#include <stdio.h>
#include "PR.h"
#include "utilitaires.h"

double hVapValue(varPR* globales, Tableau* TabBornesRacines, Tableau* TabRacines);
double hMinusHStar(varPR* globales, double z);
double hMinusHStar(varPR* globales, double z);
double fonctionCpVap(double T, cpStruct *cpCoeffs);
void menuDefautCp(cpStruct *cpCoeffs);
double deltaHSinglePhase(cpStruct* cpCoeffs, varPR* globales1, varPR* globales2, Tableau* TabBornesRacines1, Tableau* TabRacines1, Tableau* TabBornesRacines2, Tableau* TabRacines2);
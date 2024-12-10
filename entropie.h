#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamique de mémoire
#include <stdio.h>
#include "PR.h"
#include "hVapEOS.h"
#include "utilitaires.h"

double sMinusSStar(varPR* globales, double z);
double fonctionCpVapSurT(double T, cpStruct *cpCoeffs);
double deltaSGP(cpStruct* cpCoeffs, varPR* globales1, varPR* globales2);
double deltaEntropie(cpStruct* cpCoeffs, varPR* globales1, varPR* globales2, Tableau* TabBornesRacines1, Tableau* TabRacines1, Tableau* TabBornesRacines2, Tableau* TabRacines2);
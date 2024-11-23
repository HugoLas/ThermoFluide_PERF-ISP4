#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamique de mémoire
#include <stdio.h>
#include "utilitaires.h"

void menuDefautHvapWatson(watsonStruct* proprietes);
double hVapNormalRiedel(watsonStruct* proprietes);
double watsonFormula(watsonStruct* proprietes, double temperature);

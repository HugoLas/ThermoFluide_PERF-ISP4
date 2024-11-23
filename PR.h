#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>
#include "utilitaires.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~ Déclarations des fonctions ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Déclare les fonctions avant main pour les signaler au compiler. */

void trouveZ(varPR* globales, Tableau* TabBornesRacines, Tableau* TabRacines);
double trouveA(varPR* globales);
double trouveB(varPR* globales);
double trouveAlpha(varPR* globales);
double PR(double Z, varPR* globales);
double derivePR(double Z, varPR* globales);
double trouveQZB(double Z, varPR* globales);
double trouvePhi(double Z, double Q, varPR* globales);
void PointsDepartNewton(Tableau *tab, double borneInf, double borneSup, varPR* globales); // La fonction prend en entrée l'adresse du tableau sur lequel travailler, les bornes de la fonction.
void NewtonRaphson(Tableau *tabResult, Tableau *tabIntervalles, double ecartZero, varPR* globales);
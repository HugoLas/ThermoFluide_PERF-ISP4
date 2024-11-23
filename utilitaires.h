#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~ Déclaration des structures ~~~~~~~~~~~~~~~~~~~~~~~~~

/*Les structures sont des objets qui sont constitués d'attributs, par exemple on peut avoir une structure "personne" avec pour attributs un entier âge, et un entier taille.*/

typedef struct{     // Définit une structure "Tableau" qui permet d'obtenir à la fois les données du tableau mais aussi sa taille. (en fonction du nb. de racine, la taille change)
    int taille;
    double *donnees;
} Tableau;

typedef struct{ // Cette structure s'apparente à des variables globales. Elle est passée à toutes les fonctions de la librairie.
    double T1;
    double P1;
    double Tc;
    double Pc;
    double Tr;
    double Pr;
    double Omega_A; 
    double Omega_B;
    double acentric;
    double alpha;
    double A;
    double B;
} varPR;

/* Déclaration des fonctions*/
double valeurMax(Tableau *tab);
double valeurMin(Tableau *tab);
void instancierTableau(Tableau *tab, int taille);
void menuDefautPvap(varPR* globales);


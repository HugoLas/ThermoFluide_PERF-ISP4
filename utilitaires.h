#pragma once // Permet de n'inclure qu'une fois les librairies (ignore les instructions include si déjà fait dans la fonction qui appelle cette librairie)

//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>       // Permet d'effectuer des calculs
#include <stdlib.h>     // Permet l'allocation dynamic de mémoire
#include <stdbool.h>    // Permet l'utilisation des booleans
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

typedef struct{
    double A;
    double B;
    double C;
} antoineStruct;

typedef struct{
    double tempNormal;
    double tempCritic;
    double pressionCritic;
}   watsonStruct;

typedef struct{
    double a;
    double b;
    double c;
    double d;
} cpStruct;

typedef struct{
    double Bliq;
    double Bvap;
} BMelange;

typedef struct{
    double Aliq;
    double Avap;
} AMelange; // Identique à BMelange mais plus clair dans le programme d'en faire 2 structures distinctes

typedef struct{
    double x1;
    double x2;
    double y1;
    double y2;
} titresMelange;


/* Déclaration des fonctions*/
double valeurMax(Tableau *tab);
double valeurMin(Tableau *tab);
void instancierTableau(Tableau *tab, int taille);
void menuDefautPvap(varPR* globales);
void chargerProfilGlobales(varPR *globales);
void menuDefautValeursZ(varPR* globales);
void menuDefautT2P2(varPR *globales, bool SI); // Saisir true pour SI dans le cas où les valeurs sont données en SI.
void lectureAsciiArt(char *str);
double integraleCp(double borneInf, double borneSup, double pasIntegration, double (*ptrFonction)(double, cpStruct*),cpStruct* cpCoeffs); // Utilise la méthode de Simpson (Très précise pour intégrer des polynomes, ici on s'en servira pour intégrer des capacités calo)
Tableau* creerTableauInt(int taille, char* localisation);
varPR* creerGlobales(char* localisation);
antoineStruct* creerAntoine(char* localisation, void (*ptrFonction)(antoineStruct*));
cpStruct* creerCpStruct(char* localisation);
void defautsGlobalesEthane(varPR* globales,double T, double P, bool SI);
void defautsGlobalesHeptane(varPR* globales, double T, double P, bool SI);

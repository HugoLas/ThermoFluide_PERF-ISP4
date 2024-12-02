//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>
#include "PR.h"
#include "utilitaires.h"
#include "Psat.h"
#include "antoine.h"
#include "hVapWatson.h"

int main(){
int choixMenuPrincipal;
double Psaturation; //envisager d'en faire une allocation dynamique uniquement appelée dans case 1 et 2
lectureAsciiArt("art.txt");
printf("\n\n");

do
{
    printf("Que souhaitez-vous calculer ?\n");
    printf("\n");
    printf("(1) La pression de vapeur saturante pour une température ? (Peng-Robinson) \n");
    printf("(2) La pression de vapeur saturante pour une température ? (Formule d'Antoine) \n");
    printf("(3) Les valeurs de facteur de compressibilité (Z) pour une substance, pour une température ? (Peng-Robinson) \n");
    printf("(4) L'enthalpie de vaporisation pour une température donnée. (Corrélations de Riedel et de Watson) \n");
    printf("(0) Pour quitter\n");
    printf("\n");

    printf("--> ");
    scanf("%d", &choixMenuPrincipal); // %d est le code qui indique que l'on souhaite un entier.

    switch(choixMenuPrincipal) {
        case 1:
            varPR *globalesPsat = (varPR*)malloc(sizeof(varPR)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de Psat.c. La structure en question est définie dans utilitaires.h
            if (globalesPsat == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation globalesPr.\n");
            return 0;
            }
            globalesPsat->T1 = 173;           // Valeurs par défaut.
            globalesPsat->Tc = 305.4;         // Valeurs par défaut.
            globalesPsat->Pc = 48.8;          // Valeurs par défaut.
            globalesPsat->Omega_A = 0.457236; // Valeurs par défaut.
            globalesPsat->Omega_B = 0.077796; // Valeurs par défaut.
            globalesPsat->acentric = 0.099;   // Valeurs par défaut.
            menuDefautPvap(globalesPsat);
            globalesPsat->Tr = globalesPsat->T1/globalesPsat->Tc;
            globalesPsat->Pr = globalesPsat->P1/globalesPsat->Pc;
            globalesPsat->alpha = trouveAlpha(globalesPsat);
            globalesPsat->Pr = trouvePsat(1, globalesPsat);
            Psaturation = globalesPsat->Pr*globalesPsat->Pc;
            printf("Pression de vapeur saturante à T = %.2f K vaut %.4f bar.\n", globalesPsat->T1, Psaturation);
            free(globalesPsat);
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            printf("\n");
            break;

        case 2:
            double *temperatureAntoine = (double*)malloc(sizeof(double));
            if (temperatureAntoine == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation temperature.\n");
            return 0;
            }
            antoineStruct *coefficientsAntoine = (antoineStruct*)malloc(sizeof(antoineStruct));
            if (coefficientsAntoine == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation coefficientsAntoine.\n");
            return 0;
            }
            menuDefautAntoine(coefficientsAntoine);
            printf("Choix de la température [K] : (Sélectionnez une valeur cohérente avec la plage de validité des coefficients !)\n");
            printf("--> ");
            scanf("%lf", temperatureAntoine);
            printf("Pression de vapeur saturante à T = %.2f K vaut %.4f bar.\n", *temperatureAntoine, antoineFormula(coefficientsAntoine,*temperatureAntoine));
            free(coefficientsAntoine);
            free(temperatureAntoine);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;

        case 3:
            //printf("Pas encore dispo" );
            //printf("\n");
            varPR *globalesPr = (varPR*)malloc(sizeof(varPR)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de PR.c et Psat.c. La structure en question est définie dans utilitaires.h
            if (globalesPr == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation globalesPr.\n");
            return 0;
            }
            globalesPr->T1 = 173;           // Valeur par défaut.
            globalesPr->P1 = 0.5271;        // Valeur par défaut. (provient de la question sur pVap)
            globalesPr->Tc = 305.4;         // Valeur par défaut.
            globalesPr->Pc = 48.8;          // Valeur par défaut.
            globalesPr->Omega_A = 0.457236; // Valeur par défaut.
            globalesPr->Omega_B = 0.077796; // Valeur par défaut.
            globalesPr->acentric = 0.099;   // Valeur par défaut.
            menuDefautValeursZ(globalesPr);
            Tableau *TableauBornesRacines = (Tableau*)malloc(sizeof(Tableau));
            if (TableauBornesRacines == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation TableauBornesRacines dans couple Z.\n");
            return 0;
            }
            instancierTableau(TableauBornesRacines,6);
            Tableau *TableauRacines = (Tableau*)malloc(sizeof(Tableau));
            if (TableauRacines == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation TableauRacines dans couple Z.\n");
            return 0;
            }
            instancierTableau(TableauRacines,3);
            trouveZ(globalesPr, TableauBornesRacines, TableauRacines);
            if (TableauRacines->taille==1)
            {
                printf("Pas d'équilibre liquide vapeur, Z = %.4f \n",TableauRacines->donnees[0]);
            }
            else if ((TableauRacines->taille==2) && (TableauRacines->donnees[0]==-1))
            {
                printf("Erreur lors de la recherche du couple Z, erreur provient de NewtonRaphson.");
            }
            else
            {
                printf("Equilibre liquide vapeur, Z_liq = %.4f, Z_vap = %.4f \n",TableauRacines->donnees[0],TableauRacines->donnees[1]);
            }
            free(TableauBornesRacines->donnees);
            free(TableauRacines->donnees);
            free(TableauBornesRacines);
            free(TableauRacines);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;

        case 4:
            double *temperatureWatson = (double*)malloc(sizeof(double));
            if (temperatureWatson == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation temperature.\n");
            return 0;
            }
            watsonStruct *parametres = (watsonStruct*)malloc(sizeof(watsonStruct));
            if (parametres == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation parametres Watson.\n");
            return 0;
            }
            menuDefautHvapWatson(parametres);
            printf("Choix de la température [K] : \n");
            printf("--> ");
            scanf("%lf", temperatureWatson);
            printf("Enthalpie de vaporisation à T = %.2f K vaut %.6f J/mol",*temperatureWatson, watsonFormula(parametres, *temperatureWatson));
            free(parametres);
            free(temperatureWatson);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar(); 
            break;
        case 0:
            printf("\n");
        default:
            printf("Numéro de valeur non reconnu. \n" );
    }

} while (choixMenuPrincipal != 0);

return 0;
}
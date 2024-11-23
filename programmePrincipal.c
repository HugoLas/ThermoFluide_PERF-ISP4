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
            varPR *globalesPr = (varPR*)malloc(sizeof(varPR)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de PR.c et Psat.c. La structure en question est définie dans utilitaires.h
            if (globalesPr == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation globalesPr.\n");
            return 0;
            }
            globalesPr->T1 = 173;           // Valeurs par défaut.
            globalesPr->Tc = 305.4;         // Valeurs par défaut.
            globalesPr->Pc = 48.8;          // Valeurs par défaut.
            globalesPr->Omega_A = 0.457236; // Valeurs par défaut.
            globalesPr->Omega_B = 0.077796; // Valeurs par défaut.
            globalesPr->acentric = 0.099;   // Valeurs par défaut.
            menuDefautPvap(globalesPr);
            globalesPr->Tr = globalesPr->T1/globalesPr->Tc;
            globalesPr->Pr = globalesPr->P1/globalesPr->Pc;
            globalesPr->alpha = trouveAlpha(globalesPr);
            globalesPr->Pr = trouvePsat(1, globalesPr);
            Psaturation = globalesPr->Pr*globalesPr->Pc;
            printf("Pression de vapeur saturante à T = %.2f K vaut %.4f bar.\n", globalesPr->T1, Psaturation);
            free(globalesPr);
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
            printf("Pas encore dispo" );
            printf("\n");
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

        default:
            printf("Numéro de valeur non reconnu. \n" );
    }

} while (choixMenuPrincipal != 0);

return 0;
}
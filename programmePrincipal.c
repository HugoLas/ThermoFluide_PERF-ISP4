//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>
#include "PR.h"
#include "utilitaires.h"
#include "Psat.h"

int main(){
    int choixMenuPrincipal;

    printf("Bienvenue dans ProSpen -2. Que souhaitez-vous calculer ?\n");
    printf("\n");
    printf("(1) La pression de vapeur saturante pour une température ? (Peng-Robinson) \n");
    printf("(2) La pression de vapeur saturante pour une température ? (Formule d'Antoine) \n");
    printf("(3) Les valeurs de facteur de compressibilité (Z) pour une substance, pour une température ? (Peng-Robinson) \n");
    printf("\n");

    printf("--> ");

    scanf("%d", &choixMenuPrincipal); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenuPrincipal != 1 || choixMenuPrincipal != 2 || choixMenuPrincipal != 3) // On demande un choix tant que l'utilisateur rentre n'importe quoi. J'aurais aussi pu mettre une condition du type <1 ou >3.
    {
        switch(choixMenuPrincipal) {
            case 1:
                varPR *globalesPr = (varPR*)malloc(sizeof(varPR)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de PR.c et Psat.c. La structure en question est définie dans utilitaires.h
                menuValeursDefautPvap(globalesPr);
                trouvePsat(1, globalesPr);
                printf("\n");
                break;
            case 2:
                printf("Pas encore dispo" );
                printf("\n");
                break;
            case 3:
                printf("Pas encore dispo" );
                printf("\n");
                break;
            default:
                printf("Numéro de valeur non reconnue. \n" );
        }
        printf("--> ");
        scanf("%d", &choixMenuPrincipal);
    }
    
}
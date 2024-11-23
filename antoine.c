#include "antoine.h"
#include "utilitaires.h"

double antoineFormula(antoineStruct* coeffs, double Temp){

return pow(10,coeffs->A-(coeffs->B / (Temp + coeffs->C)));

}

void menuDefautAntoine(antoineStruct* coeffs){
int choixMenu;
choixMenu = -1; // initialisation
printf("Plusieurs espèces et plusieurs plages de validité sont proposées.\n");
printf("\n");
printf("(1) Ethane de 91.33 à 144.13K [NIST, Carruth and Kobayashi, 1973]\n");
printf("(2) Ethane de 135.74 à 199.91K [NIST, Loomis and Walters, 1926]\n");
printf("(3) Pentane de 268.8 à 341.37K [NIST, Osborn and Douslin, 1974]\n");
printf("(0) Espèce customisée (Utilisateur devra rentrer les coefficients lui-même)\n");
printf("\n");
printf("Faites votre choix\n");
printf("--> ");
scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
while(choixMenu > 3 || choixMenu < 0) // choix en dehors de l'intervalle.
{
    printf("Valeur non valide, recommencez.\n");
    printf("--> ");
    scanf("%d", &choixMenu);
}

switch(choixMenu) {
    case 1:
        coeffs->A = 4.50706;
        coeffs->B = 791.3;
        coeffs->C = -6.422;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
    case 2:
        coeffs->A = 3.93835;
        coeffs->B = 659.739;
        coeffs->C = -16.719;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
    case 3:
        coeffs->A = 3.9892;
        coeffs->B = 1070.617;
        coeffs->C = -40.454;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
    default:
        printf("Vous avez choisi espèce customisée. \n" );
        coeffs->A = 0;
        coeffs->B = 0;
        coeffs->C = 0;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
}

choixMenu = -1; // réinitialisation
printf("Recherche de la pression de vapeur saturante par Antoine. Les coefficients par défaut sont : \n");
printf("\n");
printf("(1) A = %f \n", coeffs->A);
printf("(2) B = %f \n", coeffs->B);
printf("(3) C = %f \n", coeffs->C);
printf("\n");
printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
printf("--> ");
scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
while (choixMenu != 0)
{
    switch(choixMenu) {
        case 1:
            printf("A = " );
            scanf("%lf", &coeffs->A); //%lf spécifie que l'on attend un double, & spécifie à quelle variable attribuer la valeur.
            printf("\n");
            break;
        case 2:
            printf("B = " );
            scanf("%lf", &coeffs->B);
            printf("\n");
            break;
        case 3:
            printf("C = " );
            scanf("%lf", &coeffs->C);
            printf("\n");
            break;
        default:
            printf("Numéro de valeur non reconnue. \n" );
    }
    printf("Pour changer une autre valeur, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");
    scanf("%d", &choixMenu);
}

}
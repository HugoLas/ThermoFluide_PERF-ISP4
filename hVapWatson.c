#include "hVapWatson.h"
#include "utilitaires.h"

void menuDefautHvapWatson(watsonStruct* proprietes){
int choixMenu;
choixMenu = -1; // initialisation
printf("Plusieurs espèces sont proposées.\n");
printf("\n");
printf("(1) Ethane [NIST]\n");
printf("(2) Pentane [NIST]\n");
printf("(0) Espèce customisée (Utilisateur devra rentrer les valeurs lui-même)\n");
printf("\n");
printf("Faites votre choix\n");
printf("--> ");
scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
while(choixMenu > 2 || choixMenu < 0) // choix en dehors de l'intervalle.
{
    printf("Valeur non valide, recommencez.\n");
    printf("--> ");
    scanf("%d", &choixMenu);
}

switch(choixMenu) {
    case 1:
        proprietes->tempNormal = 184.6;
        proprietes->tempCritic = 305.3;
        proprietes->pressionCritic = 49;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
    case 2:
        proprietes->tempNormal = 309.2;
        proprietes->tempCritic = 469.8;
        proprietes->pressionCritic = 33.6;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
    default:
        printf("Vous avez choisi espèce customisée. \n" );
        proprietes->tempNormal = 0;
        proprietes->tempCritic = 0;
        proprietes->pressionCritic = 0;
        printf("Profil chargé.\n" );
        printf("\n");
        break;
}

choixMenu = -1; // réinitialisation
printf("Recherche de l'enthalpie de vaporisation normale par Riedel. Les coefficients par défaut sont : \n");
printf("\n");
printf("(1) Température normale d'ébullition = %f K \n", proprietes->tempNormal);
printf("(2) Température critique = %f K \n", proprietes->tempCritic);
printf("(3) Pression critique = %f bar \n", proprietes->pressionCritic);
printf("\n");
printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
printf("--> ");
scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
while (choixMenu != 0)
{
    switch(choixMenu) {
        case 1:
            printf("Température normale d'ébullition = " );
            scanf("%lf", &proprietes->tempNormal); //%lf spécifie que l'on attend un double, & spécifie à quelle variable attribuer la valeur.
            printf("\n");
            break;
        case 2:
            printf("Température critique = " );
            scanf("%lf", &proprietes->tempCritic);
            printf("\n");
            break;
        case 3:
            printf("Pression critique = " );
            scanf("%lf", &proprietes->pressionCritic);
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

double hVapNormalRiedel(watsonStruct* proprietes){ // en J/mol [https://sciencedirect.com/science/article/pii/S2090123213000386]
    return ((1.093*8.314*proprietes->tempNormal) * ( (log(proprietes->pressionCritic)-1.013)/(0.930-((proprietes->tempNormal)/(proprietes->tempCritic))) ));

}

double watsonFormula(watsonStruct* proprietes, double temperature){
    return hVapNormalRiedel(proprietes)*pow((1-(temperature/proprietes->tempCritic))/(1-(proprietes->tempNormal/proprietes->tempCritic)),0.38);

}
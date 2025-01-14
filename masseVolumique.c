#include "masseVolumique.h"

double trouveRhoLiq(varPR* globales, Tableau* TabBornesRacines, Tableau* TabRacines, double grandM){
    double rho;
    globales->alpha = trouveAlpha(globales);
    trouveZ(globales,TabBornesRacines,TabRacines);
    rho = grandM*((globales->P1)/(TabRacines->donnees[0]*8.314*globales->T1));
    return rho;
}

void menuDefautRho(varPR* globales, double grandM){
    int choixMenu;

    printf("Recherche de la masse volumique d'un liquide. Les valeurs par défaut sont : \n");
    printf("\n");

    printf("(1) T1 = %.3f K \n", globales->T1);

    printf("(2) P1 = %.3f Pa \n", globales->P1);

    printf("(3) M = %.6f kg/mol \n", grandM);

    printf("(4) Tc = %.3f K \n", globales->Tc);

    printf("(5) Pc = %.3f Pa \n", globales->Pc);

    printf("(6) Facteur acentric \"w\" = %.6f \n", globales->acentric);

    printf("(7) Omega_A = %.6f \n", globales->Omega_A);

    printf("(8) Omega_B = %.6f \n", globales->Omega_B);

    printf("\n");

    printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");

    scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenu != 0)
    {
        switch(choixMenu) {
            case 1:
                printf("T1 = " );
                scanf("%lf", &globales->T1); //%lf spécifie que l'on attend un double, & sert à récupérer l'adresse de T1
                printf("\n");
                break;
            case 2:
                printf("P1 = ");
                scanf("%lf", &globales->P1);
                break;
            case 3:
                printf("M = ");
                scanf("%lf", &grandM); // "&" car on fournit la valeur sur laquelle le pointeur pointe (j'ai bien mis une '*' devant grandM dans l'appel de la fonction). Ici on veut son adresse.
                printf("\n");
                break;
            case 4:
                printf("Tc = " );
                scanf("%lf", &globales->Tc);
                printf("\n");
                break;
            case 5:
                printf("Pc = " );
                scanf("%lf", &globales->Pc);
                printf("\n");
                break;
            case 6:
                printf("facteur acentric = " );
                scanf("%lf", &globales->acentric);
                printf("\n");
                break;
            case 7:
                printf("Omega_A = " );
                scanf("%lf", &globales->Omega_A);
                printf("\n");
                break;
            case 8:
                printf("Omega_B = " );
                scanf("%lf", &globales->Omega_B);
                printf("\n");
                break;
            case 0:
                printf("\n");
                break;
            default:
                printf("Numéro de valeur non reconnue. \n" );
                break;
        }
        printf("Pour changer une autre valeur, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
        printf("--> ");
        scanf("%d", &choixMenu);
    }
    globales->Tr = globales->T1/globales->Tc;
    globales->Pr = globales->P1/globales->Pc;
    return;
}

double volumeMolaire(varPR* globales, Tableau* TabBornesRacines, Tableau* TabRacines, int phase){
    globales->alpha = trouveAlpha(globales);
    trouveZ(globales,TabBornesRacines,TabRacines);
    if (phase == 1)                         // 1 -> phase liquide 
    {
        return((8.314*globales->T1*TabRacines->donnees[0])/globales->P1);
    }
    else if (TabRacines->taille == 1)       // taille == 1 + phase vapeur -> une seule racine, pas d'équilibre donc vapeur surchauffée. (ou liq sous refroidi mais on part du principe que l'utilisateur a bien choisi la phase)
    {
        printf("taille == 1 + phase vapeur -> une seule racine \n");
        printf("TabRacines->donnees[0] = %.4f \n", TabRacines->donnees[0]);
        return((8.314*globales->T1*TabRacines->donnees[0])/globales->P1);
    }
    else                                    // taille != 1 + phase vapeur -> 2 racines, il y a un équilibre liq vap. Dans ce cas, zvap est dans la seconde case. (la case n°1 car l'indexage démarre à 0)
    {
        //printf("taille != 1 + phase vapeur -> 2 racines \n");
        //printf("TabRacines->donnees[1] = %.4f \n",TabRacines->donnees[1]);
        return((8.314*globales->T1*TabRacines->donnees[1])/globales->P1);
    }
}

double deltaV(varPR* globales1, Tableau* TabBornesRacines1, Tableau* TabRacines1, varPR* globales2, Tableau* TabBornesRacines2, Tableau* TabRacines2){
    int choixPhase;
    double v1, v2;
    printf("Détermination de la différence de volume molaire d'une phase pour un changement de couple {P,T}. Avec quelle phase souhaitez-vous travailler ? \n");
    printf("\n");

    printf("(1) Phase liquide \n");

    printf("(2) Phase gaz \n");
    printf("\n");
    printf("--> ");

    scanf("%d", &choixPhase); // %d est le code qui indique que l'on souhaite un entier.
    printf("\n");
    
    while ((choixPhase!=1) && (choixPhase!=2))
    {
        printf("Valeur non reconnue. Veuillez recommencer : \n");
        printf("--> ");
        scanf("%d", &choixPhase);
        printf("\n");
    }

    v1 = volumeMolaire(globales1,TabBornesRacines1,TabRacines1,choixPhase);
    printf("v1 = %.8f \n", v1);
    v2 = volumeMolaire(globales2,TabBornesRacines2,TabRacines2,choixPhase);
    printf("v2 = %.8f \n", v2);
    return(v2-v1);
}
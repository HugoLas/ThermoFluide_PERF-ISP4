#include "utilitaires.h"

void menuValeursDefautPVap(varPR* globales){
    int choixMenu;

    printf("Recherche de la pression de vapeur saturante. Les valeurs par défaut sont : \n");
    printf("\n");

    printf("(1) globales->T1 = %.3f K \n", globales->T1);

    printf("(2) globales->Tc = %.3f K \n", globales->Tc);

    printf("(3) globales->Pc = %.3f bar \n", globales->Pc);

    printf("(4) Facteur globales->acentric \"w\" = %.6f \n", globales->acentric);

    printf("(5) globales->Omega_A = %.6f \n", globales->Omega_A);

    printf("(6) globales->Omega_B = %.6f \n", globales->Omega_B);

    printf("\n");

    printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");

    scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenu != 0)
    {
        switch(choixMenu) {
            case 1:
                printf("globales->T1 = " );
                scanf("%lf", &globales->T1); //%lf spécifie que l'on attend un double, & spécifie à quelle variable attribuer la valeur.
                printf("\n");
                break;
            case 2:
                printf("globales->Tc = " );
                scanf("%lf", &globales->Tc);
                printf("\n");
                break;
            case 3:
                printf("globales->Pc = " );
                scanf("%lf", &globales->Pc);
                printf("\n");
                break;
            case 4:
                printf("facteur globales->acentric = " );
                scanf("%lf", &globales->acentric);
                printf("\n");
                break;
            case 5:
                printf("globales->Omega_A = " );
                scanf("%lf", &globales->Omega_A);
                printf("\n");
                break;
            case 6:
                printf("globales->Omega_B = " );
                scanf("%lf", &globales->Omega_B);
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

double valeurMin(Tableau *tab){
    int i;
    double valMin;
    if (tab->taille==0)
    {
        printf("erreur : tableau de taille nulle. valeurMax = -1000");
        return -1000;
    }
    
    valMin = tab->donnees[0];
    for (i = 0; i < tab->taille; i++)
    {
        if (tab->donnees[i]<valMin)
        {
            valMin = tab->donnees[i];
        }
    }
    return valMin;
}

double valeurMax(Tableau *tab){
    int i;
    double valMax;
    if (tab->taille==0)
    {
        printf("erreur : tableau de taille nulle. valeurMax = -1000");
        return -1000;
    }
    
    valMax = tab->donnees[0];
    for (i = 0; i < tab->taille; i++)
    {
        if (tab->donnees[i]>valMax)
        {
            valMax = tab->donnees[i];
        }
    }
    return valMax;
}

void instancierTableau(Tableau *tab, int taille){   // Cette fonction permet la création du tableau, associée à sa taille. Le tableau est alloué mais non peuplé.
    tab->donnees = (double*)malloc(taille*sizeof(double));
    tab->taille = taille;
    if (tab->donnees == NULL)
    {
        printf("Erreur de malloc : Structure tableau, allocation échouée");
        tab->taille = 0;
    }
}

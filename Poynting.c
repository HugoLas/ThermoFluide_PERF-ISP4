# include "Poynting.h"
# include "PR.h"

double vRackett(varPR* globales, double Zc){
    //printf("Zc = %.3f \n", Zc);
    //printf("(8.314*globales->Tc/globales->Pc) = %.4f \n",(8.314*globales->Tc/globales->Pc));
    //printf("pow(Zc,pow((1-globales->Tr),0.2857)) = %.4f \n",pow(Zc,pow((1-globales->Tr),0.2857)));
    //printf("pow((1-globales->Tr),0.2857) = %.4f \n",pow((1-globales->Tr),0.2857));
    return((8.314*globales->Tc/globales->Pc)*pow(Zc,pow((1-globales->Tr),0.2857))); // page 99
}

double poyntingFactor(varPR* globales, double Zc, double P2){
    double argumentExpo = vRackett(globales, Zc)*(P2-globales->P1)/(8.314*globales->T1); 
    //printf("vRackett = %.4f \n", vRackett(globales, Zc));
    //printf("argumentExpo = %.4f \n", argumentExpo);
    return(exp(argumentExpo));
}

double fugaciteLiq(double ZVap, varPR* globales, double P2, double Zc){
    double Q = trouveQZB(ZVap,globales);
    //printf("Q = %.3f \n", Q);
    double phi = trouvePhi(ZVap,Q,globales);
    printf("phi = %.3f \n", phi);
    double Py = poyntingFactor(globales,Zc,P2);
    printf("Py = %.3f \n", Py);
    printf("Psat = %.3f \n", globales->P1);
    return(phi*globales->P1*Py);
}

void menuPoynting(varPR* globales, double* P2, double* Zc){
    int choixMenu;
    *P2 = 13.78;
    *Zc = 48.8*pow(10,5)*148.3*pow(10,-6)/(8.314*305.4); // Zc = Pc*Vc/(R*Tc)
    printf("\n");
    printf("Valeurs par défaut : \n");
    //printf("[1] T1 = %.2f \n",globales->T1);
    //printf("[2] Psat = %.3f \n",globales->P1);
    printf("[1] P2 = %.2f \n", *P2);
    printf("[2] Zc = %.3f \n", *Zc);
    printf("\n");

    printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");

    scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenu != 0)
    {
        switch(choixMenu) {
            //case 1:
            //    printf("T1 [K] = " );
            //    scanf("%lf", &globales->T1); //%lf spécifie que l'on attend un double, & spécifie à quelle variable attribuer la valeur.
            //    printf("\n");
            //    break;
            //case 2:
            //    printf("Psat [bar] = ");
            //    scanf("%lf", &globales->P1);
            //    break;
            case 1:
                printf("P2 [bar] = ");
                scanf("%lf", *P2); // on fournit déjà un pointeur, nul besoin du "&"
                break;
            case 2:
                printf("Zc = ");
                scanf("%lf", *Zc);
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



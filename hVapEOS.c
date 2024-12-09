#include "hVapEOS.h"

double hVapValue(varPR* globales, Tableau* TabBornesRacines, Tableau* TabRacines){
    double ecartGpVap, ecartGpLiq;
    trouveZ(globales,TabBornesRacines,TabRacines);
    ecartGpVap = hMinusHStar(globales, TabRacines->donnees[1]);
    ecartGpLiq = hMinusHStar(globales, TabRacines->donnees[0]);
    return (ecartGpVap-ecartGpLiq);
}

double hMinusHStar(varPR* globales, double z){
    double m;
    m = 0.37464+(1.54226*globales->acentric)-(0.26992*globales->acentric*globales->acentric);
    return (8.314*globales->T1*(z-1-((globales->A*sqrt(2))/(4*globales->B))*(1+(m*sqrt(globales->Tr/globales->alpha)))*log(trouveQZB(z, globales))));
}

double fonctionCpVap(double T, cpStruct *cpCoeffs){
    return (cpCoeffs->a + cpCoeffs->b*T + cpCoeffs->c*pow(T,2) + cpCoeffs->d*pow(T,3));
}

void menuDefautCp(cpStruct *cpCoeffs){
    int choixMenu;

    printf("Détermination de l'enthalpie du gaz parfait. Référence prise à T0 = 0K, P0 = 1 atm, h*(T0,P0) = 0 J/mol. Les valeurs par défaut sont : \n");
    printf("\n");

    printf("(1) CPVAP A = %.12f \n", cpCoeffs->a);

    printf("(2) CPVAP B = %.12f \n", cpCoeffs->b);

    printf("(3) CPVAP C = %.12f \n", cpCoeffs->c);

    printf("(4) CPVAP D = %.12f \n", cpCoeffs->d);

    printf("\n");

    printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");

    scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenu != 0)
    {
        switch(choixMenu) {
            case 1:
                printf("CPVAP A = " );
                scanf("%lf", &cpCoeffs->a); //%lf spécifie que l'on attend un double, & sert à récupérer l'adresse de a.
                printf("\n");
                break;
            case 2:
                printf("CPVAP B = ");
                scanf("%lf", &cpCoeffs->b);
                break;
            case 3:
                printf("CPVAP C = ");
                scanf("%lf", &cpCoeffs->c);
                break;
            case 4:
                printf("CPVAP D = ");
                scanf("%lf", &cpCoeffs->d);
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
    return;
}

double deltaHSinglePhase(cpStruct* cpCoeffs, varPR* globales1, varPR* globales2, Tableau* TabBornesRacines1, Tableau* TabRacines1, Tableau* TabBornesRacines2, Tableau* TabRacines2){
    /* Pour bien comprendre cette fonction, je recommande de visualiser les équations suivantes (écrites en LaTeX) :
    
    \left(h-h^{*}\right)_{T_{2},P_{2}}-\left(h-h^{*}\right)_{T_{1},P_{1}}=h_{T_{2},P_{2}}-h_{T_{2},P_{2}}^{*}-h_{T_{1},P_{1}}+h_{T_{1},P_{1}}^{*}=\left(h_{T_{2},P_{2}}-h_{T_{1},P_{1}}\right)-\left(h_{T_{2},P_{2}}^{*}-h_{T_{1},P_{1}}^{*}\right)

    \boxed{\underbrace{\left(h_{T_{2},P_{2}}-h_{T_{1},P_{1}}\right)}_{\Delta h}=\left(h-h^{*}\right)_{T_{2},P_{2}}-\left(h-h^{*}\right)_{T_{1},P_{1}}+\left(h_{T_{2},P_{2}}^{*}-h_{T_{1},P_{1}}^{*}\right)}

    h_{T_{1},P_{1}}^{*}=\underbrace{h_{T_{0},P_{0}}^{*}}_{\mathclap{\text{Référence considérée}=0}}+\int_{\underbrace{T_{0}}_{\mathclap{=0}}}^{T_{1}}c_{p}^{*}dT

    h_{T_{2},P_{2}}^{*}=\underbrace{h_{T_{0},P_{0}}^{*}}_{\mathclap{\text{Référence considérée}=0}}+\int_{\underbrace{T_{0}}_{\mathclap{=0}}}^{T_{2}}c_{p}^{*}dT
    
    */
    
    int choixPhase;
    double deltaH;

    printf("Détermination de la différence d'enthalpie d'une phase pour un changement de couple {P,T}. Avec quelle phase souhaitez-vous travailler ? \n");
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
    
    trouveZ(globales1, TabBornesRacines1, TabRacines1);
    printf("Pour {P1,T1} : \n");
    if (TabRacines1->taille==1)
    {
        printf("Pas d'équilibre liquide vapeur, Z = %.4f \n",TabRacines1->donnees[0]);
    }
    else if ((TabRacines1->taille==2) && (TabRacines1->donnees[0]==-1))
    {
        printf("Erreur lors de la recherche du couple Z, erreur provient de NewtonRaphson.");
    }
    else
    {
        printf("Equilibre liquide vapeur, Z_liq = %.4f, Z_vap = %.4f \n",TabRacines1->donnees[0],TabRacines1->donnees[1]);
    }

    trouveZ(globales2, TabBornesRacines2, TabRacines2);
    printf("Pour {P2,T2} : \n");
    if (TabRacines2->taille==1)
    {
        printf("Pas d'équilibre liquide vapeur, Z = %.4f \n",TabRacines2->donnees[0]);
    }
    else if ((TabRacines2->taille==2) && (TabRacines2->donnees[0]==-1))
    {
        printf("Erreur lors de la recherche du couple Z, erreur provient de NewtonRaphson.");
    }
    else
    {
        printf("Equilibre liquide vapeur, Z_liq = %.4f, Z_vap = %.4f \n",TabRacines2->donnees[0],TabRacines2->donnees[1]);
    }

    // TODO : Pas sûr que ce soit faisable mais attention : Si l'utilisateur choisit phase vapeur alors qu'il a des liquides sous refroidis ou inversement, rien ne le protège. S'il existe une seule phase pour un des couples {P,T}, l'utilisateur doit la choisir impérativement sous peine d'avoir de faux résultats.
    // La précision des résultats peut être améliorée en donnant un pas d'intégration plus faible à integraleCp. (3e paramètre de la fonction)
    switch(choixPhase) {
        /*Par la suite, je suis amené à appeler globales1->T1 et globales2->T1. Ce n'est pas une erreur. Le premier correspond à T1, le second à T2 malgré son nom qui peut porter à confusion. C'est simplement lié au nommage des éléments de la structure VarPR dans utilitaires.h*/
        case 1: // Cas simple car le z_liq se trouve toujours dans la première case du tableau, qu'il soit sous-refroidi (taille du tableau = 1) ou à l'éq (taille du tableau = 2)
            deltaH = hMinusHStar(globales2,TabRacines2->donnees[0]) - hMinusHStar(globales1,TabRacines1->donnees[0]) + (integraleCp(0,globales2->T1,0.0003,fonctionCpVap,cpCoeffs) - integraleCp(0,globales1->T1,0.0003,fonctionCpVap,cpCoeffs));
            break;
        case 2: // Cas litigieux car le z_vap peut se trouver dans la première ou seconde case du tableau selon s'il existe un équilibre ou non avec le couple {P,T} donné.
            if (TabRacines1->taille==1 && TabRacines2->taille==1) // Il n'y a pas d'équilibre (vapeur surchauffée pour les 2 couples {P,T}) -> Z_vap est donc toujours dans la première case de TabRacines
            {
                deltaH = hMinusHStar(globales2,TabRacines2->donnees[0]) - hMinusHStar(globales1,TabRacines1->donnees[0]) + (integraleCp(0,globales2->T1,0.0003,fonctionCpVap,cpCoeffs) - integraleCp(0,globales1->T1,0.0003,fonctionCpVap,cpCoeffs));
            }
            else if (TabRacines1->taille==1)                      // Seul le couple {P,T} de départ génère une vapeur surchauffée. Je prends donc la première case du tableau 1 mais la deuxième du tableau 2.
            {
                deltaH = hMinusHStar(globales2,TabRacines2->donnees[1]) - hMinusHStar(globales1,TabRacines1->donnees[0]) + (integraleCp(0,globales2->T1,0.0003,fonctionCpVap,cpCoeffs) - integraleCp(0,globales1->T1,0.0003,fonctionCpVap,cpCoeffs));
            }
            else if (TabRacines2->taille==1)                // Seul le couple {P,T} de fin génère une vapeur surchauffée. Je prends donc la deuxième case du tableau 1 mais la première case du tableau 2.
            {
                deltaH = hMinusHStar(globales2,TabRacines2->donnees[0]) - hMinusHStar(globales1,TabRacines1->donnees[1]) + (integraleCp(0,globales2->T1,0.0003,fonctionCpVap,cpCoeffs) - integraleCp(0,globales1->T1,0.0003,fonctionCpVap,cpCoeffs));
            }
            else                                                  // Les deux conditions données génèrent des équilibres. TableauRacine1&2 ont toujours 2 cases et z_vap est toujours dans la seconde.
            {
                deltaH = hMinusHStar(globales2,TabRacines2->donnees[1]) - hMinusHStar(globales1,TabRacines1->donnees[1]) + (integraleCp(0,globales2->T1,0.0003,fonctionCpVap,cpCoeffs) - integraleCp(0,globales1->T1,0.0003,fonctionCpVap,cpCoeffs));        
            }
            break;
        default:
            printf("Numéro de valeur non reconnue. \n" );
            printf("Erreur dans hVapEOS.c -> Dans le switchcase(choixPhase), une valeur non reconnue a été renseignée malgré la vérification -> return -1 \n");
            return -1;
            break;
        }
    return (0.001*deltaH);
}
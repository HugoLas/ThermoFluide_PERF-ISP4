#include "entropie.h"

double sMinusSStar(varPR* globales, double z){
    double m;
    m = 0.37464+(1.54226*globales->acentric)-(0.26992*globales->acentric*globales->acentric);
    return (8.314*(log(z-globales->B) - ((globales->A*sqrt(2))/(4*globales->B)) * (m*sqrt(globales->Tr/globales->alpha)) * log(trouveQZB(z, globales))));
}

double fonctionCpVapSurT(double T, cpStruct *cpCoeffs){
    return (cpCoeffs->a/T + cpCoeffs->b + cpCoeffs->c*T + cpCoeffs->d*pow(T,2));
}

double deltaSGP(cpStruct* cpCoeffs, varPR* globales1, varPR* globales2){
    double integrale1, integrale2;
    integrale1 = integraleCp(1,globales1->T1,0.003,fonctionCpVapSurT,cpCoeffs); // Je prends T0=1K comme reference pour pas diviser par 0 à borne inf (j'ai A/T dans l'intégrale). Pour se convaincre qu'on peut prendre n'importe quelle ref : \int_{1}^{3}x^{2}\,dx-\int_{1}^{2}x^{2}\,dx=\int_{1.2}^{3}x^{2}\,dx-\int_{1.2}^{2}x^{2}\,dx
    integrale2 = integraleCp(1,globales2->T1,0.003,fonctionCpVapSurT,cpCoeffs); // Oui, je sais, le membre "T1" de la structure crée une confusion. j'aurais dû l'appeler simplement T ou créer une autre structure avec T1 & T2, P1 & P2, etc.
    printf("integrale2-integrale1 = %.2f \n", integrale2-integrale1);
    printf("log(globales1->P1/globales2->P1) = %.2f \n", log(globales1->P1/globales2->P1));
    printf("return = %.2f \n",8.314*log(globales1->P1/globales2->P1)+integrale2-integrale1);
    
    return(8.314*log(globales1->P1/globales2->P1)+integrale2-integrale1);
}

// Piste d'amélioration : Cette fonction est quasiment la même que pour l'enthalpie. Possibilité de factoriser (et passer en utilitaire) en prenant des pointeurs de fonctions dans les paramètes (sMinusSStar, hMinusHStar, deltaSGP et créer l'équiv pour deltaHGP)
double deltaEntropie(cpStruct* cpCoeffs, varPR* globales1, varPR* globales2, Tableau* TabBornesRacines1, Tableau* TabRacines1, Tableau* TabBornesRacines2, Tableau* TabRacines2){
    int choixPhase;
    double deltaS;

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
            deltaS = sMinusSStar(globales2,TabRacines2->donnees[0]) - sMinusSStar(globales1,TabRacines1->donnees[0]) + deltaSGP(cpCoeffs,globales1,globales2);
            break;
        case 2: // Cas litigieux car le z_vap peut se trouver dans la première ou seconde case du tableau selon s'il existe un équilibre ou non avec le couple {P,T} donné.
            if (TabRacines1->taille==1 && TabRacines2->taille==1) // Il n'y a pas d'équilibre (vapeur surchauffée pour les 2 couples {P,T}) -> Z_vap est donc toujours dans la première case de TabRacines
            {
                deltaS = sMinusSStar(globales2,TabRacines2->donnees[0]) - sMinusSStar(globales1,TabRacines1->donnees[0]) + deltaSGP(cpCoeffs,globales1,globales2);
            }
            else if (TabRacines1->taille==1)                      // Seul le couple {P,T} de départ génère une vapeur surchauffée. Je prends donc la première case du tableau 1 mais la deuxième du tableau 2.
            {
                deltaS = sMinusSStar(globales2,TabRacines2->donnees[1]) - sMinusSStar(globales1,TabRacines1->donnees[0]) + deltaSGP(cpCoeffs,globales1,globales2);
            }
            else if (TabRacines2->taille==1)                // Seul le couple {P,T} de fin génère une vapeur surchauffée. Je prends donc la deuxième case du tableau 1 mais la première case du tableau 2.
            {
                //printf("sMinusSStar(globales2,TabRacines2->donnees[0]) = %.2f \n", sMinusSStar(globales2,TabRacines2->donnees[0]));
                //printf("sMinusSStar(globales1,TabRacines1->donnees[1]) = %.2f \n", sMinusSStar(globales1,TabRacines1->donnees[1]));
                //printf("deltaSGP(cpCoeffs,globales1,globales2) = %.2f \n", deltaSGP(cpCoeffs,globales1,globales2));
                deltaS = sMinusSStar(globales2,TabRacines2->donnees[0]) - sMinusSStar(globales1,TabRacines1->donnees[1]) + deltaSGP(cpCoeffs,globales1,globales2);
            }
            else                                                  // Les deux conditions données génèrent des équilibres. TableauRacine1&2 ont toujours 2 cases et z_vap est toujours dans la seconde.
            {
                deltaS = sMinusSStar(globales2,TabRacines2->donnees[1]) - sMinusSStar(globales1,TabRacines1->donnees[1]) + deltaSGP(cpCoeffs,globales1,globales2);        
            }
            break;
        default:
            printf("Numéro de valeur non reconnue. \n" );
            printf("Erreur dans hVapEOS.c -> Dans le switchcase(choixPhase), une valeur non reconnue a été renseignée malgré la vérification -> return -1 \n");
            return -1;
            break;
        }
    return (deltaS);




}
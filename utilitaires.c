#include "utilitaires.h"

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

void menuDefautPvap(varPR *globales)
{
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
                break;
        }
        printf("Pour changer une autre valeur, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
        printf("--> ");
        scanf("%d", &choixMenu);
    }
    return;
}

void chargerProfilGlobales(varPR *globales){
    int choixMenu, conversionBoolSI;
    double T, P;
    bool SI;

    printf("Veuillez saisir une température en K, puis, faites \"entrée\".\n");
    printf("--> ");
    scanf("%lf", &T); // %lf est le code qui indique que l'on souhaite un double.
    printf("Veuillez saisir une pression en bar, puis, faites \"entrée\".\n");
    printf("--> ");
    scanf("%lf", &P);
    printf("Souhaitez-vous faire la conversion en SI ? 0 pour non, 1 pour oui. \n");
    printf("--> ");
    scanf("%d", &conversionBoolSI); //Scanf n'a pas de format specifier pour les bool, à la place, on force la conversion int -> bool.
    SI = conversionBoolSI;

    choixMenu = -1; // initialisation
    printf("Plusieurs espèces et plusieurs plages de validité sont proposées.\n");
    printf("\n");
    printf("(1) Ethane \n");
    printf("(2) n-Heptane\n");
    printf("(0) Espèce customisée (Utilisateur devra rentrer les coefficients lui-même)\n");
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
            defautsGlobalesEthane(globales,T,P,SI);
            printf("Profil chargé.\n" );
            printf("\n");
            break;
        case 2:
            defautsGlobalesHeptane(globales,T,P,SI);
            printf("Profil chargé.\n" );
            printf("\n");
            break;
        default:
            printf("Vous avez choisi espèce customisée. \n" );
                globales->T1 = T;           
                globales->P1 = P;
                globales->Tc = 0;         
                globales->Pc = 0;           
                globales->Omega_A = 0.457236; 
                globales->Omega_B = 0.077796; 
                globales->acentric = 0;
                if (SI == true)
                {
                    globales->P1 = P*pow(10,5);
                    globales->Pc = 48.8*pow(10,5);
                }
                globales->Pr=globales->P1/globales->Pc;
                globales->Tr=globales->T1/globales->Tc;
                return;
            printf("Profil chargé.\n" );
            printf("\n");
            break;
    }
    globales->Pr=globales->P1/globales->Pc;
    globales->Tr=globales->T1/globales->Tc;
    return;

}

void menuDefautValeursZ(varPR *globales)
{
    int choixMenu;

    printf("Les valeurs par défaut sont : \n");
    printf("\n");

    printf("(1) T1 = %.3f K \n", globales->T1);

    printf("(2) P1 = %.3f bar \n", globales->P1);

    printf("(3) Tc = %.3f K \n", globales->Tc);

    printf("(4) Pc = %.3f bar \n", globales->Pc);

    printf("(5) Facteur acentric \"w\" = %.6f \n", globales->acentric);

    printf("(6) Omega_A = %.6f \n", globales->Omega_A);

    printf("(7) Omega_B = %.6f \n", globales->Omega_B);

    printf("\n");

    printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");

    scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenu != 0)
    {
        switch(choixMenu) {
            case 1:
                printf("T1 = " );
                scanf("%lf", &globales->T1); //%lf spécifie que l'on attend un double, & spécifie à quelle variable attribuer la valeur.
                printf("\n");
                break;
            case 2:
                printf("P1 = ");
                scanf("%lf", &globales->P1);
                break;
            case 3:
                printf("Tc = " );
                scanf("%lf", &globales->Tc);
                printf("\n");
                break;
            case 4:
                printf("Pc = " );
                scanf("%lf", &globales->Pc);
                printf("\n");
                break;
            case 5:
                printf("facteur acentric = " );
                scanf("%lf", &globales->acentric);
                printf("\n");
                break;
            case 6:
                printf("Omega_A = " );
                scanf("%lf", &globales->Omega_A);
                printf("\n");
                break;
            case 7:
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

void lectureAsciiArt(char *str){
    FILE *file;
    char line[1024]; //Longueur arbitraire, je suppose que cette longueur de ligne est suffisante pour afficher l'ASCII art.

    file = fopen(str,"r");
    if (file == NULL) {
        printf("Erreur ouverture fichier");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);
    return;
}

double integraleCp(double borneInf, double borneSup, double pasIntegration, double (*ptrFonction)(double,cpStruct*),cpStruct* cpCoeffs){ // On récupère comme paramètre la fonction à intégrer à travers son pointeur.
    double a, b;                                // Bornes des "sous-intégrales" pour chaque morceau d'intervalle
    double aire=0;                              // Aire sous la fonction
    int compteur = 0;
    int maxIter = 1000000000;                   // Pour éviter une boucle infinie, je la limite à 1 milliard d'itérations
    a = borneInf;                               // Initialisation de la première sous integrale au début de l'intervalle
    b = a+pasIntegration;                       // Initialisation de la première sous integrale
    while (b<borneSup && compteur < maxIter)
    {
        if ((borneSup-b)>=pasIntegration)       // Il reste au moins un pas à parcourir
        {
            aire = aire + ((b-a)/6)*(ptrFonction(a,cpCoeffs) + 4*(ptrFonction(((a+b)/2),cpCoeffs)) + ptrFonction(b,cpCoeffs));
            a=b;                                // On progresse dans l'intervalle
            b=a+pasIntegration;                 // On progresse dans l'intervalle
        }
        else{
            b = borneSup;                       // On évite de dépasser de l'intervalle
            aire = aire + ((b-a)/6)*(ptrFonction(a,cpCoeffs) + 4*(ptrFonction(((a+b)/2),cpCoeffs)) + ptrFonction(b,cpCoeffs));
            a=borneSup;                         // On a atteint le bout de l'intervalle -> a = borneSup
        }
        compteur=compteur+1;
    }
    return aire;
}

Tableau* creerTableauInt(int taille, char* localisation){ // Localisation permet de rentrer un string qui sera print comme localisation de l'erreur si celle-ci est rencontrée
Tableau *Tab= (Tableau*)malloc(sizeof(Tableau));
if (Tab == NULL) {
    printf("Erreur d'allocation mémoire -> %s \n", localisation); // %s prend un pointeur vers char (un string donc)
    instancierTableau(Tab,0);                       // Si erreur, alors je signale grace à taille = 0
}
else
{
    instancierTableau(Tab,taille);
}
return Tab;
}

varPR* creerGlobales(char* localisation){ // Localisation permet de rentrer un string qui sera print comme localisation de l'erreur si celle-ci est rencontrée
varPR *globales= (varPR*)malloc(sizeof(varPR));
if (globales == NULL) {
    printf("Erreur d'allocation mémoire -> %s \n", localisation); // %s prend un pointeur vers char (un string donc)
}
return globales;
}

antoineStruct* creerAntoine(char* localisation, void (*ptrFonction)(antoineStruct*)){ // obligé de faire appel de manière détournée à la fonction menuDefautAntoine(coeffs) car celle-ci est définie dans un fichier qui appelle utilitaires.h
    antoineStruct* coefficientsAntoine = (antoineStruct*)malloc(sizeof(antoineStruct));
    if (coefficientsAntoine == NULL) {
    printf("Erreur d'allocation mémoire -> %s \n", localisation);
    return 0;
    }
    ptrFonction(coefficientsAntoine);
    printf("\n");
    return coefficientsAntoine;
}

cpStruct* creerCpStruct(char* localisation){
    cpStruct *cpCoeffs = (cpStruct*)malloc(sizeof(cpStruct)); 
    if (cpCoeffs == NULL) {
    printf("Erreur d'allocation mémoire -> %s \n");
    return 0;
    }
    return cpCoeffs;
}

void defautsGlobalesEthane(varPR* globales, double T, double P, bool SI){
    globales->T1 = T;           
    globales->P1 = P;
    globales->Tc = 305.4;         
    globales->Pc = 48.8;           
    globales->Omega_A = 0.457236; 
    globales->Omega_B = 0.077796; 
    globales->acentric = 0.099;
    if (SI == true)
    {
        globales->P1 = P*pow(10,5);
        globales->Pc = 48.8*pow(10,5);
    }
    
    //globales->Tr = globales->T1/globales->Tc;     --> deplacé dans menuDefautValeurZ qui peut modifier T et P et donc Tr et Pr. Pas de sens de le mettre ici. 
    //globales->Pr = globales->P1/globales->Pc;     --> deplacé dans menuDefautValeurZ qui peut modifier T et P et donc Tr et Pr. Pas de sens de le mettre ici.
    //globales->alpha = trouveAlpha(globales);
    
    return;
}

void defautsGlobalesHeptane(varPR* globales, double T, double P, bool SI){
    globales->T1 = T;           
    globales->P1 = P;
    globales->Tc = 540.3;         
    globales->Pc = 27.4;           
    globales->Omega_A = 0.457236; 
    globales->Omega_B = 0.077796; 
    globales->acentric = 0.349;
    if (SI == true)
    {
        globales->P1 = P*pow(10,5);
        globales->Pc = 48.8*pow(10,5);
    }
    
    globales->Tr = globales->T1/globales->Tc;
    globales->Pr = globales->P1/globales->Pc;
    //globales->alpha = trouveAlpha(globales);
    
    return;
}

void menuDefautT2P2(varPR *globales, bool SI)
{
    int choixMenu;

    printf("Veuillez renseigner les valeurs de [T2;P2]. Les valeurs par défaut sont : \n");
    printf("\n");

    printf("(1) T2 = %.3f K \n", globales->T1);         // Attention à la confusion possible, T2 ici est bien stocké dans globales->T1 (simplement, il y a un globale pour le couple 1, et un globale pour le couple 2)

    if (SI == true)
    {
        printf("(2) P2 = %.3f bar \n", (globales->P1)*pow(10,-5));
    }
    else
    {
        printf("(2) P2 = %.3f bar \n", globales->P1);
    }
    
    

    printf("\n");

    printf("Pour changer une de ces valeurs, entrez le n° correspondant puis faites \"entrée\". Sinon, entrez 0. \n");
    printf("--> ");

    scanf("%d", &choixMenu); // %d est le code qui indique que l'on souhaite un entier.
    while (choixMenu != 0)
    {
        switch(choixMenu) {
            case 1:
                printf("T2 [K] = " );
                scanf("%lf", &globales->T1); //%lf spécifie que l'on attend un double, & spécifie à quelle variable attribuer la valeur.
                printf("\n");
                break;
            case 2:
                printf("P2 [bar] = ");
                scanf("%lf", &globales->P1);
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
    //globales->alpha = trouveAlpha(globales);
    return;
}
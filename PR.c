//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

#include "PR.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~ Définition des fonctions déclarées dans le header file "PR.h" ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ici on donne le contenu des fonctions dont on aura besoin. Ces fonctions peuvent s'appeler entre elles. */


Tableau* trouveZ(varPR* globales){ // Envisager de faire plutot une void qui manipule des valeurs dans globales

globales->Tr = globales->T1/globales->Tc;
//printf("globales->Tr = %.5f \n", globales->Tr);

globales->Pr = globales->P1/globales->Pc;
//printf("globales->Pc = %.5f \n", globales->Pc);

globales->alpha = trouveAlpha(globales);
//printf("globales->alpha = %.5f \n", globales->alpha);

globales->A = trouveA(globales);
//printf("globales->A = %.5f \n", globales->A);

globales->B = trouveB(globales);
//printf("globales->B = %.5f \n", globales->B);

PointsDepartNewton(TabBornesRacines,0.0,2,globales);
NewtonRaphson(TabRacines,TabBornesRacines,0.00001,globales);

if (TabRacines->taille != 1 && TabRacines->donnees[0] != -1000){
    //pas de racine --> donner un code d'erreur à Zliq et Zgaz

}


return 0;
};  


double trouveA(varPR* globales){
    double A;
    printf("globales->Pr = %f \n", globales->Pr);
    A = globales->Omega_A*globales->alpha*(globales->Pr/(globales->Tr*globales->Tr));
    printf("A = %f \n", A);
    return A;
}

double trouveB(varPR* globales){
    double B = globales->Omega_B*(globales->Pr/globales->Tr);
    return B;
}

double trouveAlpha(varPR* globales){
    double m;
    double racineAlpha;
    double alpha;
    m = 0.37464+(1.54226*globales->acentric)-(0.26992*globales->acentric*globales->acentric);
    racineAlpha = 1+m*(1-sqrt(globales->Tr));
    alpha = racineAlpha*racineAlpha;
    //printf("m = %.4f \n",m);
    //printf("alpha = %.4f \n",alpha);
    return alpha;
}

double PR(double Z, varPR* globales){
    double Valeur = Z*Z*Z+(globales->B-1)*Z*Z+(globales->A-2*globales->B-3*globales->B*globales->B)*Z-globales->B*(globales->A-globales->B-globales->B*globales->B);
    return Valeur;
}

double derivePR(double Z, varPR* globales){
    double derive = 3*Z*Z+2*(globales->B-1)*Z+(globales->A-2*globales->B-3*globales->B*globales->B);
    return derive;
}

void PointsDepartNewton(Tableau *tab, double borneInf, double borneSup, varPR* globales){ // L'astérisque indique que la fonction renverra un pointeur. Ce pointeur est une adresse mémoire.
    double pas[] = {0.1,0.01,0.001,0.0001}; // Remplacer le premier pas par un pas très grand (3.9 par ex. si l'intervalle des bornes est de 0 à 4) pour démontrer la fonction de sélection automatique du pas
    double abscisse, ordonnee, ordonneeTampon;
    int compteurIntervalle = 0;
    int compteurPas = 0;
    int i;

    while ((compteurIntervalle<6)&&(compteurPas <4)) //A l'issue de trois changements, compteurIntervalle = 6, donc on s'arrête de chercher d'autres racines. Un fois qu'on a testé la 3e valeur de pas (pas[2]), la boucle incrémente de 1 le compteur et on doit s'arrêter car pas[3] n'existe pas.
    {
        compteurIntervalle = 0; /*  Doit absolument être reset à chaque tour de boucle : Autrement on peut avoir un cas où on trouve un changement de signe au premier tour qui englobe 3 racines, puis avec un pas plus fin 
                                    on trouve un changement de signe qui englobe seulement les 2 premières + un changement de signe pour la troisième racine 
                                    -> on aura saturé le tableau sans avoir trois intervalles sans intersection. Donc on reset la progression et on recommence à écrire
                                    au début du tableau quand on change de pas. */ 
        abscisse = borneInf;
        ordonnee = PR(abscisse,globales);    // Initialisation de l'ordonnee
        printf("ordonnée = %f\n",ordonnee);
        if (ordonnee < 0.000000001 && ordonnee > -0.000000001){                      // Si jamais (quasi impossible) on tombe sur zero tout pile avec la premiere abscisse, on a déjà une première racine. 
            tab->donnees[compteurIntervalle] = abscisse - pas[compteurPas];   // Donc pour que la racine soit dans l'intervalle, on retire 1*pas ici..
            compteurIntervalle = compteurIntervalle +1;
            tab->donnees[compteurIntervalle] = abscisse + pas[compteurPas];   // et on ajoute 1*pas là.
            compteurIntervalle = compteurIntervalle +1;
        }
        abscisse = abscisse + pas[compteurPas];
        while ((abscisse <= borneSup + pas[compteurPas]) && (compteurIntervalle < 5)) {  /* A l'issue du 2e changement, compteurIntervalle vaut 4, a l'issue du 3e changement, il vaut 6. (je crois que je pouvais mettre compteurIntervalle < 6 mais ca marche comme ca) 
                                                                            Abscisse + pas au cas où le dernier changement de signe soit sur la borneSup */
        ordonneeTampon = PR(abscisse, globales);
        
        if (ordonneeTampon<0.000000001 && ordonneeTampon >-0.000000001 && tab->donnees[compteurIntervalle-1]!=abscisse && tab->donnees[compteurIntervalle-1]!=abscisse-pas[compteurPas]) /* Autrement dit, =0 (donc on a trouvé une racine) mais avec les doubles, un == pourrait poser problème. Normalement on a environ droit à 15 chiffres significatifs.
                                                                                                                                                                                            Je vérifie aussi qu'on vient pas de déclarer l'abscisse précédente (ou actuelle car c'est possible aussi) comme racine, sinon c'est juste qu'on la détecte en double à tort.*/
        {
            printf("ordonneeTampon ~= 0 --> Abscisse : %f, ordonnee : %.10f, ordonneeTampon : %.10f \n",abscisse,ordonnee,ordonneeTampon);
            tab->donnees[compteurIntervalle] = abscisse - pas[compteurPas];
            compteurIntervalle = compteurIntervalle +1;
            tab->donnees[compteurIntervalle] = abscisse + pas[compteurPas]; /* En réalité si ordonneeTampon vaut 0 c'est qu'on a trouvé une racine. Pour l'exercice je renvoie un intervalle 
                                                                    qui la contient en ajoutant le pas mais on pourrait directement la renvoyer en tant que telle. */
            compteurIntervalle = compteurIntervalle +1;
        }
        else if (ordonneeTampon*ordonnee<0 && tab->donnees[compteurIntervalle-1]!=abscisse && tab->donnees[compteurIntervalle-1]!=abscisse-pas[compteurPas]) {
            printf("ordonneeTampon*ordonnee<0 --> Abscisse : %f, ordonnee : %f, ordonneeTampon : %f \n",abscisse,ordonnee,ordonneeTampon);
            tab->donnees[compteurIntervalle] = abscisse - pas[compteurPas];
            compteurIntervalle = compteurIntervalle +1;
            tab->donnees[compteurIntervalle] = abscisse;
            compteurIntervalle = compteurIntervalle +1;
        }
        ordonnee = ordonneeTampon;
        abscisse = abscisse + pas[compteurPas];
        }
        compteurPas = compteurPas + 1;
    }
    
    if (compteurIntervalle != 6 && compteurIntervalle > 0) //On part de 0, si on trouve un changement on incrémente 2 fois de 1. Donc compteurIntervalle = 2 avec les cases 0 et 1 remplies. Même raisonnement jusqu'à 6.
    {
        double *NouvTableauAbscisses = (double*)realloc(tab->donnees,compteurIntervalle * sizeof(double)); // Donc si on a pas trouvé 3 racines, on renvoie un tableau qui contient le bon nombre de racines seulement.
        if (NouvTableauAbscisses != NULL) 
        {
            tab->donnees = NouvTableauAbscisses; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
            tab->taille = compteurIntervalle;
            printf("Reallocation de la mémoire, %i bornes \n", compteurIntervalle);
        }
        else{
            printf("Erreur de réallocation mémoire : tableau de 6 cases --> tableau de %d cases\n",compteurIntervalle);
        }
    }
    else if (compteurIntervalle == 0) // Si pas de racine dans l'intervalle
    {
        double *NouvTableauAbscisses = (double*)realloc(tab->donnees,2 * sizeof(double)); 
        if (NouvTableauAbscisses != NULL) 
        {
            tab->donnees = NouvTableauAbscisses; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
            tab->taille = 2;
            tab->donnees[0] = -1;
            tab->donnees[1] = -1;
            printf("PAS DE RACINE SUR INTERVALLE ABSCISSES -> Reallocation de la mémoire -> intervalle = [-1;-1] \n");
        }
        else{
            printf("Erreur de réallocation mémoire : tableau de 6 cases --> tableau de %d cases\n",compteurIntervalle);
        }
    }       
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

double trouveQZB(double Z, varPR* globales){
    double Q;
    //printf("Z = %f \n", Z);
    Q = (Z+(1+sqrt(2))*globales->B)/(Z+(1-sqrt(2))*globales->B);
    return Q;
}

double trouvePhi(double Z, double Q, varPR* globales){
    double phi = exp(Z-1-log(Z-globales->B)-((globales->A*sqrt(2)/(4*globales->B))*log(Q))); 
    return phi;
}

void NewtonRaphson(Tableau *tabResult, Tableau *tabIntervalles, double ecartZero, varPR* globales){
    int maxBoucle = 1000;
    int i;
    double x0, x1, dfx0, mini, maxi;
    
    if (tabIntervalles->donnees[0]==-1 && tabIntervalles->donnees[1]==-1)
    {
        printf("(!) pas de racine sur l'intervalle -> racine = -1000");
        tabResult->donnees[0] = -1000;
        return;
    }
    
    tabResult->taille = 0;
    for (i = 0; i < tabIntervalles->taille; i=i+2)
    {   
        x0 = tabIntervalles->donnees[i];
        dfx0 = derivePR(x0,globales);
        if (dfx0 < 0.000000001 && dfx0 > -0.000000001)
        {
            printf("Tangente horizontale, x0 = x0 - 0.1");
            x0 = x0 - 0.1;
        }
        
        x1=0.1; // Simplement pour rentrer dans la boucle
        while (fabs(PR(x1, globales))>ecartZero && i<maxBoucle)
        {
            x1 = x0 - (PR(x0, globales)/derivePR(x0, globales));
            x0 = x1;
        }
        tabResult->donnees[tabResult->taille]=x1;
        printf("Racine %i = %f \n",tabResult->taille+1, tabResult->donnees[tabResult->taille]);
        tabResult->taille=tabResult->taille+1;
    }
    if (tabIntervalles->taille == 2 && tabIntervalles->donnees[0]!=-1) // -1 correspond au code d'erreur renvoyé pour 0 racine. Ici on se place dans le cas d'une racine unique.
    {
    double *NouvTab = (double*)realloc(tabResult->donnees,1 * sizeof(double)); 
    if (NouvTab != NULL) 
    {
        tabResult->donnees = NouvTab; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
        tabResult->taille = 1;
        printf("Reallocation de la mémoire Newton racine unique\n");
    }
    else{
        printf("Erreur de réallocation mémoire fonction NewtonRaphton avec une seule racine.");
    }
    }
    else{
    mini = valeurMin(tabResult);
    maxi = valeurMax(tabResult);
    tabResult->donnees[0] = mini;
    tabResult->donnees[1] = maxi;
    double *NouvTab = (double*)realloc(tabResult->donnees,2 * sizeof(double)); 
    if (NouvTab != NULL) 
    {
        tabResult->donnees = NouvTab; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
        tabResult->taille = 2;
        printf("Reallocation de la mémoire Newton 2 racines\n");
    }
    else{
        printf("Erreur de réallocation mémoire fonction NewtonRaphton");
    }
    }
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

double trouvePsat(double tolerance, varPR* globales){
    double Preduit, QLiq, QVap, phiLiq, phiVap;
    double pas[] = {0.0001,0.00001,0.000001};
    int compteurPas;

    compteurPas = 0;
    Preduit = 0.000001;

    Tableau *TabBornesRacines = (Tableau*)malloc(sizeof(Tableau));
    instancierTableau(TabBornesRacines,6);

    Tableau *TabRacines = (Tableau*)malloc(sizeof(Tableau));
    instancierTableau(TabRacines,3);

    do
    {
        printf("Pas = %f \n",pas[compteurPas]);
        do
        {
            if (TabBornesRacines->taille != 6) // Si à l'issue de la dernière boucle on a redim le tableau, il faut le rétablir à 6 cases au cas où à la prochaine itération on ait 3 racines.
            {
                double *NouvTableauBornes = (double*)realloc(TabBornesRacines->donnees,6 * sizeof(double)); // Donc si on a pas trouvé 3 racines, on renvoie un tableau qui contient le bon nombre de racines seulement.
                if (NouvTableauBornes != NULL) 
                {
                    TabBornesRacines->donnees = NouvTableauBornes; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
                    TabBornesRacines->taille = 6;
                    printf("Reallocation de la mémoire\n");
                }
                else{
                    printf("Erreur de réallocation mémoire : Realloc du tableau des bornes des racines pour nouvelle tentative de pression reduite. \n");
                    return -1; // Pression absurde pour signaler le problème et éviter un crash.
                }
            }
            
            if (TabRacines->taille != 3) // Si à l'issue de la dernière boucle on a redim le tableau, il faut le rétablir à 3 cases au cas où à la prochaine itération on ait 3 racines.
            {
                double *NouvTableauRacines = (double*)realloc(TabRacines->donnees,3 * sizeof(double)); // Donc si on a pas trouvé 3 racines, on renvoie un tableau qui contient le bon nombre de racines seulement.
                if (NouvTableauRacines != NULL) 
                {
                    TabRacines->donnees = NouvTableauRacines; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
                    TabRacines->taille = 3;
                    printf("Reallocation de la mémoire\n");
                }
                else{
                    printf("Erreur de réallocation mémoire : Realloc du tableau des racines pour nouvelle tentative de pression reduite. \n");
                    return -1; // Pression absurde pour signaler le problème et éviter un crash.
                }
            }

            globales->A = trouveA(globales);
            //printf("globales->A = %.5f \n", globales->A);
            globales->B = trouveB(globales);
            //printf("globales->B = %.5f \n", globales->B);
            PointsDepartNewton(TabBornesRacines,0.0,2,globales);
            NewtonRaphson(TabRacines,TabBornesRacines,0.00001,globales);
            if (TabRacines->taille != 1 && TabRacines->donnees[0] != -1000)
            {
                //printf("abscisse 1 : %f \n",TabBornesRacines->donnees[0]);
                //printf("abscisse 2 : %f \n",TabBornesRacines->donnees[2]);
                //printf("Racine 1 : %f \n",TabRacines->donnees[0]);
                //printf("Racine 2 : %f \n",TabRacines->donnees[1]);
                QLiq = trouveQZB(TabRacines->donnees[0], globales);
                //printf("QLiq = %.5f \n", QLiq);
                QVap = trouveQZB(TabRacines->donnees[1], globales);
                //printf("QVap = %.5f \n", QVap);
                phiLiq = trouvePhi(TabRacines->donnees[0],QLiq, globales);
                //printf("phiLiq = %.5f \n", phiLiq);
                phiVap = trouvePhi(TabRacines->donnees[1],QVap, globales);
                //printf("phiVap = %.5f \n", phiVap);

                if ((100*fabs(phiLiq-phiVap))/phiLiq < tolerance)
                {
                    free(TabBornesRacines->donnees);
                    free(TabRacines->donnees);
                    free(TabBornesRacines);
                    free(TabRacines);
                    return Preduit;
                }
                else
                {
                    Preduit = Preduit + pas[compteurPas];
                }
            }
            else
            {
                Preduit = Preduit + pas[compteurPas];
            }
            
        } while (Preduit < 3); // la valeur 3 est arbitraire, cela semble très grand pour une pression reduite + le programme est limité à globales->Tr=1 pour lequel Pr vaut environ 1 aussi.
        compteurPas = compteurPas + 1;
    } while (compteurPas < 3);

    free(TabBornesRacines->donnees);
    free(TabRacines->donnees);
    free(TabBornesRacines);
    free(TabRacines);
    return -1; //Si malgré toutes les itérations et pas différents on ne répond pas au critère de tolérance, alors on renvoie Pr = -1.
    }


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
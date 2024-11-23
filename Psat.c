#include "Psat.h"
#include "PR.h"
#include "utilitaires.h" // Déjà appelé par les autres headers mais grâce à "#pragma once" ça n'est pas un problème.


double trouvePsat(double tolerance, varPR* globales){
    double QLiq, QVap, phiLiq, phiVap;
    double pas[] = {0.0001,0.00001,0.000001};
    int compteurPas;

    compteurPas = 0;
    globales->Pr = 0.000001;

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
            printf("A = %f \n",globales->acentric);
            printf("A = %f \n",globales->A);
            printf("B = %f \n",globales->B);
            //printf("Continuer ? -> appuyer sur entrer \n"); //Debugging
            //getchar(); //Debugging
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
                    return globales->Pr;
                }
                else
                {
                    globales->Pr = globales->Pr + pas[compteurPas];
                }
            }
            else
            {
                globales->Pr = globales->Pr + pas[compteurPas];
            }
            
        } while (globales->Pr < 3); // la valeur 3 est arbitraire, cela semble très grand pour une pression reduite + le programme est limité à globales->Tr=1 pour lequel Pr vaut environ 1 aussi.
        compteurPas = compteurPas + 1;
    } while (compteurPas < 3);

    free(TabBornesRacines->donnees);
    free(TabRacines->donnees);
    free(TabBornesRacines);
    free(TabRacines);
    return -1; //Si malgré toutes les itérations et pas différents on ne répond pas au critère de tolérance, alors on renvoie Pr = -1.
    }
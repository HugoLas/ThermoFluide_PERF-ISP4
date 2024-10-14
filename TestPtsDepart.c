#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>

typedef struct{     // Définit une structure "Tableau" qui permet d'obtenir à la fois les données du tableau mais aussi sa taille. (en fonction du nb. de racine, la taille change)
    int taille;
    double *donnees;
} Tableau;

void PointsDepartNewton(Tableau *tab, double borneInf, double borneSup, double ab[]); // La fonction prend en entrée l'adresse du tableau sur lequel travailler, les bornes de la fonction, ses paramètres.
double PR(double A, double B, double C, double D, double Z);
void instancierTableau(Tableau *tab, int taille);

int main(){
    printf("J'entre dans main \n");
    int boucle;
    double abg[4]; // prendre 1, -6, 11, -6 pour trois racines = 1, 2, 3; prendre 1, -5, 8, -4 pour deux racines = 1, 2 (penser à itérer le print du tableau jusqu'à 4 et pas 6); prendre 1 -3 3 -1 pour une racine = 1 (itérer jusqu'à 2)
    abg[0] = 1;
    abg[1] = -6;
    abg[2] = 11;
    abg[3] = -6; //remplacer par -5.9 pour des racines pas 'rondes' <=> qui tombent pas pile sur une valeur de pas
// ou
//    abg[0] = 1;
//    abg[1] = -5;
//    abg[2] = 8;
//    abg[3] = -4;
// ou
//    abg[0] = 1;
//    abg[1] = -3;
//    abg[2] = 3;
//    abg[3] = -1;


    printf("J'ai peuplé abg \n");

    Tableau *TabPointsDepart = (Tableau*)malloc(sizeof(Tableau));
    instancierTableau(TabPointsDepart,6);

    PointsDepartNewton(TabPointsDepart,0,4,abg); 

    for (boucle = 0; boucle < TabPointsDepart->taille; boucle++) // (ou boucle < 4 pour 2 racines ou boucle < 2 pour une racine)
    {
        printf("%.4f ",TabPointsDepart->donnees[boucle]);
        //printf("\n");
        //printf("%d ",boucle);
    }
    
    free(TabPointsDepart->donnees);
    free(TabPointsDepart);

    return 0;
}

void PointsDepartNewton(Tableau *tab, double borneInf, double borneSup, double ab[]){ // L'astérisque indique que la fonction renverra un pointeur. Ce pointeur est une adresse mémoire.
    double pas[] = {0.1,0.01,0.001}; // Remplacer le premier pas par un pas très grand (3.9 par ex. si l'intervalle des bornes est de 0 à 4) pour démontrer la fonction de sélection automatique du pas
    double abscisse, ordonnee, ordonneeTampon;
    int compteurIntervalle = 0;
    int compteurPas = 0;
    int i;

    while ((compteurIntervalle<6)&&(compteurPas <3)) //A l'issue de trois changements, compteurIntervalle = 6, donc on s'arrête de chercher d'autres racines. Un fois qu'on a testé la 3e valeur de pas (pas[2]), la boucle incrémente de 1 le compteur et on doit s'arrêter car pas[3] n'existe pas.
    {
        compteurIntervalle = 0; /*  Doit absolument être reset à chaque tour de boucle : Autrement on peut avoir un cas où on trouve un changement de signe au premier tour qui englobe 3 racines, puis avec un pas plus fin 
                                    on trouve un changement de signe qui englobe seulement les 2 premières + un changement de signe pour la troisième racine 
                                    -> on aura saturé le tableau sans avoir trois intervalles sans intersection. Donc on reset la progression et on recommence à écrire
                                    au début du tableau quand on change de pas. */ 
        abscisse = borneInf;
        ordonnee = PR(ab[0],ab[1],ab[2],ab[3],abscisse);    // Initialisation de l'ordonnee
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
        ordonneeTampon = PR(ab[0],ab[1],ab[2],ab[3],abscisse);
        
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
    
    if (compteurIntervalle != 6) //On part de 0, si on trouve un changement on incrémente 2 fois de 1. Donc compteurIntervalle = 2 avec les cases 0 et 1 remplies. Même raisonnement jusqu'à 6.
    {
        double *NouvTableauAbscisses = (double*)realloc(tab->donnees,compteurIntervalle * sizeof(double)); // Donc si on a pas trouvé 3 racines, on renvoie un tableau qui contient le bon nombre de racines seulement.
        if (NouvTableauAbscisses != NULL) 
        {
            tab->donnees = NouvTableauAbscisses; //Pas besoin de free quoique ce soit. Realloc a déjà libéré l'ancienne mémoire. Je crois que NouvTableauAbscisse demeure valable et est indispendable. En revanche, il n'y a bien qu'un seul espace mémoire d'alloué.
            tab->taille = compteurIntervalle;
            printf("Reallocation de la mémoire\n");
        }
        else{
            printf("Erreur de réallocation mémoire : tableau de 6 cases --> tableau de %d cases\n",compteurIntervalle);
        }
    }       
}

double PR(double A, double B, double C, double D, double Z){
    double Valeur = A*Z*Z*Z+B*Z*Z+C*Z+D;
    return Valeur;
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
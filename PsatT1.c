//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~ Déclaration des structures ~~~~~~~~~~~~~~~~~~~~~~~~~

/*Les structures sont des objets qui sont constitués d'attributs, par exemple on peut avoir une structure "personne" avec pour attributs un entier âge, et un entier taille.*/

typedef struct{     // Définit une structure "Tableau" qui permet d'obtenir à la fois les données du tableau mais aussi sa taille. (en fonction du nb. de racine, la taille change)
    int taille;
    double *donnees;
} Tableau;

//~~~~~~~~~~~~~~~~~~~~~~~~~ Déclaration des variables ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces variables appartiennent à l'espace de travail global. Elles existent durant la durée de vie du programme.
Par opposition, les variables locales (celles déclarées dans les fonctions) ne vivent que pendant l'execution de la fonction.*/

float T1;
float Tc;
float Pc;
float Omega_Ag; //Float a une précision de 7 chiffres, Omega fournit a 6 chiffres significatifs.
float Omega_Bg;
double Trg;
double Prg;
float acentric;
double ab[2]; // Contiendra les paramètres A et B pour Peng Robinson

//~~~~~~~~~~~~~~~~~~~~~~~~~ Déclarations des fonctions ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Déclare les fonctions avant main pour les signaler au compiler. */

double trouveA(float Omega_A, float acentric, double Tr, double Pr);
double trouveB(float Omega_B, double Tr, double Pr);
double trouveAlpha(float acentric, double Tr);
double PR(double A, double B, double Z);
double derivePR(double A, double B, double Z);
void PointsDepartNewton(Tableau *tab, double borneInf, double borneSup, double ab[]); // La fonction prend en entrée l'adresse du tableau sur lequel travailler, les bornes de la fonction, ses paramètres.
void instancierTableau(Tableau *tab, int taille);

//~~~~~~~~~~~~~~~~~~~~~~~~~ Programme principal ~~~~~~~~~~~~~~~~~~~~~~~~~

/* C'est cette fonction qui est appelée lors de l'exécution du programme. */

int main(){
printf("Valeur de T1 : \n");
scanf("%f", &T1); //%f spécifie que l'on attend un float, & spécifie à quelle variable attribuer la valeur.
printf("Valeur de Tc : \n");
scanf("%f", &Tc);
printf("Valeur de Pc : \n");
scanf("%f", &Pc);
printf("Valeur de Omega_A : \n");
scanf("%f", &Omega_Ag);
printf("Valeur de Omega_B : \n");
scanf("%f", &Omega_Bg);

Trg = T1/Tc;

return 0;
};  


//~~~~~~~~~~~~~~~~~~~~~~~~~ Définition des fonctions déclarées plus haut ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ici on donne le contenu des fonctions dont on aura besoin. Ces fonctions peuvent être appelées par le main
tout commme elles peuvent s'appeler entre elles. */

double trouveA(float Omega_A, float acentric, double Tr, double Pr){
    double A = Omega_A*trouveAlpha(acentric,Tr)*(Pr/(Tr*Tr));
    return A;
}

double trouveB(float Omega_B, double Tr, double Pr){
    double B = Omega_B*(Pr/Tr);
    return B;
}

double trouveAlpha(float acentric, double Tr){
    double m = 0.37464+(1.54226*acentric)-(0.26992*acentric*acentric);
    double alpha = (1+m*(1-sqrt(Tr)))*(1+m*(1-sqrt(Tr)));
    return alpha;
}

double PR(double A, double B, double Z){
    double Valeur = Z*Z*Z+(B-1)*Z*Z+(A-2*B-3*B*B)*Z-B*(A-B-B*B);
    return Valeur;
}

double derivePR(double A, double B, double Z){
    double derive = 3*Z*Z+2*(B-1)*Z+(A-2*B-3*B*B);
    return derive;
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
        ordonnee = PR(ab[0],ab[1],abscisse);    // Initialisation de l'ordonnee
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
        ordonneeTampon = PR(ab[0],ab[1],abscisse);
        
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

void instancierTableau(Tableau *tab, int taille){   // Cette fonction permet la création du tableau, associée à sa taille. Le tableau est alloué mais non peuplé.
    tab->donnees = (double*)malloc(taille*sizeof(double));
    tab->taille = taille;
    if (tab->donnees == NULL)
    {
        printf("Erreur de malloc : Structure tableau, allocation échouée");
        tab->taille = 0;
    }
}
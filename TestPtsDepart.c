#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>

double * PointsDepartNewton(double borneInf, double borneSup, double pas, double ab[]);
double PR(double A, double B, double C, double D, double Z);

int main(){
    printf("J'entre dans main \n");
    int boucle;
    double abg[4];
    abg[0] = 1;
    abg[1] = -6;
    abg[2] = 11;
    abg[3] = -5.9;
    printf("J'ai peuplé abg \n");
    double* MonTableau = PointsDepartNewton(0,4,0.1,abg); 

    for (boucle = 0; boucle < 6; boucle++)
    {
        printf("%.3f ",MonTableau[boucle]);
        //printf("\n");
        //printf("%d ",boucle);
    }
    

    return 0;
}

double * PointsDepartNewton(double borneInf, double borneSup, double pas, double ab[]){ // L'astérisque indique que la fonction renverra un pointeur. Ce pointeur est une adresse mémoire.
    double abscisse, ordonnee, ordonneeTampon;
    int compteurIntervalle = 0;
    int i;
    double* tableauAbscisses = (double*)malloc(6 * sizeof(double)); /*Alloue dans la mémoire un emplacement de 6 fois la taille d'un double, 
                                                                        pour stocker les 6 abscisses qui encadrent les racines. */
    if (tableauAbscisses == NULL) {
        printf("Erreur d'allocation mémoire !\n");
        return NULL;
    }
    else
    {
        printf("Allocation réussie \n");
    }
    
    abscisse = borneInf;
    ordonnee = PR(ab[0],ab[1],ab[2],ab[3],abscisse);    // Initialisation de l'ordonnee
    printf("ordonnée = %f\n",ordonnee);
    if (ordonnee == 0){                      // Si jamais (quasi impossible) on tombe sur zero tout pile avec la premiere abscisse, on a déjà une première racine. 
        tableauAbscisses[compteurIntervalle] = abscisse - pas;   // Donc pour que la racine soit dans l'intervalle, on retire 1*pas ici..
        compteurIntervalle = compteurIntervalle +1;
        tableauAbscisses[compteurIntervalle] = abscisse + pas;   // et on ajoute 1*pas là.
        compteurIntervalle = compteurIntervalle +1;
    }
    abscisse = abscisse + pas;
    while ((abscisse <= borneSup + pas) && (compteurIntervalle < 5)) {  /* A l'issue du 2e changement, compteurIntervalle vaut 4, a l'issue du 3e changement, il vaut 6. 
                                                                        Abscisse + pas au cas où le dernier changement de signe soit sur la borneSup */
     ordonneeTampon = PR(ab[0],ab[1],ab[2],ab[3],abscisse);
     
     if (ordonneeTampon<0.000001 && ordonneeTampon >-0.000001) // Autrement dit, =0 (donc on a trouvé une racine) mais avec les doubles un == pourrait poser problème.
     {
        printf("ordonneeTampon ~= 0 --> Abscisse : %f, ordonnee : %f, ordonneeTampon : %f \n",abscisse,ordonnee,ordonneeTampon);
        tableauAbscisses[compteurIntervalle] = abscisse - pas;
        compteurIntervalle = compteurIntervalle +1;
        tableauAbscisses[compteurIntervalle] = abscisse + pas; /* En réalité si ordonneeTampon vaut 0 c'est qu'on a trouvé une racine. Pour l'exercice je renvoie un intervalle 
                                                                qui la contient en ajoutant le pas mais on pourrait directement la renvoyer en tant que telle. */
        compteurIntervalle = compteurIntervalle +1;
     }
     else if (ordonneeTampon*ordonnee<0) {
        printf("ordonneeTampon*ordonnee<0 --> Abscisse : %f, ordonnee : %f, ordonneeTampon : %f \n",abscisse,ordonnee,ordonneeTampon);
        tableauAbscisses[compteurIntervalle] = abscisse - pas;
        compteurIntervalle = compteurIntervalle +1;
        tableauAbscisses[compteurIntervalle] = abscisse;
        compteurIntervalle = compteurIntervalle +1;
     }
     //else if ((ordonneeTampon*ordonnee==0) && (ordonnee != 0))
     //{
     //   printf("(ordonneeTampon*ordonnee==0) && (ordonnee != 0) --> Abscisse : %f, ordonnee : %f, ordonneeTampon : %f \n",abscisse,ordonnee,ordonneeTampon);
     //   tableauAbscisses[compteurIntervalle] = abscisse - pas;
     //   compteurIntervalle = compteurIntervalle +1;
     //   tableauAbscisses[compteurIntervalle] = abscisse + pas; /* En réalité si le produit vaut 0 c'est qu'on a trouvé une racine. Pour l'exercice je renvoie un intervalle 
     //                                                           qui la contient en ajoutant le pas mais on pourrait directement la renvoyer en tant que telle. */
     //   compteurIntervalle = compteurIntervalle +1;
     //}
     
     ordonnee = ordonneeTampon;
     abscisse = abscisse + pas;
    }
    return tableauAbscisses;
}

double PR(double A, double B, double C, double D, double Z){
    double Valeur = A*Z*Z*Z+B*Z*Z+C*Z+D;
    return Valeur;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire


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
double * PointsDepartNewton(int borneInf, int borneSup, double pas, double ab[]);


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

double * PointsDepartNewton(int borneInf, int borneSup, double pas, double ab[]){ // L'astérisque indique que la fonction renverra un pointeur. Ce pointeur est une adresse mémoire.
    double abscisse, ordonnee, ordonneeTampon;
    int compteurIntervalle = 0;
    int i;
    double* tableauAbscisses = (double*)malloc(6 * sizeof(double)); /*Alloue dans la mémoire un emplacement de 6 fois la taille d'un double, 
                                                                        pour stocker les 6 abscisses qui encadrent les racines. */
    if (tableauAbscisses == NULL) {
        printf("Erreur d'allocation mémoire !\n");
        return NULL;
    }
    abscisse = borneInf;
    ordonnee = PR(ab[0],ab[1],borneInf);    // Initialisation de l'ordonnee
    if (ordonnee = 0){                      // Si jamais (quasi impossible) on tombe sur zero tout pile avec la premiere abscisse, on a déjà une première racine. 
        tableauAbscisses[compteurIntervalle] = abscisse - pas;   // Donc pour que la racine soit dans l'intervalle, on retire 1*pas ici..
        compteurIntervalle = compteurIntervalle +1;
        tableauAbscisses[compteurIntervalle] = abscisse + pas;   // et on ajoute 1*pas là.
        compteurIntervalle = compteurIntervalle +1;
    }
    abscisse = abscisse + pas;
    while ((abscisse <= borneSup + pas) || (compteurIntervalle < 5)) {  /* A l'issue du 2e changement, compteurIntervalle vaut 4, a l'issue du 3e changement, il vaut 6. 
                                                                        Abscisse + pas au cas où le dernier changement de signe soit sur la borneSup */
     ordonneeTampon = PR(ab[0],ab[1],abscisse);
     if (ordonneeTampon*ordonnee<0) {
        tableauAbscisses[compteurIntervalle] = abscisse - pas;
        compteurIntervalle = compteurIntervalle +1;
        tableauAbscisses[compteurIntervalle] = abscisse;
        compteurIntervalle = compteurIntervalle +1;
     }
     ordonnee = ordonneeTampon;
     abscisse = abscisse + pas;
    }
    
    
}
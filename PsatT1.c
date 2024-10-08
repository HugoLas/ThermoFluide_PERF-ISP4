//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire


//~~~~~~~~~~~~~~~~~~~~~~~~~ Déclaration des variables ~~~~~~~~~~~~~~~~~~~~~~~~~

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

double trouveA(float Omega_A, float acentric, double Tr, double Pr); //déclare les fonctions avant main pour les signaler au compiler.
double trouveB(float Omega_B, double Tr, double Pr);
double trouveAlpha(float acentric, double Tr);
double PR(double A, double B, double Z);
double derivePR(double A, double B, double Z);
double * PointsDepartNewton(int borneInf, int borneSup, double ab[]);


//~~~~~~~~~~~~~~~~~~~~~~~~~ Programme principal ~~~~~~~~~~~~~~~~~~~~~~~~~

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

double * PointsDepartNewton(int borneInf, int borneSup, double ab[]){
    double abscisse, ordonnee, ordonneeTampon;
    int compteurChangement = 0;
    abscisse = borneInf;
    while abscisse <= borneSup || compteurChangement < 3 {
     ordonneeTampon = PR(ab[0],ab[1],abscisse);
     if ordonneeTampon*ordonnee<0
     
    }
    
    
}
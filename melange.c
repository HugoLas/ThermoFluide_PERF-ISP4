#include "melange.h"

double trouveACompose(double A1, double A2, double k12){
return((1-k12)*sqrt(A1*A2));
}

BMelange* trouveBMelange(titresMelange* titre, varPR* globalesEspece1, varPR* globalesEspece2, char* localisation){ // Localisation permet d'afficher l'emplacement de l'erreur sous forme de string en cas de probleme d'allocation memoire
    BMelange *BMel= (BMelange*)malloc(sizeof(BMelange));
    if (BMel == NULL) {
        printf("Erreur d'allocation mémoire -> %s \n", localisation); // %s prend un pointeur vers char (un string donc)
    }
    BMel->Bliq=titre->x1*trouveB(globalesEspece1)+titre->x2*trouveB(globalesEspece2);
    BMel->Bvap=titre->y1*trouveB(globalesEspece1)+titre->y2*trouveB(globalesEspece2);
    return(BMel);
}

AMelange* trouveAMelange(titresMelange* titre, varPR* globalesEspece1, varPR* globalesEspece2, double k12, char* localisation){ // Localisation permet d'afficher l'emplacement de l'erreur sous forme de string en cas de probleme d'allocation memoire
    AMelange *AMel= (AMelange*)malloc(sizeof(AMelange));
    if (AMel == NULL) {
        printf("Erreur d'allocation mémoire -> %s \n"); // %s prend un pointeur vers char (un string donc)
    }
    //printf("globalesEspece1->Tr = %.3f \n",globalesEspece1->Tr);
    //printf("globalesEspece1->Pr = %.3f \n",globalesEspece1->Pr);
    //printf("globalesEspece2->Tr = %.3f \n",globalesEspece2->Tr);
    //printf("globalesEspece2->Pr = %.3f \n",globalesEspece2->Pr);
    double A1 = trouveA(globalesEspece1);
    
    //printf("globalesEspece1->alpha = %.3f \n",globalesEspece1->alpha);
    // printf("trouveA(globalesEspece1) = %.3f \n",trouveA(globalesEspece1));
    double A2 = trouveA(globalesEspece2);
    // printf("trouveA(globalesEspece2) = %.3f \n",trouveA(globalesEspece2));
    AMel->Aliq = pow(titre->x1,2)*A1+pow(titre->x2,2)*A2+2*titre->x1*titre->x2*trouveACompose(A1,A2,k12);
    AMel->Avap = pow(titre->y1,2)*A1+pow(titre->y2,2)*A2+2*titre->y1*titre->y2*trouveACompose(A1,A2,k12);
    return(AMel);
}

double trouvePhiMelange(varPR* globaleEspece, double Z, double Bi, double B, double A, double A_i, double A2, double titre_i, double titre2, double k12){
    //  Pour comprendre, générer l'équation annotée ci-dessous (LaTeX) :
    //  \phi_{i}=\exp\left[\underbrace{\frac{B_{i}}{B}\left(Z-1\right)}_{\text{bloc1}}-\underbrace{\ln\left(Z-B\right)-\frac{A\sqrt{2}}{4B}}_{\text{bloc2}}\times\left(\underbrace{\frac{2}{A}\times\sum_{j=1}^{2}\left(z_{j}A_{ij}\right)-\frac{B_{i}}{B}}_{\text{bloc3}}\right)\times\ln\left(\underbrace{\frac{Z+\left(1+\sqrt{2}\right)\times B}{Z+\left(1-\sqrt{2}\right)\times B}}_{\text{\ensuremath{\text{bloc4}}}}\right)\right]
    
    /*Ici, B_{i} fait référence au B de la page 66 du cours \left(\frac{\Omega_{B}P_{R}}{T_{R}}\right).
    B fait référence au B_{\text{mélange}} de la page 133. En réalité, il faut choisir sa phase (liquide ou vapeur). 
    De même pour Z, il s'agit du Z_{\text{mélange}} et il faut aussi choisir sa phase.*/
    //printf("bloc1 = ((Bi/B)*(Z-1))-log(Z-B) \n");
    //printf("bloc1 = ((%.3f/%.3f)*(%.3f-1))-log(%.3f-%.3f) \n",Bi,B,Z,Z,B);
    //printf("\n");
    double bloc1 = ((Bi/B)*(Z-1))-log(Z-B);
    //printf("bloc2 = (A*sqrt(2))/(4*B) \n");
    //printf("bloc2 = (%.3f*sqrt(2))/(4*%.3f) \n",A,B);
    //printf("\n");
    double bloc2 = (A*sqrt(2))/(4*B);
    //printf("bloc3 = (2/A)*(titre_i*A_i+titre2*trouveACompose(A_i,A2,k12))-(Bi/B) \n");
    //printf("bloc3 = (2/%.3f)*(%.3f*%.3f+%.3f*%.3f)-(%.3f/%.3f) \n",A,titre_i,A_i,titre2,trouveACompose(A_i,A2,k12),Bi,B);
    //printf("\n");
    double bloc3 = (2/A)*(titre_i*A_i+titre2*trouveACompose(A_i,A2,k12))-(Bi/B);
    //printf("bloc4 = (Z+(1+sqrt(2))*B)/(Z+(1-sqrt(2))*B) \n");
    //printf("bloc4 = (%.3f+(1+sqrt(2))*%.3f)/(%.3f+(1-sqrt(2))*%.3f) \n",Z,B,Z,B);
    //printf("\n");
    double bloc4 = (Z+(1+sqrt(2))*B)/(Z+(1-sqrt(2))*B);
    //printf("return = exp(bloc1-(bloc2*bloc3*log(bloc4))) \n");
    //printf("return = exp(%.3f-(%.3f*%.3f*log(%.3f))) \n",bloc1,bloc2,bloc3,bloc4);
    //printf("\n");
    return(exp(bloc1-(bloc2*bloc3*log(bloc4))));
}

void titresEnFonctionDeK(titresMelange *titre, double K1, double K2){
    titre->x1 = (K2+1)/(K1+K2);
    titre->x2 = 1-titre->x1;
    titre->y1 = (K1*(K2-1))/(K2-K1);
    titre->y2 = 1-titre->y1;
}

void actualiserTitres(titresMelange *titre, titresMelange *titreSuivant){
    titre->x1 = titreSuivant->x1;
    titre->x2 = titreSuivant->x2;
    titre->y1 = titreSuivant->y1;
    titre->y2 = titreSuivant->y2;    
}

double ecartMoyenTitres(titresMelange *titre, titresMelange *titreSuivant){
    double ecartX1 = fabs(titreSuivant->x1-titre->x1);
    double ecartX2 = fabs(titreSuivant->x2-titre->x2);
    double ecartY1 = fabs(titreSuivant->y1-titre->y1);
    double ecartY2 = fabs(titreSuivant->y2-titre->y2);
    return(100*(ecartX1+ecartX2+ecartY1+ecartY2)/4);
}

titresMelange* trouveTitres(varPR* globaleEspece1, varPR* globaleEspece2, double P, double T, double k12, antoineStruct* coeffsAntoine1, antoineStruct* coeffsAntoine2, double critere){
    double K1, K2, phi1Liq, phi1Vap, phi2Liq, phi2Vap, B1, B2, A1, A2;
    long compteur = 0; 
    long maxIter = 10000;
    AMelange *AMel;
    BMelange *BMel;
    
    titresMelange *titre = (titresMelange*)malloc(sizeof(titresMelange));
    if (titre == NULL) {
        printf("Erreur d'allocation mémoire -> melange.c -> trouveTitres -> allocation de 'titre' \n");
    }
    titresMelange *titreSuivant = (titresMelange*)malloc(sizeof(titresMelange));
    if (titreSuivant == NULL) {
        printf("Erreur d'allocation mémoire -> melange.c -> trouveTitres -> allocation de 'titreSuivant' \n");
    }
    Tableau* TabBornesRacinesLiq = creerTableauInt(6,"melange.c -> trouveTitres -> allocation de 'TabBornesRacinesLiq'");
    Tableau* TabRacinesLiq = creerTableauInt(3, "melange.c -> trouveTitres -> allocation de 'TabRacinesLiq'");
    Tableau* TabBornesRacinesVap = creerTableauInt(6,"melange.c -> trouveTitres -> allocation de 'TabBornesRacinesVap'");
    Tableau* TabRacinesVap = creerTableauInt(3, "melange.c -> trouveTitres -> allocation de 'TabRacinesVap'");


    K1 = antoineFormula(coeffsAntoine1, T);
    K2 = antoineFormula(coeffsAntoine2, T);
    
    //K1 = 1.899;
    //K2 = 0.0373;

    //K1=2;
    //K2=0.01;

    //K2 = 1.7;
    //K1 = 0.6;

    //K1 = 6.10;
    //K2 = 0.00759;


    //titre->x1 = 0;
    //titre->x2 = 0;
    //titre->y1 = 0;
    //titre->y2 = 0;
    memset(titre, 0, sizeof(titresMelange)); // Initialise à 0 tous les titres pour que la condition d'entrée dans la boucle soit vraie.
    titresEnFonctionDeK(titreSuivant,K1,K2);
    //printf("Debugging, premiers titres : \n");
    //printf("x1 = %.3f \n",titreSuivant->x1);
    //printf("y1 = %.3f \n",titreSuivant->y1);
    //printf("x2 = %.3f \n",titreSuivant->x2);
    //printf("y2 = %.3f \n",titreSuivant->y2);

    //printf("ecartMoyenTitres(titre, titreSuivant) = %.3f \n", ecartMoyenTitres(titre, titreSuivant));
    //printf("critere = %.3f \n", critere);
    //printf("%d \n", (ecartMoyenTitres(titre, titreSuivant) > critere) && (compteur < maxIter));

    while ((ecartMoyenTitres(titre, titreSuivant) > critere) && (compteur < maxIter))
    {
        //printf("entrée dans la boucle --------------------------------------------------------\n");
        //printf("ecartMoyenTitres(titre, titreSuivant) = %.3f \n", ecartMoyenTitres(titre, titreSuivant));
        actualiserTitres(titre,titreSuivant);
        AMel = trouveAMelange(titreSuivant, globaleEspece1, globaleEspece2, k12, "melange.c -> trouveTitres -> trouveAMelange");
        //printf("Calcul BMel : trouveBMelange(titreSuivant, globaleEspece1->B, globaleEspece2->B) \n");
        //printf("Calcul BMel : trouveBMelange(%.3f, %.3f, %.3f, %.3f) \n",titreSuivant->x1, titreSuivant->x2,trouveB(globaleEspece1), trouveB(globaleEspece2));
        BMel = trouveBMelange(titreSuivant, globaleEspece1, globaleEspece2, "melange.c -> trouveTitres -> trouveBMelange");
        trouveZMelange(AMel,BMel,TabBornesRacinesLiq,TabRacinesLiq,0);
        //printf("TabRacinesLiq->taille = %d \n",TabRacinesLiq->taille);
        trouveZMelange(AMel,BMel,TabBornesRacinesVap,TabRacinesVap,1);
        //printf("TabRacinesVap->taille = %d \n",TabRacinesVap->taille);
        //printf("ZliqMel = %.3f \n", valeurMin(TabRacinesLiq));
        phi1Liq = trouvePhiMelange(globaleEspece1,valeurMin(TabRacinesLiq),trouveB(globaleEspece1),BMel->Bliq,AMel->Aliq,trouveA(globaleEspece1),trouveA(globaleEspece2),titre->x1,titre->x2,k12);
        //printf("phi1Liq = %.3f \n", phi1Liq);
        //printf("ZVapMel = %.3f \n", valeurMax(TabRacinesVap));
        phi1Vap = trouvePhiMelange(globaleEspece1,valeurMax(TabRacinesVap),trouveB(globaleEspece1),BMel->Bvap,AMel->Avap,trouveA(globaleEspece1),trouveA(globaleEspece2),titre->y1,titre->y2,k12);
        //printf("phi1Vap = %.3f \n", phi1Vap);
        phi2Liq = trouvePhiMelange(globaleEspece2,valeurMin(TabRacinesLiq),trouveB(globaleEspece2),BMel->Bliq,AMel->Aliq,trouveA(globaleEspece2),trouveA(globaleEspece1),titre->x2,titre->x1,k12);
        //printf("phi2Liq = %.3f \n", phi2Liq);
        phi2Vap = trouvePhiMelange(globaleEspece2,valeurMax(TabRacinesVap),trouveB(globaleEspece2),BMel->Bvap,AMel->Avap,trouveA(globaleEspece2),trouveA(globaleEspece1),titre->y2,titre->y1,k12);
        //printf("phi2Vap = %.3f \n", phi2Vap);

        K1 = phi1Liq/phi1Vap;
        K2 = phi2Liq/phi2Vap;
        //printf("K1 = %.3f \n", K1);
        //printf("K2 = %.3f \n", K2);
        titresEnFonctionDeK(titreSuivant,K1,K2);
        
        //printf("Debugging, titre fin de boucle : \n");
        //printf("x1 = %.3f \n",titreSuivant->x1);
        //printf("y1 = %.3f \n",titreSuivant->y1);
        //printf("x2 = %.3f \n",titreSuivant->x2);
        //printf("y2 = %.3f \n",titreSuivant->y2);

        compteur = compteur + 1;

    }
    
    //printf("Je suis ici... \n");

    free(titre);
    free(AMel);
    free(BMel);
    free(TabBornesRacinesLiq->donnees);
    free(TabBornesRacinesLiq);
    free(TabBornesRacinesVap->donnees);
    free(TabBornesRacinesVap);
    free(TabRacinesLiq->donnees);
    free(TabRacinesLiq);
    free(TabRacinesVap->donnees);
    free(TabRacinesVap);

    return(titreSuivant);
}

double tiret(double titre1, double titre2, varPR* globales1, varPR* globales2, double k12, double AMel){
    // Pour comprendre : 1+\frac{1}{A}\times\left[\underbrace{x_{1}m_{1}\sqrt{\frac{T_{R,1}}{\alpha_{1}}}\times\left(x_{1}A_{1}+x_{2}A_{12}\right)}_{A}+\underbrace{x_{2}m_{2}\sqrt{\frac{T_{R,2}}{\alpha_{2}}}\times\left(x_{1}A_{21}+x_{2}A_{2}\right)}_{B}\right]
    // Explications page 138 du cours.
    double m1 = 0.37464+(1.54226*globales1->acentric)-(0.26992*globales1->acentric*globales1->acentric);
    double m2 = 0.37464+(1.54226*globales2->acentric)-(0.26992*globales2->acentric*globales2->acentric);

    double P = titre1*m1*sqrt(globales1->Tr/globales1->alpha)*(titre1*trouveA(globales1)+titre2*trouveACompose(trouveA(globales1),trouveA(globales2),k12));
    double Q = titre2*m2*sqrt(globales2->Tr/globales2->alpha)*(titre1*trouveACompose(trouveA(globales1),trouveA(globales2),k12)+titre2*trouveA(globales2));

    return (1+(1/AMel)*(P+Q));
}

double hMinusHStarMelange(double Z, double AMel, double BMel, double T, double titre1, double titre2, varPR* globales1, varPR* globales2, double k12){
    
    return(8.314*T*((Z-1) - (AMel*sqrt(2)/(4*BMel))*tiret(titre1,titre2,globales1,globales2,k12,AMel)*log((Z + BMel*(1+sqrt(2))) / (Z + BMel*(1+sqrt(2))))));
}

void enthalpieMelange(titresMelange* titres, varPR* globales1, varPR* globales2, double k12, cpStruct* cpCoeffs1, cpStruct* cpCoeffs2){
    
    Tableau* TabBornesRacinesLiq = creerTableauInt(6,"melange.c -> trouveTitres -> allocation de 'TabBornesRacinesLiq'");
    Tableau* TabRacinesLiq = creerTableauInt(3, "melange.c -> trouveTitres -> allocation de 'TabRacinesLiq'");
    Tableau* TabBornesRacinesVap = creerTableauInt(6,"melange.c -> trouveTitres -> allocation de 'TabBornesRacinesVap'");
    Tableau* TabRacinesVap = creerTableauInt(3, "melange.c -> trouveTitres -> allocation de 'TabRacinesVap'");

    AMelange* AMel = trouveAMelange(titres,globales1,globales2,k12,"enthalpieMelange -> AMel");
    BMelange* BMel = trouveBMelange(titres,globales1,globales2,"enthalpieMelange -> BMel");
 
    trouveZMelange(AMel,BMel,TabBornesRacinesLiq,TabRacinesLiq,0);
    double ZmelLiq = valeurMin(TabRacinesLiq);
    printf("ZmelLiq = %.3f \n",ZmelLiq);
    trouveZMelange(AMel,BMel,TabBornesRacinesVap,TabRacinesVap,1);
    double ZmelVap = valeurMax(TabRacinesVap);
    printf("ZmelVap = %.3f \n",ZmelVap);

    double hMinusHStarLiq = hMinusHStarMelange(ZmelLiq,AMel->Aliq,BMel->Bliq,globales1->T1,titres->x1,titres->x2,globales1,globales2,k12); 
    double hMinusHStarVap = hMinusHStarMelange(ZmelVap,AMel->Avap,BMel->Bvap,globales1->T1,titres->y1,titres->y2,globales1,globales2,k12); 
    printf("\n");
    printf("h moins h* phase liquide = %.3f \n", hMinusHStarLiq);
    printf("h moins h* phase vapeur = %.3f \n", hMinusHStarVap);

    double hStar1 = 0 + (-1)*integraleCp(globales1->T1,298.15,0.0003,fonctionCpVap,cpCoeffs1); // Je permute les bornes car la t° de réf imposée dans la consigne > t° du problème considéré (293K)
    double hStar2 = 0 + (-1)*integraleCp(globales2->T1,298.15,0.0003,fonctionCpVap,cpCoeffs2);

    printf("\n");
    printf("h*1 = %.3f \n", hStar1);
    printf("h*2 = %.3f \n", hStar2);


    double hStarLiq = titres->x1*hStar1+titres->x2*hStar2;
    double hStarVap = titres->y1*hStar1+titres->y2*hStar2;

    printf("\n");
    printf("h*Liq = %.3f \n", hStarLiq);
    printf("h*Vap = %.3f \n", hStarVap);

    double hMelangeLiq = hMinusHStarLiq + hStarLiq;
    double hMelangeVap = hMinusHStarVap + hStarVap;
    printf("\n");
    printf("h mélange en phase liquide = %.3f \n", hMelangeLiq);
    printf("h melange en phase vapeur = %.3f \n", hMelangeVap);

    return;
}
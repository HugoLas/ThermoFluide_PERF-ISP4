#include "hVapEOS.h"

double hVapValue(varPR* globales, Tableau* TabBornesRacines, Tableau* TabRacines){
    double ecartGpVap, ecartGpLiq;
    trouveZ(globales,TabBornesRacines,TabRacines);
    ecartGpVap = hMinusHStar(globales, TabRacines->donnees[1]);
    ecartGpLiq = hMinusHStar(globales, TabRacines->donnees[0]);
    return (ecartGpVap-ecartGpLiq);
}

double hMinusHStar(varPR* globales, double z){
    double m;
    m = 0.37464+(1.54226*globales->acentric)-(0.26992*globales->acentric*globales->acentric);
    return (8.314*globales->T1*(z-1-((globales->A*sqrt(2))/(4*globales->B))*(1+(m*sqrt(globales->Tr/globales->alpha)))*log(trouveQZB(z, globales))));
}



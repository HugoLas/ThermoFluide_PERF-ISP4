//~~~~~~~~~~~~~~~~~~~~~~~~~ Appel des librairies ~~~~~~~~~~~~~~~~~~~~~~~~~

/* Ces librairies donnent accès à des fonctions déjà programmées par des gens talentueux
et nous évite de les reprogrammer (ce qui irait au delà de nos compétences) */

#include <math.h>   // Permet d'effectuer des calculs
#include <stdlib.h> // Permet l'allocation dynamic de mémoire
#include <stdio.h>
#include "PR.h"
#include "utilitaires.h"
#include "Psat.h"
#include "antoine.h"
#include "hVapWatson.h"
#include "hVapEOS.h"
#include "masseVolumique.h"
#include "entropie.h"


int main(){
int choixMenuPrincipal;
double Psaturation; //envisager d'en faire une allocation dynamique uniquement appelée dans case 1 et 2
lectureAsciiArt("art.txt");
printf("\n\n");

do
{
    printf("Que souhaitez-vous calculer ?\n");
    printf("\n");
    printf("(1) La pression de vapeur saturante pour une température ? (Equation d'état, Peng-Robinson) \n");
    printf("(2) La pression de vapeur saturante pour une température ? (Formule d'Antoine) \n");
    printf("(3) Les valeurs de facteur de compressibilité (Z) pour une substance, pour une température ? (Peng-Robinson) \n");
    printf("(4) L'enthalpie de vaporisation pour une température donnée. (Corrélations de Riedel et de Watson) \n");
    printf("(5) L'enthalpie de vaporisation pour un couple température/pression donné. (Equation d'état, Peng-Robinson) \n");
    printf("(6) La masse volumique d'un liquide pour un couple température/pression donné. (Equation d'état, Peng-Robinson) \n");
    printf("(7) La différence d'enthalpie entre 2 couples {T,P} donnés. (Equation d'état, Peng-Robinson) \n");
    printf("(8) La différence d'entropie entre 2 couples {T,P} donnés. (Equation d'état, Peng-Robinson) \n");
    printf("(9) La différence de volume molaire entre 2 couples {T,P} donnés. (Equation d'état, Peng-Robinson) \n");
    printf("(0) Pour quitter\n");
    printf("\n");

    printf("--> ");
    scanf("%d", &choixMenuPrincipal); // %d est le code qui indique que l'on souhaite un entier.

    switch(choixMenuPrincipal) {
        case 1:
            //varPR *globalesPsat = (varPR*)malloc(sizeof(varPR)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de Psat.c. La structure en question est définie dans utilitaires.h
            //if (globalesPsat == NULL) {
            //printf("Erreur d'allocation mémoire -> main, allocation globalesPr.\n");
            //return 0;
            //}

            varPR *globalesPsat;
            globalesPsat = creerGlobales("programme principal -> globalesPsat dans cas n°1 \n");
            
            globalesPsat->T1 = 173;           // Valeurs par défaut.
            globalesPsat->Tc = 305.4;         // Valeurs par défaut.
            globalesPsat->Pc = 48.8;          // Valeurs par défaut.
            globalesPsat->Omega_A = 0.457236; // Valeurs par défaut.
            globalesPsat->Omega_B = 0.077796; // Valeurs par défaut.
            globalesPsat->acentric = 0.099;   // Valeurs par défaut.
            menuDefautPvap(globalesPsat);
            globalesPsat->Tr = globalesPsat->T1/globalesPsat->Tc;
            globalesPsat->alpha = trouveAlpha(globalesPsat);
            globalesPsat->Pr = trouvePsat(1, globalesPsat);
            Psaturation = globalesPsat->Pr*globalesPsat->Pc;
            printf("Pression de vapeur saturante à T = %.2f K vaut %.4f bar.\n", globalesPsat->T1, Psaturation);
            
            free(globalesPsat);
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            printf("\n");
            break;

        case 2:
            double *temperatureAntoine = (double*)malloc(sizeof(double));
            if (temperatureAntoine == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation temperature.\n");
            return 0;
            }
            
            antoineStruct *coefficientsAntoine = (antoineStruct*)malloc(sizeof(antoineStruct));
            if (coefficientsAntoine == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation coefficientsAntoine.\n");
            return 0;
            }
            menuDefautAntoine(coefficientsAntoine);
            
            // TODO : sécuriser la saisie en mettant une boucle en cas de mauvaise saisie
            printf("Choix de la température [K] : (Sélectionnez une valeur cohérente avec la plage de validité des coefficients !)\n");
            printf("--> ");
            scanf("%lf", temperatureAntoine);
            printf("Pression de vapeur saturante à T = %.2f K vaut %.4f bar.\n", *temperatureAntoine, antoineFormula(coefficientsAntoine,*temperatureAntoine));
            
            free(coefficientsAntoine);
            free(temperatureAntoine);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;

        case 3:
            varPR *globalesPr = creerGlobales("programme principal -> globalesPr dans cas n°3 \n");
            defautsGlobalesEthane(globalesPr,173,0.5271,false);
            menuDefautValeursZ(globalesPr);
            Tableau *TableauBornesRacinesPR = creerTableauInt(6,"Programme principal -> cas N°3 -> TableauBornesRacinesPR\n");
            Tableau *TableauRacinesPR = creerTableauInt(3, "Programme Principal -> cas n°3 -> Tableau TableauRacinesPR");
            trouveZ(globalesPr, TableauBornesRacinesPR, TableauRacinesPR);
            
            if (TableauRacinesPR->taille==1)
            {
                printf("Pas d'équilibre liquide vapeur, Z = %.4f \n",TableauRacinesPR->donnees[0]);
            }
            else if ((TableauRacinesPR->taille==2) && (TableauRacinesPR->donnees[0]==-1))
            {
                printf("Erreur lors de la recherche du couple Z, erreur provient de NewtonRaphson.");
            }
            else
            {
                printf("Equilibre liquide vapeur, Z_liq = %.4f, Z_vap = %.4f \n",TableauRacinesPR->donnees[0],TableauRacinesPR->donnees[1]);
            }
            
            free(TableauBornesRacinesPR->donnees);
            free(TableauRacinesPR->donnees);
            free(TableauBornesRacinesPR);
            free(TableauRacinesPR);
            free(globalesPr);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;

        case 4:
            double *temperatureWatson = (double*)malloc(sizeof(double));
            if (temperatureWatson == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation temperature.\n");
            return 0;
            }
            
            watsonStruct *parametres = (watsonStruct*)malloc(sizeof(watsonStruct));
            if (parametres == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation parametres Watson.\n");
            return 0;
            }
            menuDefautHvapWatson(parametres);
            
            // TODO : sécuriser la saisie en mettant une boucle en cas de mauvaise saisie
            printf("Choix de la température [K] : \n");
            printf("--> ");
            scanf("%lf", temperatureWatson);
            printf("Enthalpie de vaporisation à T = %.2f K vaut %.6f J/mol",*temperatureWatson, watsonFormula(parametres, *temperatureWatson));
            
            free(parametres);
            free(temperatureWatson);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar(); 
            break;
        case 5:
            varPR *globalesHVap = creerGlobales("Programme Principal -> cas n°5 -> globalesHVap");
            defautsGlobalesEthane(globalesHVap,173,0.5271,false);
            menuDefautValeursZ(globalesHVap);
            Tableau *TableauBornesRacinesHVap = creerTableauInt(6,"Programme Principal -> cas n°5 -> TableauBornesRacinesHVap");
            Tableau *TableauRacinesHVap = creerTableauInt(3,"Programme Principal -> cas n°5 -> TableauRacinesHVap");

            printf("Enthalpie de vaporisation à T = %.2f K vaut %.6f J/mol",globalesHVap->T1,hVapValue(globalesHVap, TableauBornesRacinesHVap,TableauRacinesHVap));
            
            free(TableauBornesRacinesHVap->donnees);
            free(TableauRacinesHVap->donnees);
            free(TableauBornesRacinesHVap);
            free(TableauRacinesHVap);
            free(globalesHVap);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;
        case 6:
            double *masseMolaire = (double*)malloc(sizeof(double));
            if (masseMolaire==NULL){
                printf("Erreur d'allocation mémoire -> main, allocation de masseMolaire pour le cas n°6 \n");
                return 0;
            }
            *masseMolaire = (double)30/1000; // /1000 car on veut des kg.mol^-1 pour fonctionner en S.I. Je cast (double) sur le numérateur pour forcer la division à ne pas être entière. (autrement le résultat =0)
            
            varPR *globalesRho = creerGlobales("Programme Principal -> cas n°6 -> globalesRho");
            defautsGlobalesEthane(globalesRho,173,0.5271,true);
            menuDefautRho(globalesRho, *masseMolaire); // L'astérisque est importante car on fournit un double et pas un pointeur sur double (l'* permet d'accèder à la valeur)
            
            Tableau *TableauBornesRacinesRho = creerTableauInt(6,"Programme Principal -> cas n°6 -> TableauBornesRacinesRho");
            Tableau *TableauRacinesRho = creerTableauInt(3,"Programme Principal -> cas n°6 -> TableauRacinesRho");

            printf("Masse volumique à T = %.2f K et P = %.2f Pa vaut %.2f kg/m^3",globalesRho->T1,globalesRho->P1,trouveRhoLiq(globalesRho, TableauBornesRacinesRho,TableauRacinesRho,*masseMolaire)); // L'astérisque est importante car on fournit un double et pas un pointeur sur double (l'* permet d'accèder à la valeur)
            
            free(TableauBornesRacinesRho->donnees);
            free(TableauRacinesRho->donnees);
            free(TableauBornesRacinesRho);
            free(TableauRacinesRho);
            free(masseMolaire);
            free(globalesRho);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;
        case 7:            
            printf("Vous allez maintenant renseigner les valeurs concernant le cas n°1... \n"); 
            varPR *globalesDeltaH_1 = creerGlobales("Programme Principal -> cas n°7 -> globalesDeltaH_1");
            defautsGlobalesEthane(globalesDeltaH_1,173,0.5271,false);
            menuDefautValeursZ(globalesDeltaH_1);
            
            printf("Vous allez maintenant renseigner les valeurs concernant le cas n°2... \n");
            varPR *globalesDeltaH_2 = creerGlobales("Programme Principal -> cas n°7 -> globalesDeltaH_2");
            defautsGlobalesEthane(globalesDeltaH_2,293,13.78,false);
            menuDefautT2P2(globalesDeltaH_2,false);            

            Tableau *TableauBornesRacinesDH_1 = creerTableauInt(6,"Programme Principal -> cas n°7 -> TableauBornesRacinesDH_1");
            Tableau *TableauBornesRacinesDH_2 = creerTableauInt(6,"Programme Principal -> cas n°7 -> TableauBornesRacinesDH_2");
            Tableau *TableauRacinesDH_1 = creerTableauInt(3,"Programme Principal -> cas n°7 -> TableauRacinesDH_1");
            Tableau *TableauRacinesDH_2 = creerTableauInt(3,"Programme Principal -> cas n°7 -> TableauRacinesDH_2");

            cpStruct *cpCoeffs = (cpStruct*)malloc(sizeof(cpStruct)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de PR.c et hVapEOS.c. La structure en question est définie dans utilitaires.h
            if (cpCoeffs == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation cpCoeffs cas n°7.\n");
            return 0;
            }
            cpCoeffs->a =5.409; 
            cpCoeffs->b =1.781*pow(10,-1);
            cpCoeffs->c =-6.938*pow(10,-5);
            cpCoeffs->d =8.713*pow(10,-9);

            menuDefautCp(cpCoeffs);

            printf("La différence d'enthalpie entre les deux états spécifiés vaut : %.2f kJ/mol \n",deltaHSinglePhase(cpCoeffs,globalesDeltaH_1,globalesDeltaH_2,TableauBornesRacinesDH_1,TableauRacinesDH_1,TableauBornesRacinesDH_2,TableauRacinesDH_2));

            free(globalesDeltaH_1);
            free(globalesDeltaH_2);
            free(TableauBornesRacinesDH_1->donnees);
            free(TableauBornesRacinesDH_1);
            free(TableauBornesRacinesDH_2->donnees);
            free(TableauBornesRacinesDH_2);
            free(TableauRacinesDH_1->donnees);
            free(TableauRacinesDH_1);
            free(TableauRacinesDH_2->donnees);
            free(TableauRacinesDH_2);
            free(cpCoeffs);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;

        case 8:
            varPR *globalesDeltaS_1 = creerGlobales("Programme Principal -> cas n°8 -> globalesDeltaS_1");
            defautsGlobalesEthane(globalesDeltaS_1,173,0.5271,false);
            printf("Vous allez maintenant renseigner les valeurs concernant le cas n°1... \n");             
            menuDefautValeursZ(globalesDeltaS_1);
            
            printf("Vous allez maintenant renseigner les valeurs concernant le cas n°2... \n");
            varPR *globalesDeltaS_2 = creerGlobales("Programme Principal -> cas n°8 -> globalesDeltaS_2");
            defautsGlobalesEthane(globalesDeltaS_2,293,13.78,false);
            menuDefautT2P2(globalesDeltaS_2,false);       

            Tableau *TableauBornesRacinesDS_1 = creerTableauInt(6,"Programme Principal -> cas n°8 -> TableauBornesRacinesDS_1");
            Tableau *TableauBornesRacinesDS_2 = creerTableauInt(6,"Programme Principal -> cas n°8 -> TableauBornesRacinesDS_2");
            Tableau *TableauRacinesDS_1 = creerTableauInt(3,"Programme Principal -> cas n°8 -> TableauRacinesDS_1");
            Tableau *TableauRacinesDS_2 = creerTableauInt(3,"Programme Principal -> cas n°8 -> TableauRacinesDS_2");

            cpStruct *cpCoeffsEntropie = (cpStruct*)malloc(sizeof(cpStruct)); // Je demande l'allocation dynamique d'une vecteur contenant toutes les variables globales nécessaires au fonctionnement de PR.c et hVapEOS.c. La structure en question est définie dans utilitaires.h
            if (cpCoeffsEntropie == NULL) {
            printf("Erreur d'allocation mémoire -> main, allocation cpCoeffsEntropie cas n°8.\n");
            return 0;
            }
            cpCoeffsEntropie->a =5.409; 
            cpCoeffsEntropie->b =1.781*pow(10,-1);
            cpCoeffsEntropie->c =-6.938*pow(10,-5);
            cpCoeffsEntropie->d =8.713*pow(10,-9);

            menuDefautCp(cpCoeffsEntropie);

            printf("La différence d'entropie entre les deux états spécifiés vaut : %.2f J/mol/K \n",deltaEntropie(cpCoeffsEntropie,globalesDeltaS_1,globalesDeltaS_2,TableauBornesRacinesDS_1,TableauRacinesDS_1,TableauBornesRacinesDS_2,TableauRacinesDS_2));

            free(globalesDeltaS_1);
            free(globalesDeltaS_2);
            free(TableauBornesRacinesDS_1->donnees);
            free(TableauBornesRacinesDS_1);
            free(TableauBornesRacinesDS_2->donnees);
            free(TableauBornesRacinesDS_2);
            free(TableauRacinesDS_1->donnees);
            free(TableauRacinesDS_1);
            free(TableauRacinesDS_2->donnees);
            free(TableauRacinesDS_2);
            free(cpCoeffsEntropie);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;
        case 9:
            varPR *globalesDeltaV_1, *globalesDeltaV_2;
            Tableau *TableauBornesRacinesDV_1, *TableauBornesRacinesDV_2;
            Tableau *TableauRacinesDV_1, *TableauRacinesDV_2;
            globalesDeltaV_1 = creerGlobales("Prog principal -> cas n°9, globalesDeltaV_1");
            globalesDeltaV_2 = creerGlobales("Prog principal -> cas n°9, globalesDeltaV_2");
            defautsGlobalesEthane(globalesDeltaV_1, 173, 0.5271, true); // Par le true, je signale que je souhaite une conversion en SI.
            defautsGlobalesEthane(globalesDeltaV_2, 293, 13.78, true);
            menuDefautValeursZ(globalesDeltaV_1);
            menuDefautT2P2(globalesDeltaV_2,true);

            TableauBornesRacinesDV_1 = creerTableauInt(6,"Prog principal -> cas n°9, TableauBornesRacinesDV_1");
            TableauBornesRacinesDV_2 = creerTableauInt(6,"Prog principal -> cas n°9, TableauBornesRacinesDV_2");
            TableauRacinesDV_1 = creerTableauInt(3,"Prog principal -> cas n°9, TableauRacinesDV_1");
            TableauRacinesDV_2 = creerTableauInt(3,"Prog principal -> cas n°9, TableauRacinesDV_2");

            printf("La différence de volume molaire entre les deux états spécifiés vaut : %.5f m^3/mol \n",deltaV(globalesDeltaV_1, TableauBornesRacinesDV_1, TableauRacinesDV_1, globalesDeltaV_2, TableauBornesRacinesDV_2, TableauRacinesDV_2));
            free(globalesDeltaV_1);
            free(globalesDeltaV_2);
            free(TableauBornesRacinesDV_1->donnees);
            free(TableauBornesRacinesDV_1);
            free(TableauBornesRacinesDV_2->donnees);
            free(TableauBornesRacinesDV_2);
            free(TableauRacinesDV_1->donnees);
            free(TableauRacinesDV_1);
            free(TableauRacinesDV_2->donnees);
            free(TableauRacinesDV_2);
            printf("\n");
            printf("Pour continuer, appuyez sur entrer...");
            getchar();
            getchar();
            break;
        case 0:
            printf("\n");
            break;
        default:
            printf("Numéro de valeur non reconnu. \n" );
    }

} while (choixMenuPrincipal != 0);

return 0;
}
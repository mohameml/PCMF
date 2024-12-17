#include "pricer.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <iostream>

int main() {
    // Initialisation des paramètres
    int nAssets = 3;          // Nombre d'actifs
    double fdStep = 0.01;     // Pas de différence finie
    int nSample = 10000;      // Nombre d'échantillons
    double interestRate = 0.02;

    // Initialisation des strikes
    PnlVect *strikes = pnl_vect_create_from_list(nAssets, 100.0, 105.0, 110.0);

    // Initialisation des dates de paiement
    PnlVect *paymentDates = pnl_vect_create_from_list(4, 1.0, 2.0, 3.0, 4.0);

    // Initialisation de la matrice de volatilité
    PnlMat *volatility = pnl_mat_create_from_list(nAssets, nAssets,
                                                 0.2, 0.1, 0.0,
                                                 0.1, 0.25, 0.0,
                                                 0.0, 0.0, 0.3);

    // Construction du pricer
    BlackScholesPricer pricer(strikes, nAssets, fdStep, nSample, volatility, paymentDates, interestRate);

    // Affichage des paramètres pour vérifier
    pricer.print();

    // Libération de la mémoire
    pnl_vect_free(&strikes);
    pnl_vect_free(&paymentDates);
    pnl_mat_free(&volatility);

    return 0;
}

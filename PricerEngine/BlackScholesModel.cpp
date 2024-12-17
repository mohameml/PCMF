#include "BlackScholesModel.hpp"
#include <cmath>
#include <random>
#include "compute_last_index.hpp"
#include <iostream>
#include <cassert>

BlackScholesModel::BlackScholesModel()
{
}

BlackScholesModel::BlackScholesModel(const nlohmann::json &jsonParams){
    jsonParams.at("VolCholeskyLines").get_to(volatility);
    jsonParams.at("MathPaymentDates").get_to(paymentDates);
    jsonParams.at("DomesticInterestRate").get_to(interestRate);
    // jsonParams.at("RelativeFiniteDifferenceStep").get_to(fdStep);
    nAssets = volatility->n;
    G = pnl_vect_create(nAssets);
    vectVol = pnl_vect_create(nAssets);

    for (int d = 0; d < nAssets; d++)
    {
        PnlVect L_d = pnl_vect_wrap_mat_row(volatility, d);
        pnl_vect_set(vectVol,d,pnl_vect_norm_two(&L_d));
    }
    
}



BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&vectVol);

    pnl_vect_free(&paymentDates);
    pnl_vect_free(&G);
    pnl_mat_free(&volatility);
}


void BlackScholesModel::asset(const PnlMat *past, double t, bool isMonitoringDate, PnlMat *path, PnlRng *rng)
{
    int D = this->nAssets;
    double r = this->interestRate;

    int last_index = isMonitoringDate ? past->m : past->m -1;


    if (last_index == path->m - 1)
    {
        // pnl_mat_set_subblock(path, past, 0, 0);
        pnl_mat_extract_subblock(path, past, 0, path->m, 0, path->n);
        return;
    }


    pnl_mat_set_subblock(path, past, 0, 0);

    pnl_vect_rng_normal(G, D, rng);
    double dt = GET(paymentDates, last_index+1) - t;

    for (int d = 0; d < D; d++)
    {
        double s_t_d = MGET(past, past->m - 1, d);
        PnlVect L_d = pnl_vect_wrap_mat_row(volatility, d);

        // double sigma_d = pnl_vect_norm_two(&L_d);

        double sigma_d = GET(vectVol,d);
        MLET(path, last_index + 1, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * dt + sigma_d * sqrt(dt) * pnl_vect_scalar_prod(&L_d, G));
    }


    for (int i = last_index + 2; i < path->m; i++)
    {
        pnl_vect_rng_normal(G, D, rng);

        for (int d = 0; d < D; d++)
        {
            double s_t_d = MGET(path, i - 1, d);
            PnlVect L_d = pnl_vect_wrap_mat_row(volatility, d);

            double sigma_d = GET(vectVol,d);
            double time_step = GET(paymentDates, i) - GET(paymentDates, i-1);

            MLET(path, i, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * time_step + sigma_d * sqrt(time_step) * pnl_vect_scalar_prod(&L_d, G));
        }
    }
}





// void BlackScholesModel::shift_asset(int d, double h, PnlMat *original_paths)
// {
//     for (int i = 1; i < original_paths->m; i++)
//     {
//         // pnl_mat_set(original_paths, i, d, pnl_mat_get(original_paths, i, d) * h);
//         MLET(original_paths, i, d) *= h;
//     };
// }

// void BlackScholesModel::shift_asset(int d, double t, double h, PnlMat *original_paths)
// {
//     int nb_lines = original_paths->m;
//     double T = this->time_step * (nb_lines - 1);
//     int index = compute_last_index(t, T, nb_lines - 1);
//     for (int i = index + 1; i < nb_lines; i++)
//     {
//         // pnl_mat_set(original_paths, i, d, pnl_mat_get(original_paths, i, d) * h);
//         MLET(original_paths, i, d) *= h;
//     };
// }
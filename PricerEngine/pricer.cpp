#include <iostream>
#include "json_reader.hpp"
#include "pricer.hpp"

BlackScholesPricer::BlackScholesPricer(nlohmann::json &jsonParams) {
    jsonParams.at("VolCholeskyLines").get_to(volatility);
    jsonParams.at("MathPaymentDates").get_to(paymentDates);
    jsonParams.at("Strikes").get_to(strikes);
    jsonParams.at("DomesticInterestRate").get_to(interestRate);
    jsonParams.at("RelativeFiniteDifferenceStep").get_to(fdStep);
    jsonParams.at("SampleNb").get_to(nSamples);
    nAssets = volatility->n;
    G = pnl_vect_create(nAssets);
}

BlackScholesPricer::~BlackScholesPricer() {
    pnl_vect_free(&paymentDates);
    pnl_vect_free(&G);
    pnl_vect_free(&strikes);
    pnl_mat_free(&volatility);
}

void BlackScholesPricer::print() {
    std::cout << "nAssets: " << nAssets << std::endl;
    std::cout << "fdStep: " << fdStep << std::endl;
    std::cout << "nSamples: " << nSamples << std::endl;
    std::cout << "strikes: ";
    pnl_vect_print_asrow(strikes);
    std::cout << "paymentDates: ";
    pnl_vect_print_asrow(paymentDates);
    std::cout << "volatility: ";
    pnl_mat_print(volatility);
}

void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev) {
    
    price = 0.;
    priceStdDev = 0.;
    deltas = pnl_vect_create_from_zero(nAssets);
    deltasStdDev = pnl_vect_create_from_zero(nAssets);
    /* A compléter */
}




void BlackScholesPricer::asset(PnlMat *path, PnlRng *rng, double creationDate)
{
    int D = this->nAssets;
    double r = this->interestRate;
    // pnl_mat_set_row(path, spots, 0);
    int date_number = paymentDates->size;
    double tm = creationDate;

    for (int i = 0; i < date_number; i++)
    {
        pnl_vect_rng_normal(G, D, rng);
        for (int d = 0; d < D; d++)
        {
            PnlVect L_d = pnl_vect_wrap_mat_row(volatility, d);
            double sigma_d = pnl_vect_norm_two(&L_d);
            double time_step = GET(paymentDates, i) - tm;
            MLET(path, i, d) = MGET(path, i - 1, d) * exp((r - sigma_d * sigma_d / 2.0) * (time_step) + sigma_d * sqrt(time_step) * pnl_vect_scalar_prod(&L_d, G));
            tm =  GET(paymentDates, i);
        }
    }
}

void BlackScholesPricer::asset(const PnlMat *past, double t, bool isMonitoringDate, PnlMat *path, PnlRng *rng)
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

        double sigma_d = pnl_vect_norm_two(&L_d);
        MLET(path, last_index + 1, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * dt + sigma_d * sqrt(dt) * pnl_vect_scalar_prod(&L_d, G));
    }


    for (int i = last_index + 2; i < path->m; i++)
    {
        pnl_vect_rng_normal(G, D, rng);

        for (int d = 0; d < D; d++)
        {
            double s_t_d = MGET(path, i - 1, d);
            PnlVect L_d = pnl_vect_wrap_mat_row(volatility, d);
            double sigma_d = pnl_vect_norm_two(&L_d);
            double time_step = GET(paymentDates, i) - GET(paymentDates, i-1);
            MLET(path, i, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * time_step + sigma_d * sqrt(time_step) * pnl_vect_scalar_prod(&L_d, G));
        }
    }
}
#include <iostream>
#include "json_reader.hpp"
#include "pricer.hpp"
#include "Option.hpp"
#include "BlackScholesModel.hpp"

BlackScholesPricer::BlackScholesPricer(nlohmann::json &jsonParams) {

    jsonParams.at("MathPaymentDates").get_to(paymentDates);
    jsonParams.at("SampleNb").get_to(nSamples);
    model = new BlackScholesModel(jsonParams);
    option = instance_option(jsonParams);
    jsonParams.at("RelativeFiniteDifferenceStep").get_to(fdStep);
    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
}


BlackScholesPricer::~BlackScholesPricer() {
    pnl_vect_free(&paymentDates);
    pnl_rng_free(&rng);
    delete model;
    delete option;
}

void BlackScholesPricer::print() {
    std::cout << "nAssets: " << model->nAssets << std::endl;
    std::cout << "fdStep: " << fdStep << std::endl;
    std::cout << "nSamples: " << nSamples << std::endl;
    std::cout << "strikes: ";
    pnl_vect_print_asrow(option->strike);
    std::cout << "paymentDates: ";
    pnl_vect_print_asrow(paymentDates);
    std::cout << "volatility: ";
    pnl_mat_print(model->volatility);
}



void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev) {
    
    price = 0.;
    priceStdDev = 0.;
    deltas = pnl_vect_create_from_zero(model->nAssets);
    deltasStdDev = pnl_vect_create_from_zero(model->nAssets);
    /* A compléter */

    int D = model->nAssets;
    int M = this->nSamples;
    int N = option->strike->size;
    double h = fdStep;
    double r = model->interestRate;
    double lastIndex = isMonitoringDate ? past->m - 1 : past->m - 2 ; 


    PnlMat *path = pnl_mat_create(N + 1, D);

    for (int i = 0; i < M; i++)
    {
        model->asset(past, currentDate , isMonitoringDate , path, rng);
        double phi_j = option->payOff(path);
        price += phi_j;
        priceStdDev += phi_j * phi_j;

        for (int d = 0; d < D; d++)
        {
            double diff_payoff = 0.0;
            model->shift_asset(d, lastIndex, 1 + h, path);
            diff_payoff += option->payOff(path);
            model->shift_asset(d, lastIndex , (1.0 - h) / (1.0 + h), path);
            diff_payoff -= option->payOff(path);
            model->shift_asset(d, lastIndex , 1.0 / (1.0 - h), path);
            LET(deltas, d) += diff_payoff;
            LET(deltasStdDev, d) += diff_payoff * diff_payoff;
        }
    }

    end_of_calcul_price(price, priceStdDev, currentDate);
    PnlVect St = pnl_vect_wrap_mat_row(past, past->m - 1);
    end_of_calcul_delta(deltas, deltasStdDev, currentDate, &St);

    pnl_mat_free(&path);
}



void BlackScholesPricer::end_of_calcul_price(double &price, double &price_stdev, double t) const
{
    double r = model->interestRate;
    double T = GET(paymentDates , paymentDates->size -1 ) ;
    double M = nSamples;
    price = std::exp(-r * (T - t)) * price / M;
    price_stdev = price_stdev * std::exp(-2.0 * r * (T - t)) / M - price * price;
    price_stdev = std::sqrt(price_stdev / M);
}

void BlackScholesPricer::end_of_calcul_delta(PnlVect *delta, PnlVect *delta_stdev, double t, PnlVect *St) const
{
    double M = nSamples;
    double r = model->interestRate;
    double T = GET(paymentDates , paymentDates->size -1 ) ; 

    double h = fdStep;

    // fin de calcul de delta :
    pnl_vect_mult_scalar(delta, std::exp(-r * (T - t)) / (2.0 * h * M));
    pnl_vect_div_vect_term(delta, St);

    // fin de calcul de delta_std_dev :
    pnl_vect_mult_scalar(delta_stdev, std::exp(-2.0 * r * (T - t)) / (4.0 * M * h * h));
    pnl_vect_div_vect_term(delta_stdev, St);
    pnl_vect_div_vect_term(delta_stdev, St);
    PnlVect *delta_copy = pnl_vect_copy(delta);
    pnl_vect_mult_vect_term(delta_copy, delta);
    pnl_vect_minus_vect(delta_stdev, delta_copy);
    pnl_vect_div_double(delta_stdev, M);
    pnl_vect_map_inplace(delta_stdev, std::sqrt);

    pnl_vect_free(&delta_copy);
}






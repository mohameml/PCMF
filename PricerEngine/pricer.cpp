#include <iostream>
#include "json_reader.hpp"
#include "pricer.hpp"

BlackScholesPricer::BlackScholesPricer(nlohmann::json &jsonParams) {

    // jsonParams.at("SampleNb").get_to(nSamples);
    // rng = pnl_rng_create(PNL_RNG_MERSENNE);
    // pnl_rng_sseed(rng, time(NULL));
    // option = instance_option(jsonParams); 
    jsonParams.at("MathPaymentDates").get_to(paymentDates);
    jsonParams.at("sample number").get_to(nSamples);
    model = new BlackScholesModel(jsonParams);
    option = instance_option(jsonParams);
    rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
}


BlackScholesPricer::~BlackScholesPricer() {
    // pnl_vect_free(&G);
    // pnl_vect_free(&strikes);
    // pnl_mat_free(&volatility);
    // pnl_rng_free(&rng);
    // delete option ; 

    pnl_vect_free(&paymentDates);
    pnl_rng_free(&rng);
    delete model;
    delete option;
}


void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev) {
    
    price = 0.;
    priceStdDev = 0.;
    deltas = pnl_vect_create_from_zero(nAssets);
    deltasStdDev = pnl_vect_create_from_zero(nAssets);
    /* A compléter */

    int D = this->nAssets;
    int M = this->nSamples;
    int N = this->strikes->size;
    double h = this->fdStep;
    double r = this->interestRate;
    double lastIndex = isMonitoringDate ? past->m - 1 : past->m - 2 ; 


    PnlMat *path = pnl_mat_create(N + 1, D);

    for (int i = 0; i < M; i++)
    {
        asset(past, currentDate , isMonitoringDate , path, rng);
        double phi_j = option->payOff(path);
        price += phi_j;
        priceStdDev += phi_j * phi_j;

        for (int d = 0; d < D; d++)
        {
            double diff_payoff = 0.0;
            shift_asset(d, lastIndex, 1 + h, path);
            diff_payoff += option->payOff(path);
            shift_asset(d, lastIndex , (1.0 - h) / (1.0 + h), path);
            diff_payoff -= option->payOff(path);
            shift_asset(d, lastIndex , 1.0 / (1.0 - h), path);
            LET(deltas, d) += diff_payoff;
            LET(deltasStdDev, d) += diff_payoff * diff_payoff;
        }
    }

    end_of_calcul_price(price, priceStdDev, currentDate);
    PnlVect St = pnl_vect_wrap_mat_row(past, past->m - 1);
    end_of_calcul_delta(deltas, deltasStdDev, currentDate, &St);

    pnl_mat_free(&path);
}

void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev) {

<<<<<<< HEAD
void BlackScholesPricer::end_of_calcul_price(double &price, double &price_stdev, double t) const
{
    double r = interestRate;
    double T = GET(paymentDates , paymentDates->size -1 ) ;
    double M = nSamples;
    price = std::exp(-r * (T - t)) * price / M;
    price_stdev = price_stdev * std::exp(-2.0 * r * (T - t)) / M - price * price;
    price_stdev = std::sqrt(price_stdev / M);
}

void BlackScholesPricer::end_of_calcul_delta(PnlVect *delta, PnlVect *delta_stdev, double t, PnlVect *St) const
{
    double M = nSamples;
    double r = interestRate;
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


void BlackScholesPricer::shift_asset(int d, double lastIndex, double h, PnlMat *path)
{
    for (int i = lastIndex + 1; i < path->m; i++)
    {
        MLET(path, i, d) *= h;
    };
}
    // int D = this->option->size;
    // int M = this->nSamples;
    // int N = this->fixing_dates_number;
    // double h = this->fd_step;
    // double r = this->model->interest_rate;
    // double T = this->option->maturity;

    // price = 0.0;

//     PnlMat *matrix = pnl_mat_create(N + 1, D);

//     for (int i = 0; i < M; i++)
//     {
//         this->model->asset(Past, t, T, matrix, this->rng);
//         double phi_j = this->option->payOff(matrix);
//         price += phi_j;

//     }

//     end_of_calcul_price(price, price_std, t);
//     PnlVect St = pnl_vect_wrap_mat_row(Past, Past->m - 1);

//     pnl_mat_free(&matrix);

// }

void MonteCarlo::end_of_calcul_price(double &price, double &price_stdev, double t) const
{
    int D = this->nAssets;
    double r = this->interestRate;

    // int last_index = isMonitoringDate ? past->m - 1 : past->m - 2;
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
    // double r = model->interest_rate;
    // double T = option->maturity;
    // double M = sample_number;
    // price = std::exp(-r * (T - t)) * price / M;
    // price_stdev = price_stdev * std::exp(-2.0 * r * (T - t)) / M - price * price;
    // price_stdev = std::sqrt(price_stdev / M);
}
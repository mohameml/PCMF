#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "Option.hpp"
#include "BlackScholesModel.hpp"


class BlackScholesPricer {
public:
    Option *option;
    BlackScholesModel *model;
    PnlVect *paymentDates;
    int nSamples;
    PnlRng* rng ; 
    double fdStep ; 


    BlackScholesPricer(nlohmann::json &jsonParams);
    ~BlackScholesPricer();
    void priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev);
    void print();
    void end_of_calcul_price(double &price, double &price_stdev, double t) const;
    void end_of_calcul_delta(PnlVect *delta, PnlVect *delta_stdev, double t, PnlVect *St) const;

};

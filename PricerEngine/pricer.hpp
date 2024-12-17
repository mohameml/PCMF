#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"



class BlackScholesPricer {
public:
    PnlMat *volatility;
    PnlVect *paymentDates;
    PnlVect *strikes;
    PnlVect *G;
    int nAssets;
    double interestRate;
    double fdStep;
    int nSamples;

    BlackScholesPricer(nlohmann::json &jsonParams);
    ~BlackScholesPricer();
    void priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev);
    void print();
    void asset(PnlMat *path, PnlRng *rng, double creationDate);
    void asset(const PnlMat *past, double t, bool isMonitoringDate, PnlMat *path, PnlRng *rng);
};

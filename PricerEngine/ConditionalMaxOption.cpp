#include "ConditionalMaxOption.hpp"

ConditionalMaxOption::ConditionalMaxOption()
{
}

ConditionalMaxOption::ConditionalMaxOption(const nlohmann::json &json) : Option(json)
{

}

ConditionalMaxOption::~ConditionalMaxOption()
{

}

double ConditionalMaxOption::payOff(const PnlMat *matrix)
{
    double payOff = 0.;
    // TODO : r ? 
    // double facteur = std::exp(r*(GET(paymentDate , paymentDate->size - 1) - GET(paymentDate , m))) ; 

    double p_prec = 0.;
    for (size_t m = 0; m < matrix->m; m++)
    {
        double Pm = 0.;
        PnlVect valSoujacent = pnl_vect_wrap_mat_row(matrix, m);
        // Pm = pnl_vect_max(&valSoujacent)*facteur;
        Pm = pnl_vect_max(&valSoujacent);
        Pm = std::max(Pm - GET(strike, m), 0.);

        if(p_prec == 0.)
        {
            payOff += Pm;
        }
        p_prec = Pm;
        
    }

    return payOff;
    

}
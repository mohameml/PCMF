#include "ConditionalBasketOption.hpp"

ConditionalBasketOption::ConditionalBasketOption()
{
}

ConditionalBasketOption::ConditionalBasketOption(const nlohmann::json &json) : Option(json)
{

}

ConditionalBasketOption::~ConditionalBasketOption()
{

}

double ConditionalBasketOption::payOff(const PnlMat *matrix)
{
    double T = GET(paymentDate , paymentDate->size - 1);

    for (size_t m = 1 ; m < matrix->m; m++)
    {
        PnlVect valSoujacent = pnl_vect_wrap_mat_row(matrix, m);
        double facteur = std::exp(intersertRate*(T - GET(paymentDate , m - 1))) ; 
        double Pm = pnl_vect_sum(&valSoujacent)*facteur;
        int N = valSoujacent.size;
        Pm = std::max(1/(double)N *Pm - GET(strike, m - 1), 0.);

        if(fabs(Pm) > 10E-10)
        {
            return Pm ;
        }
        
    }

    return 0.0 ; 

}
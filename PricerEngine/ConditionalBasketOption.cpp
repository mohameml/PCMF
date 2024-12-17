#include "ConditionalBasketOption.hpp"

ConditionalBasketOption::ConditionalBasketOption()
{
}

ConditionalBasketOption::ConditionalBasketOption(const nlohmann::json json) : Option(json)
{

}

ConditionalBasketOption::~ConditionalBasketOption()
{

}

double ConditionalBasketOption::payOff(const PnlMat *matrix)
{
    double payOff = 0.;
    for (size_t m = 0; m < matrix->m; m++)
    {
        double Pm = 0.;
        PnlVect valSoujacent = pnl_vect_wrap_mat_row(matrix, m);
        Pm = pnl_vect_sum(&valSoujacent);
        int N = valSoujacent.size;
        Pm = std::max(1/(double)N *Pm - GET(strike, m), 0.);

        if(payOff == 0.)
        {
            payOff += Pm;
        }
        
    }

    return payOff;
    

}
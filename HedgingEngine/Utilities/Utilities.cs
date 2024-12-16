using TimeHandler ; 

namespace HedgingEngine.Utilities
{
    public static class Utilities
    {


        public static double SumProduit(Dictionary<string, double> dict1 , Dictionary<string, double> dict2)
        {

            double res = 0;
            foreach (string key in dict1.Keys)
            {
                res += dict1[key] * dict2[key];
            }

            return res;
        }


    }
}

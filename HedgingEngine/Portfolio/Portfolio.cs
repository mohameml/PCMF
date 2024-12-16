using MarketData ; 
using TimeHandler ; 

namespace HedgingEngine.Portfolio {

    public class Portfolio {
        public Dictionary<string, double> Compositions { get; private set; }
        public double Cash { get; private set; } = 0;
        public DateTime Date { get; private set; }

        public Portfolio(Dictionary<string, double>  dictInit , DataFeed data , double value  )
        {
            Compositions = dictInit;
            Cash = value - Utilities.Utilities.SumProduit(dictInit, data.SpotList);
            Date = data.Date;
        }

        public void UpdateCompo(Dictionary<string, double> Compos ,  DataFeed feed , double value)
        {
            // double value = GetPortfolioValue(feed , r);
            Compositions = Compos;
            Cash = value - Compos.Keys.Sum(key => Compos[key]*feed.SpotList[key]);
            Date = feed.Date;
        }

        public double GetPortfolioValue(DataFeed feed , double time , double r)
        {
            // Cash *= Math.Exp(r*converter.ConvertToMathDistance(Date , feed.Date));
            double value = Compositions.Keys.Sum(key => Compositions[key]*feed.SpotList[key]) + Cash*Math.Exp(r*time);
            return value;
        }


    }
}
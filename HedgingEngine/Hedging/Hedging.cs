using ParameterInfo;
using HedgingEngine.Pricing;
using MarketData;
using HedgingEngine.Rebalancing;
using GrpcPricing.Protos;
using TimeHandler;


namespace HedgingEngine.Hedging {


    public class Hedging {

        TestParameters FinancialParam {get ; init ;} 
        
        Pricer Pricer {get ; init ;}
        Rebalancing.Rebalancing OracleRebalacing {get ; init ;}   



        public Hedging(TestParameters parameters  ) {

            FinancialParam = parameters;
            Pricer = new PricerGrpc(parameters) ; 
            OracleRebalacing = new FixedRebalacing(parameters.RebalancingOracleDescription.Period);

        }


        public List<OutputData> Hedge(List<DataFeed> dataFeeds) {


            MathDateConverter converter = new(FinancialParam.NumberOfDaysInOneYear);

            
            List<DataFeed> past = [dataFeeds[0]];
            bool monitoringDate = FinancialParam.PayoffDescription.PaymentDates.Contains(dataFeeds[0].Date) ; 
            double timeMath = converter.ConvertToMathDistance(FinancialParam.PayoffDescription.CreationDate, dataFeeds[0].Date);
            PricingParams pricerParams = new(past ,timeMath , monitoringDate) ; 
            
            PricingResults results = Pricer.PriceAndDeltas(pricerParams);

            Portfolio.Portfolio portfolio = new(results.Deltas , dataFeeds[0] , results.Price);

            OutputData output0 = new() 
            {
                Value = results.Price,
                Date = dataFeeds[0].Date,
                Price = results.Price,
                PriceStdDev = results.PriceStdDev,
                Deltas = [.. results.Deltas.Values],
                DeltasStdDev = [.. results.DeltasStdDev.Values]
            };
            List<OutputData> listOutput = [output0] ; 

            List<DataFeed> hedgingPast  = new();


            double r =  FinancialParam.AssetDescription.CurrencyRates[FinancialParam.AssetDescription.DomesticCurrencyId];
            double time  ;
            double value ;

            foreach (DataFeed feed in dataFeeds.Skip(1))
            {

                timeMath = converter.ConvertToMathDistance(FinancialParam.PayoffDescription.CreationDate , feed.Date);
                monitoringDate = FinancialParam.PayoffDescription.PaymentDates.Contains(feed.Date);

                if(monitoringDate) {
                    hedgingPast.Add(feed);
                    past = new List<DataFeed>(hedgingPast);
                } else {
                    past = new List<DataFeed>(hedgingPast);
                    past.Add(feed);
                }
                
                pricerParams.SetParams(past , timeMath , monitoringDate);

                results = Pricer.PriceAndDeltas(pricerParams);

                if (OracleRebalacing.IsRebalancing(feed.Date))
                {
                    time = converter.ConvertToMathDistance(portfolio.Date , feed.Date);
                    value = portfolio.GetPortfolioValue(feed , time , r);
                    portfolio.UpdateCompo(results.Deltas , feed , value);

                    OutputData output = new()
                    {
                        Value = value,
                        Date = feed.Date,
                        Price = results.Price,
                        PriceStdDev = results.PriceStdDev,
                        Deltas = [.. results.Deltas.Values],
                        DeltasStdDev = [.. results.DeltasStdDev.Values]
                    };

                    listOutput.Add(output);

                }
            }

            return listOutput ;    
        }
        



    }
}
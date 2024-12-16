using MarketData ; 
using GrpcPricing.Protos ;
using ParameterInfo;
using Grpc.Net.Client;

namespace HedgingEngine.Pricing {

    public class PricerGrpc  : Pricer {


        private GrpcPricer.GrpcPricerClient GrpcClient {get ; init ;}


        public PricerGrpc(TestParameters testParameters , string adress = "http://localhost:50051") : base(testParameters)
        {
            GrpcChannel Channel = GrpcChannel.ForAddress(adress);
            GrpcClient =  new GrpcPricer.GrpcPricerClient(Channel) ; 
        }

        public override PricingResults PriceAndDeltas(PricingParams pricingParams)
        {
            PricingInput input = PricingParamsToPricingInput(pricingParams);
            PricingOutput output = GrpcClient.PriceAndDeltas(input);
            
            PricingResults res = new([.. output.Deltas], 
                output.Price , [.. output.DeltasStdDev], 
                output.PriceStdDev ,
                [.. pricingParams.DataFeeds[0].SpotList.Keys]);
                
            return res ; 
        }



        public ReqInfo Heartbeat() {

            ReqInfo info =  GrpcClient.Heartbeat(new Empty());
            Console.WriteLine("Message reçu : " + info.DomesticInterestRate);
            Console.WriteLine("Message reçu : " + info.RelativeFiniteDifferenceStep);
            Console.WriteLine("Message reçu : " + info.SampleNb);

            return info ; 
        }


        PricingInput PricingParamsToPricingInput(PricingParams pricingParams) {

            PricingInput input = new() ; 
            
            foreach (var feed in pricingParams.DataFeeds)
            {
                PastLines line = new() ; 

                foreach (var value  in feed.SpotList.Values)
                {
                    line.Value.Add(value);
                }

                input.Past.Add(line); 
            }
            input.MonitoringDateReached = pricingParams.MonitoringDate ; 
            input.Time = pricingParams.Time ; 

            return input ; 
        }


    }
}
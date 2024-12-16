using System.Threading.Channels;
using Grpc.Net.Client;
using GrpcPricing.Protos; 

namespace HedgingEngine.Pricing {


    public class GrpcPricingClient {

        private GrpcChannel Channel {get ; init ;}
        
        public GrpcPricer.GrpcPricerClient Client  {get ; set ; } 


        public GrpcPricingClient(string adress = "http://localhost:50051" ) {
            
            Channel = GrpcChannel.ForAddress(adress);
            Client = new GrpcPricer.GrpcPricerClient(Channel); 
            
        }




    }   

}
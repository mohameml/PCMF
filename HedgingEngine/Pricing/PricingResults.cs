namespace HedgingEngine.Pricing {

    public  class PricingResults {

        public Dictionary<string , double> Deltas = new() ; 
        public Dictionary<string , double> DeltasStdDev = new() ;
        public double Price ;
        public double PriceStdDev ;  


        public PricingResults(double[] deltas , double price , double[] deltasStdDev , double priceStdDev , string[] Ids) {
            Price = price ;
            PriceStdDev = priceStdDev ; 

            for (int i = 0; i < deltas.Length; i++)
            {
                Deltas[Ids[i]] = deltas[i]; 
                DeltasStdDev[Ids[i]] = deltasStdDev[i]; 

            }

        }

    }

}
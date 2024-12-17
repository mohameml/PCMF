using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using HedgingEngine.Pricing;

namespace HedgingEngineTest.Pricing
{
    public class PricerTest : Pricer
    {
        public PricerTest(ParameterInfo.TestParameters testParameters) : base(testParameters)
        {
        }

        public override PricingResults PriceAndDeltas(PricingParams pricingParams)
        {
            throw new NotImplementedException();
        }
    }
}

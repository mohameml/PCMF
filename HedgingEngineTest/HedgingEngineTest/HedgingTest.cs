using HedgingEngine.Hedging;
using MarketData;
using ParameterInfo;
using ParameterInfo.JsonUtils;

namespace HedgingEngineTest
{
    [TestFixture]
    public class HedgingTest
    {
        private string fileJsonParam;
        private string fileCsv;
        //private string fileJsonRes;
        private Hedging _hedger ;

        [SetUp]
        public void Setup()
        {
            List<DataFeed> data = MarketDataReader.ReadDataFeeds(fileCsv);

            string jsonString = File.ReadAllText(fileJsonParam);
            ParameterInfo.TestParameters financialParam = JsonIO.FromJson(jsonString);

             _hedger = new(financialParam);
        }

        [Test]
        public void Test1()
        {
            Assert.Pass();
        }
    }
}
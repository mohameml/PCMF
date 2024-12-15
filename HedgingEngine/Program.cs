using System;
using Grpc.Net.Client;
using GrpcPricing.Protos;
using System.Threading.Tasks;


class Program
{
    static async Task Main(string[] args)
    {
        // URL de votre serveur gRPC
        var channel = GrpcChannel.ForAddress("http://localhost:50051");

        // Créer un client pour le service Greeter
        var client = new GrpcPricer.GrpcPricerClient(channel);

        // Effectuer un appel au service
        var reply = await client.HeartbeatAsync(new Empty());
        Console.WriteLine("Message reçu : " + reply.DomesticInterestRate);
        Console.WriteLine("Message reçu : " + reply.RelativeFiniteDifferenceStep);
        Console.WriteLine("Message reçu : " + reply.SampleNb);

    }
}

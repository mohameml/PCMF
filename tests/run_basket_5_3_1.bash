#!/bin/bash

# Répertoires de travail
SERVER_DIR="../PricerEngine/build"
SERVER_CMD="./pricing_server ../../TestCases/Basket_5_3_1/basket_5_3_1_math_param.json"

CLIENT_DIR="../../HedgingEngine"
CLIENT_CMD="dotnet run ../TestCases/Basket_5_3_1/basket_5_3_1_param.json ../TestCases/Basket_5_3_1/basket_5_3_1_mkt_data.csv ../tests/res_basket_5_3_1.json"

# Lancer le serveur
echo "Démarrage du serveur dans $SERVER_DIR..."
cd "$SERVER_DIR" || exit
$SERVER_CMD &
SERVER_PID=$! # Récupère le PID du serveur en arrière-plan
echo "Serveur démarré avec PID $SERVER_PID."

# Attendre quelques secondes pour s'assurer que le serveur est prêt
echo "Attente de 2 secondes pour que le serveur soit opérationnel..."
sleep 2

# Lancer le client
echo "Démarrage du client dans $CLIENT_DIR..."
cd "$CLIENT_DIR" || exit
$CLIENT_CMD

# Une fois que le client a terminé, arrêter le serveur
echo "Arrêt du serveur (PID $SERVER_PID)..."
kill $SERVER_PID
echo "Serveur arrêté."


#!/bin/bash

run_test() {
    local SERVER_DIR=$1
    local SERVER_CMD=$2
    local CLIENT_DIR=$3
    local CLIENT_CMD=$4

    # Lancer le serveur
    echo "========== DÉMARRAGE DU SERVEUR =========="
    echo "Démarrage du serveur dans $SERVER_DIR..."
    cd "$SERVER_DIR" || exit
    $SERVER_CMD &
    SERVER_PID=$!
    echo "Serveur démarré avec PID $SERVER_PID."

    # Attendre quelques secondes pour que le serveur soit prêt
    echo "Attente de 2 secondes pour que le serveur soit opérationnel..."
    sleep 2

    # Lancer le client
    echo "========== DÉMARRAGE DU CLIENT =========="
    echo "Démarrage du client dans $CLIENT_DIR..."
    cd "$CLIENT_DIR" || exit
    $CLIENT_CMD

    # Une fois le client terminé, arrêter le serveur
    echo "========== ARRÊT DU SERVEUR =========="
    echo "Arrêt du serveur (PID $SERVER_PID)..."
    kill $SERVER_PID
    echo "Serveur arrêté."
}


# Max_5_3_1
echo "============================= Max_5_3_1 ======================== "
run_test "../PricerEngine/build" \
        "./pricing_server ../../TestCases/Max_5_3_1/max_5_3_1_math_param.json" \
        "../../HedgingEngine" \
        "dotnet run ../TestCases/Max_5_3_1/max_5_3_1_param.json ../TestCases/Max_5_3_1/max_5_3_1_mkt_data.csv ../tests/res_max_5_3_1.json"

# PCMF : projet couverture multi-flux 

- **Etapes principales du projet:**

    - Faire valider l'architecture et le principe mathématique de couverture
    - Réussir à faire communiquer le client et le serveur grpc (squelettes)
    - Implémentation d'un outil qui couvre un call vanille
    - Implémentation d'un outil qui couvre les deux produits décrits au dessus.


- **tests:**

    - **Basket_5_3_1:**

        ```bash
        ./pricing_server ../../TestCases/Basket_5_3_1/basket_5_3_1_math_param.json   
        dotnet run ../TestCases/Basket_5_3_1/basket_5_3_1_param.json ../TestCases/Basket_5_3_1/basket_5_3_1_mkt_data.csv ../tests/res_basket_5_3_1.json
        ```

    - **Basket_8_4_3:**

        ```bash
        ./pricing_server ../../TestCases/Basket_8_4_3/basket_8_4_3_math_param.json   
        dotnet run ../TestCases/Basket_8_4_3/basket_8_4_3_param.json ../TestCases/Basket_8_4_3/basket_8_4_3_mkt_data.csv ../tests/res_basket_8_4_3.json
        ```

    - **Max_5_3_1:**

        ```bash
        ./pricing_server ../../TestCases/Max_5_3_1/max_5_3_1_math_param.json   
        dotnet run ../TestCases/Max_5_3_1/max_5_3_1_param.json ../TestCases/Max_5_3_1/max_5_3_1_mkt_data.csv ../tests/res_max_5_3_1.json
        ```

    - **Max_8_4_3:**

        ```bash
        ./pricing_server ../../TestCases/Max_8_4_3/max_8_4_3_math_param.json   
        dotnet run ../TestCases/Max_8_4_3/max_8_4_3_param.json ../TestCases/Max_8_4_3/max_8_4_3_mkt_data.csv ../tests/res_max_8_4_3.json
        ```
        
#pragma once
#include "crow.h"
#include "../blockchain/blockchain.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    app.port(8265).run();
}
#include "rutas_api.h"
#include "crear_disco.h"

void registrarRutas(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")
    ([]() {
        return "Backend funcionando";
    });

    CROW_ROUTE(app, "/ejecutar").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);

        if (!body || !body.has("input")) {
            crow::json::wvalue respuesta;
            respuesta["output"] = "Error: entrada no valida";
            return crow::response(400, respuesta);
        }

        std::string entrada = body["input"].s();

        crow::json::wvalue respuesta;
        respuesta["output"] = "Comando recibido:\n" + entrada;
        return crow::response(200, respuesta);
    });
}
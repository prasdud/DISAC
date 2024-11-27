#include <iostream>
#include <crow.h>
#include "../blockchain/blockchain.h"

crow::response handle_add_block(const crow::request& req) {
    // Extract form data from the request
    std::string student_name;
    std::string student_id;
    std::string pdf_name;

    // Get the form data using Crow's parsing function
    auto studentName = req.url_params.get("studentName");
    auto studentId = req.url_params.get("studentId");
    auto pdfFile = req.url_params.get("pdfFile");

    if (!studentName || !studentId || !pdfFile) {
        return crow::response(400, "Missing required fields.");
    }

    student_name = *studentName;
    student_id = *studentId;
    pdf_name = *pdfFile; // PDF name (not binary)

    // Check if all fields are provided
    if (student_name.empty() || student_id.empty() || pdf_name.empty()) {
        return crow::response(400, "Missing required fields.");
    }

    // Assuming Blockchain and Block classes are defined and you are creating a new block
    Blockchain blockchain;
    std::string prevHash = blockchain.getLastBlockHash();
    Block new_block(prevHash);

    new_block.setStudentName(student_name);
    new_block.setStudentId(student_id);
    new_block.setPdfName(pdf_name);  // Set the PDF name (handle file data as needed)

    blockchain.addNewBlock(new_block);

    return crow::response(200, "Block added successfully!");
}

int main() {
    crow::SimpleApp app;

    // Handle preflight (OPTIONS) requests to allow CORS
    app.route_dynamic("/add_block")
        .methods(crow::HTTPMethod::OPTIONS)
    ([](const crow::request& req) {
        crow::response resp(204);
        resp.set_header("Access-Control-Allow-Origin", "http://127.0.0.1:5500");
        resp.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        resp.set_header("Access-Control-Allow-Headers", "Content-Type");
        return resp;
    });

    // Define the actual POST endpoint
    app.route_dynamic("/add_block")
        .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        return handle_add_block(req);
    });

    // Start the server
    app.port(18080).multithreaded().run();
}

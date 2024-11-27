#include <crow.h>
#include "../blockchain/blockchain.h"
#include "../blockchain/block.h"
#include <fstream>
#include <iostream>

crow::response handle_add_block(const crow::request& req) {
    try {
        // Access raw POST body
        std::string body = req.body;
        // Here, you will need to manually parse the body if it's in multipart format

        // Debugging: Output the raw body
        std::cout << "Received raw body: " << body << std::endl;

        // Manually extracting the form data from body string (if it's form-encoded)
        // If the body is in multipart format, you'd need a more advanced parsing strategy.
        // For now, let's assume it's in URL encoded format for simplicity.
        
        // Example: Parsing query-style body (This is just an example, you need proper multipart parsing for files)
        std::string student_name = "defaultName";  // Replace with actual extraction logic
        std::string student_id = "defaultId";     // Replace with actual extraction logic
        std::string pdf_file = "defaultPdf";      // Replace with actual extraction logic

        // Verify the fields are not empty
        if (student_name.empty() || student_id.empty() || pdf_file.empty()) {
            return crow::response(400, "Missing required fields.");
        }

        // Debugging output to check the received data
        std::cout << "Received data:" << std::endl;
        std::cout << "Student Name: " << student_name << std::endl;
        std::cout << "Student ID: " << student_id << std::endl;
        std::cout << "PDF file: " << pdf_file << std::endl;

        // Get the previous block hash from the blockchain
        Blockchain blockchain;
        std::string prevHash = blockchain.getLastBlockHash();

        // Create a new Block with the received data
        Block new_block(prevHash, student_name, student_id, pdf_file);

        // Add the block to the blockchain
        blockchain.addNewBlock(new_block);

        return crow::response(200, "Block added successfully!");
    } catch (const std::exception& e) {
        return crow::response(500, std::string("Error: ") + e.what());
    }
}

int main() {
    crow::SimpleApp app;

    // Define route to handle adding blocks
    CROW_ROUTE(app, "/add_block").methods("POST"_method)(handle_add_block);

    // Start the server
    app.port(18080).multithreaded().run();
}

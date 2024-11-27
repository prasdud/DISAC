#include <crow.h>
#include "../blockchain/blockchain.h"
#include "../blockchain/block.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <filesystem>

// Utility function to save uploaded file
std::string save_uploaded_file(const std::string& file_content, const std::string& filename) {
    // Create a directory to store uploaded files if it doesn't exist
    std::filesystem::create_directories("uploads");
    
    // Generate a unique filename to prevent overwriting
    std::string unique_filename = "uploads/" + std::to_string(std::hash<std::string>{}(filename + std::to_string(std::time(nullptr)))) + "_" + filename;
    
    std::ofstream file(unique_filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not create file: " + unique_filename);
    }
    
    file.write(file_content.data(), file_content.size());
    file.close();
    
    return unique_filename;
}

crow::response handle_add_block(const crow::request& req) {
    try {
        // Get the raw body and content type
        std::string body = req.body;
        std::string content_type = req.get_header_value("Content-Type");

        // Debugging
        std::cout << "Content-Type: " << content_type << std::endl;
        std::cout << "Body length: " << body.length() << std::endl;

        // Multipart form data parsing
        std::map<std::string, std::string> form_data;
        std::string student_name, student_id, file_content, filename;

        // Basic multipart parsing (split by boundary)
        size_t boundary_pos = content_type.find("boundary=");
        if (boundary_pos != std::string::npos) {
            std::string boundary = content_type.substr(boundary_pos + 9);
            std::vector<std::string> parts;
            
            // Split body by boundary
            size_t start = 0;
            while (true) {
                size_t pos = body.find(boundary, start);
                if (pos == std::string::npos) break;
                
                std::string part = body.substr(start, pos - start);
                if (!part.empty()) parts.push_back(part);
                
                start = pos + boundary.length();
            }

            // Process each part
            for (const auto& part : parts) {
                if (part.find("name=\"studentName\"") != std::string::npos) {
                    size_t content_start = part.find("\r\n\r\n");
                    if (content_start != std::string::npos) {
                        student_name = part.substr(content_start + 4);
                        // Remove trailing newlines
                        student_name.erase(student_name.find_last_not_of("\r\n") + 1);
                    }
                }
                else if (part.find("name=\"studentId\"") != std::string::npos) {
                    size_t content_start = part.find("\r\n\r\n");
                    if (content_start != std::string::npos) {
                        student_id = part.substr(content_start + 4);
                        // Remove trailing newlines
                        student_id.erase(student_id.find_last_not_of("\r\n") + 1);
                    }
                }
                else if (part.find("name=\"pdf_file\"") != std::string::npos) {
                    // Extract filename
                    size_t filename_pos = part.find("filename=\"");
                    if (filename_pos != std::string::npos) {
                        size_t filename_end = part.find("\"", filename_pos + 10);
                        filename = part.substr(filename_pos + 10, filename_end - (filename_pos + 10));
                    }

                    // Extract file content
                    size_t content_start = part.find("\r\n\r\n");
                    if (content_start != std::string::npos) {
                        file_content = part.substr(content_start + 4);
                        // Remove trailing newlines and boundary markers
                        file_content.erase(file_content.find_last_not_of("\r\n-") + 1);
                    }
                }
            }
        }

        // Verify the fields are not empty
        if (student_name.empty() || student_id.empty() || file_content.empty()) {
            return crow::response(400, "Missing required fields.");
        }

        // Save the uploaded file
        std::string saved_file_path = save_uploaded_file(file_content, filename);

        // Debugging output
        std::cout << "Received data:" << std::endl;
        std::cout << "Student Name: " << student_name << std::endl;
        std::cout << "Student ID: " << student_id << std::endl;
        std::cout << "Saved File Path: " << saved_file_path << std::endl;

        // Get the previous block hash from the blockchain
        Blockchain blockchain;
        std::string prevHash = blockchain.getLastBlockHash();

        // Create a new Block with the received data
        Block new_block(prevHash, student_name, student_id, saved_file_path);

        // Add the block to the blockchain
        blockchain.addNewBlock(new_block);

        return crow::response(200, "Block added successfully!");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return crow::response(500, std::string("Error: ") + e.what());
    }
}

int main() {
    crow::SimpleApp app;

    // CORS handling
    app.route_dynamic("/add_block")
        .methods("POST"_method)
        ([](const crow::request& req) {
            // Create a response with CORS headers
            crow::response res = handle_add_block(req);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            return res;
        });

    // Optional: Handle preflight OPTIONS request for CORS
    CROW_ROUTE(app, "/add_block").methods("OPTIONS"_method)
    ([](const crow::request& req) {
        crow::response res;
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.code = 200;
        return res;
    });

    // Start the server
    app.port(18080).multithreaded().run();
}
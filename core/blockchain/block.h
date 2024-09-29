#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <filesystem>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/evp.h>


class Block{
public:
    Block(std::string student_id, std::string cert_hash, std::string prevHash)
        : student_id(student_id), cert_hash(cert_hash), prev_hash(prevHash), timestamp(std::time(nullptr)) {
        pdf_binary = calculateBinary(); // Calculate binary representation on creation
        curr_hash = calculateHash(pdf_binary); // Calculate hash based on binary
    }

    std::string getHash() const { return curr_hash; }
    std::string getBinary() const { return pdf_binary; }
    

private:
    std::string student_name;
    std::string student_id;
    std::string cert_hash;
    std::string prev_hash;
    std::string curr_hash;
    std::string pdf_binary;
    time_t timestamp;



    std::string calculateBinary() const{
        //int const size = 20;
        std::uintmax_t size = std::filesystem::file_size("../../assets/sample.pdf");
        std::cout<<size<<std::endl;

        std::vector<char> ndata(size);
        std::string result_binary;
        //getting input data.pdf
        std::ifstream infile("../../assets/sample.pdf", std::ios::binary);
        if (!infile.read(ndata.data(), size)) {
            std::cerr << "Error reading file." << std::endl;
            return result_binary;
        }

        //writing to output
        for (char c : ndata) {
            std::bitset<8> x(c);
            result_binary += x.to_string();
            //std::cout << x<<std::endl;
        }

        std::cout << "Encoding complete." << std::endl;
	    return result_binary;

    }

    std::string calculateHash(const std::string& binary) const {
            unsigned char hash[SHA256_DIGEST_LENGTH];
            unsigned int hashLength;

            // Create a new digest context
            EVP_MD_CTX* ctx = EVP_MD_CTX_new();
            if (ctx == nullptr) {
                throw std::runtime_error("Failed to create digest context");
            }

            // Initialize the SHA-256 context
            if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to initialize digest");
            }

            // Update the context with the data
            if (EVP_DigestUpdate(ctx, binary.c_str(), binary.size()) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to update digest");
            }

            // Finalize the digest
            if (EVP_DigestFinal_ex(ctx, hash, &hashLength) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to finalize digest");
            }

            // Clean up
            EVP_MD_CTX_free(ctx);

            // Convert hash to hex string
            std::stringstream ss;
            for (unsigned int i = 0; i < hashLength; i++) {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
            }

            return ss.str();
        }

};
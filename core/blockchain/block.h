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
    Block(std::string prevHash, const std::string& studentName, const std::string& studentId, const std::string& pdf_data)
    : student_name(studentName), student_id(studentId), prev_hash(prevHash), timestamp(std::time(nullptr)) {
        setStudentName(studentName);
        setStudentId(studentId);
        setPdfName(pdf_data);

        // Use the PDF data passed as an argument to calculate the binary content
        pdf_binary = calculateBinary(pdf_data);
        
        // Calculate the certificate hash and block hash
        cert_hash = calculateHash(pdf_binary);
        curr_hash = calculateBlockHash();
        updateCurrHash();
    }
    
    friend std::ostream & operator<<(std::ostream & os, Block const & block){
        os <<"Student name: "<< block.student_name <<std::endl
            <<"Student id: "<<block.student_id <<std::endl
            <<"Certificate hash: "<<block.cert_hash <<std::endl
            <<"Previous hash: "<<block.prev_hash <<std::endl
            <<"Current hash: "<<block.curr_hash <<std::endl
            <<"Timestamp: "<<block.timestamp <<std::endl;
        return os;
    }


    //getters
    std::string getBinary() const { return pdf_binary; }
    std::string getCertHash() const { return cert_hash; }
    std::string getCurrHash() const { return curr_hash; }
    std::string getPrevHash() const { return prev_hash; }
    std::string getStudentId() const{ return student_id; }
    std::string getStudentName() const{ return student_name; }
    time_t getTimestamp() const{ return timestamp; }
    //std::string getBlockHash() const { return calculateBlockHash; }

    //setters
    void setHash(const std::string& newHash){
        cert_hash = newHash;
    }

    void setPrevHash(const std::string& new_prev_hash){
        prev_hash = new_prev_hash;
    }

    void setCurrHash(const std::string& new_curr_hash){
        curr_hash = new_curr_hash;
    }

    void updateCurrHash(){
        curr_hash = calculateBlockHash();
    }

    void setStudentName(const std::string& new_student_name){
        student_name = new_student_name;
    }

    void setStudentId(const std::string& new_student_id){
        student_id = new_student_id;
    }

    void setPdfName(const std::string& new_pdf_name){
        pdf_name = new_pdf_name;
    }



private:
    std::string student_name;
    std::string student_id;
    std::string cert_hash;
    std::string prev_hash;
    std::string curr_hash;
    std::string pdf_binary;
    std::string pdf_name;
    time_t timestamp;



    std::string calculateBinary(const std::string& pdf_data) const {
        std::string result_binary;
        
        // Convert each byte of the PDF data to a binary string
        for (unsigned char byte : pdf_data) {
            std::bitset<8> bit(byte);  // Convert byte to 8-bit binary
            result_binary += bit.to_string();  // Append the binary string representation
        }

        std::cout << "PDF encoding complete." << std::endl;
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


        std::string calculateBlockHash() const {
            std::stringstream block_data;
            block_data << student_id << cert_hash << prev_hash;

            return calculateHash(block_data.str());
        }
};
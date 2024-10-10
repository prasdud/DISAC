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
    Block(std::string prevHash)
    : student_id(""), prev_hash(prevHash), timestamp(std::time(nullptr)) {
        std::string tempName;
        std::string tempId;
        std::string tempPdf;
        std::cout << "Enter the student name -> " << std::endl;
        std::cin>> tempName; // Use getline to capture full name
        setStudentName(tempName);
        
        std::cout << "Enter the student id -> " << std::endl;
        std::cin >> tempId;
        setStudentId(tempId);
        
        std::cout << "Enter the pdf file -> " << std::endl;
        std::cin >> tempPdf;
        setPdfName(tempPdf);
        
        pdf_binary = calculateBinary(); // Calculate binary representation on creation
        cert_hash = calculateHash(pdf_binary); // Calculate hash based on binary
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



    std::string calculateBinary() const{
        //int const size = 20;
        std::uintmax_t size = std::filesystem::file_size("../../assets/dummy-certs/"+pdf_name+".pdf");
        //std::cout<<size<<std::endl;

        std::vector<char> ndata(size);
        std::string result_binary;
        //getting input data.pdf
        std::ifstream infile("../../assets/dummy-certs/"+pdf_name+".pdf", std::ios::binary);
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


        std::string calculateBlockHash() const {
            std::stringstream block_data;
            block_data << student_id << cert_hash << prev_hash << timestamp;

            return calculateHash(block_data.str());
        }
};
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
    Block(std::string prevHash, const std::string& studentName, const std::string& studentId, const std::string& pdfFileName)
        : student_name(studentName), student_id(studentId), prev_hash(prevHash), timestamp(std::time(nullptr)) {
        // Attempt to resolve the file path
        std::vector<std::string> possible_paths = {
            "../api/" + pdfFileName,     // Original path
            "uploads/" + pdfFileName,    // Upload directory
            pdfFileName                  // Direct path
        };

        std::string resolved_path;
        for (const auto& path : possible_paths) {
            if (std::filesystem::exists(path)) {
                resolved_path = path;
                break;
            }
        }

        if (resolved_path.empty()) {
            throw std::runtime_error("Could not locate PDF file: " + pdfFileName);
        }

        // Debugging path resolution
        std::cout << "Resolved PDF Path: " << resolved_path << std::endl;

        // Calculate PDF hash
        cert_hash = calculatePDFHash(resolved_path);
        
        // Set other details
        setStudentName(studentName);
        setStudentId(studentId);
        setPdfName(pdfFileName);

        // Calculate block hash
        curr_hash = calculateBlockHash();
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

    static std::string calculateBinary(const std::string& pdf_file_path) {
        std::string result_binary;

        // Open the file in binary mode
        std::ifstream pdf_file(pdf_file_path, std::ios::binary);
        if (!pdf_file) {
            throw std::runtime_error("Failed to open PDF file: " + pdf_file_path);
        }

        // Read the content of the file
        std::vector<unsigned char> file_content((std::istreambuf_iterator<char>(pdf_file)),
                                                std::istreambuf_iterator<char>());
        pdf_file.close();

        // Convert each byte of the file content to a binary string
        for (unsigned char byte : file_content) {
            std::bitset<8> bit(byte); // Convert byte to 8-bit binary
            result_binary += bit.to_string(); // Append the binary string representation
        }

        std::cout << "PDF content successfully converted to binary." + pdf_file_path << std::endl;
        std::cout << result_binary<< std::endl;
        return result_binary;
    }


    static std::string calculatePDFHash(const std::string& pdf_file_path){
        std::ifstream file(pdf_file_path, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open PDF file: " + pdf_file_path);
        }
        std:: cout<<"Opened pdf file successfully! "+ pdf_file_path<<std::endl;
        // Create SHA-256 context
        SHA256_CTX sha256;
        SHA256_Init(&sha256);

        // Read file in chunks
        const int BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        
        while (file.read(buffer, BUFFER_SIZE)) {
            SHA256_Update(&sha256, buffer, file.gcount());
        }

        // Handle any remaining bytes
        if (file.gcount() > 0) {
            SHA256_Update(&sha256, buffer, file.gcount());
        }

        // Finalize hash
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_Final(hash, &sha256);

        // Convert to hex string
        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        std::string hash_result = ss.str();

        // Extensive debugging
        std::cout << "PDF File Path: " << pdf_file_path << std::endl;
        std::cout << "Calculated PDF SHA-256 Hash: " << hash_result << std::endl;
        std::cout << "Hash Length: " << hash_result.length() << std::endl;
        std::cout<<hash_result<<std::endl;
        return hash_result;
    }
    static std::string calculateBinaryFromFile(const std::string& pdfPath) {
    std::ifstream pdfFile(pdfPath, std::ios::binary);
    if (!pdfFile) {
        throw std::runtime_error("Failed to open PDF file: " + pdfPath);
    }

    std::ostringstream oss;
    oss << pdfFile.rdbuf();
    std::string pdfContent = oss.str();

    return calculateBinary(pdfContent);
}

    static std::string calculateHash(const std::string& binaryData) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        unsigned int hashLength;

        // Create and finalize SHA-256 hash
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, binaryData.c_str(), binaryData.size());
        EVP_DigestFinal_ex(ctx, hash, &hashLength);
        EVP_MD_CTX_free(ctx);

        // Convert hash to hex string
        std::stringstream ss;
        for (unsigned int i = 0; i < hashLength; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return ss.str();
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





    std::string calculateBlockHash() {
        std::stringstream block_data;
        block_data << student_id << cert_hash << prev_hash;
        
        // Use SHA-256 for block hash as well
        SHA256_CTX sha256;
        SHA256_Init(&sha256);

        std::string data = block_data.str();
        SHA256_Update(&sha256, data.c_str(), data.length());

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        return ss.str();
    }
};
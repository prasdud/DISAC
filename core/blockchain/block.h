#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <filesystem>
#include <cstdint>

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

    std::string calculateHash(std::string binary) const{
        
        return cert_hash;
    }


};
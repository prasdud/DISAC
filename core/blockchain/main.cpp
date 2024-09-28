#include <iostream>
#include "block.h"
#include "blockchain.h"
#include <string>

int main(int argc, char const *argv[])
{
    // Provide appropriate values for the Block constructor parameters
    std::string student_id = "12345"; // Example student ID
    std::string cert_hash = "abcde12345"; // Example certificate hash
    std::string prevHash = "00000"; // Example previous hash

    // Create a Block instance
    Block b1(student_id, cert_hash, prevHash);

    // Get the binary representation
    std::string binary = b1.getBinary();
    std::string hash = b1.getHash();
    
    // Output the binary representation
    std::cout << "Binary Representation: " << binary << std::endl;

    return 0;
}

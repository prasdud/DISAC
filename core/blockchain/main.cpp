#include <iostream>
#include "block.h"
#include "blockchain.h"
#include <string>
#include <cassert>

void testGenesisBlock(){
    Blockchain blockchain;
    Block genesisBlock = blockchain.getLastBlock();
    std::cout << "Testing genesis block:"<< std::endl;
    std::cout << genesisBlock << std::endl;
    //assert(genesisBlock.getPrevHash() == "000000");
    //assert(genesisBlock.getCertHash() != "");
}

void testBlockChain() {
    // Create the blockchain
    Blockchain blockchain;
    // Create and add new blocks
    Block block1("Student1", blockchain.getLastBlock().getCurrHash());
    blockchain.addNewBlock(block1);

    Block block2("Student2", blockchain.getLastBlock().getCurrHash());
    blockchain.addNewBlock(block2);

    // Print the blockchain
    std::cout << "Blockchain: " << std::endl;
    std::cout << blockchain.getLastBlock() << std::endl;
}

int main(int argc, char const *argv[])
{
    // std::string student_id = "12345"; // sample student ID
    // std::string prevHash = "00000"; // sample previous hash

    // // Block instance
    // Block b1(student_id, prevHash);
    // std::cout<<b1<<std::endl;

    // // Get the binary and hash representation
    // std::string binary = b1.getBinary();
    // std::string hash = b1.getCertHash();
    
    // // Output the binary and hash representation
    // //std::cout << "Binary Representation: " << binary << std::endl;
    // std::cout << "Hash Representation: " << hash << std::endl;
    // std::cout<<b1<<std::endl;

    std:: cout<<"starting test genesis block "<<std::endl;
    testGenesisBlock();
    std:: cout<<"finished test genesis block "<<std::endl;
    //std:: cout<<"starting test blockchain"<<std::endl;
    //testBlockChain();
    //std:: cout<<"finishing test blockchain"<<std::endl;

    return 0;
}